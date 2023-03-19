#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "board.h"

#define BOARDBITSIZE (sizeof(BitBoard) * 8)
#define PRINTF_BINARY_PATTERN_INT9 "%d %c%c%c%c%c%c%c%c%c\n"
#define PRINTF_BYTE_TO_BINARY_INT9(i) \
    (((i)&0x100) ? '1' : '0'),        \
        (((i)&0x80) ? '1' : '0'),     \
        (((i)&0x40) ? '1' : '0'),     \
        (((i)&0x20) ? '1' : '0'),     \
        (((i)&0x10) ? '1' : '0'),     \
        (((i)&0x08) ? '1' : '0'),     \
        (((i)&0x04) ? '1' : '0'),     \
        (((i)&0x02) ? '1' : '0'),     \
        (((i)&0x01) ? '1' : '0')

const char *FEN = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";

BitBoard mask[BOARDLENGTH];

void initMask()
{
    for (int i = 0; i < BOARDLENGTH; ++i)
        mask[i] = (BitBoard)1 << (BOARDBITSIZE - i - 1);
}

static PlayerPieceKind getPlayerKind_ch(char ch)
{
    PieceKind kind;
    switch (toupper(ch))
    {
    case 'K':
        kind = KING;
        break;
    case 'A':
        kind = ADVISOR;
        break;
    case 'B':
        kind = BISHOP;
        break;
    case 'N':
        kind = KNIGHT;
        break;
    case 'R':
        kind = ROOK;
        break;
    case 'C':
        kind = CANON;
        break;
    default:
        kind = PAWN;
        break;
    }

    return (PlayerPieceKind){isupper(ch) ? RED : BLACK, kind};
}

static char getPieceChar(PlayerPieceKind playerKind)
{
    char ch;
    switch (playerKind.kind)
    {
    case KING:
        ch = 'K';
        break;
    case ADVISOR:
        ch = 'A';
        break;
    case BISHOP:
        ch = 'B';
        break;
    case KNIGHT:
        ch = 'N';
        break;
    case ROOK:
        ch = 'R';
        break;
    case CANON:
        ch = 'C';
        break;
    default:
        ch = 'P';
        break;
    }

    return playerKind.player == RED ? ch : tolower(ch);
}

static char *setPieChars(char *pieChars, const char *fen)
{
    pieChars[0] = '\x0';
    int len = strlen(fen);
    for (int index = 0, i = 0; i < len && index < BOARDLENGTH; ++i)
    {
        char ch = fen[i];
        if (isdigit(ch))
            for (int j = ch - '0'; j > 0; --j)
                pieChars[index++] = '_';
        else if (isalpha(ch))
            pieChars[index++] = ch;
    }

    pieChars[BOARDLENGTH] = '\x0';
    return pieChars;
}

static char *setFen(char *fen, const char *pieChars)
{
    fen[0] = '\x0';
    if (strlen(pieChars) != BOARDLENGTH)
        return fen;

    int index_F = 0;
    for (int row = 0; row < BOARDROWNUM; ++row)
    { // 从最高行开始
        int blankNum = 0;
        for (int col = 0; col < BOARDCOLNUM; ++col)
        {
            int index_p = row * BOARDCOLNUM + col;
            if (isalpha(pieChars[index_p]))
            {
                if (blankNum > 0)
                    fen[index_F++] = '0' + blankNum;

                fen[index_F++] = pieChars[index_p];
                blankNum = 0;
            }
            else if (pieChars[index_p] == '_')
                blankNum++;
        }
        if (blankNum > 0)
            fen[index_F++] = '0' + blankNum;

        fen[index_F++] = '/'; // 插入行分隔符
    }

    fen[--index_F] = '\x0';
    return fen;
}

static int getPieceIndex(PlayerPieceKind playerKind)
{
    return playerKind.player * PIECEKINDNUM + playerKind.kind;
}

static PlayerPieceKind getPlayerKind_index(int index)
{
    return index < PIECEKINDNUM ? (PlayerPieceKind){0, index} : (PlayerPieceKind){1, index - PIECEKINDNUM};
}

ChessPosition *setChessPosition(ChessPosition *chess, const char *fen)
{
    char pieChars[BOARDLENGTH];
    setPieChars(pieChars, fen);

    int len = strlen(pieChars);
    for (int i = 0; i < len; ++i)
    {
        if (!isalpha(pieChars[i]))
            continue;

        PlayerPieceKind playerKind = getPlayerKind_ch(pieChars[i]);
        int index = getPieceIndex(playerKind);

        chess->pieces[index] |= mask[i];
    }

    return chess;
}

char *setFen_Chess(char *fen, const ChessPosition *chess)
{
    char pieChars[BOARDLENGTH] = {};
    for (int i = 0; i < PLAYERNUM * PIECEKINDNUM; ++i)
    {
        PlayerPieceKind playerKind = getPlayerKind_index(i);
        int index = getPieceIndex(playerKind);
        char ch = getPieceChar(playerKind);
        BitBoard board = chess->pieces[index];
        for (int j = 0; j < BOARDLENGTH; ++j)
        {
            if (board & mask[j])
                pieChars[j] = ch;
        }
    }

    return setFen(fen, pieChars);
}

// dest最小尺寸: (BOARDROWNUM + 2) * 16
static char *sprintBitBoard(char *dest, BitBoard board)
{
    snprintf(dest, 16, "  ABCDEFGHI\n");
    for (int i = 0; i < BOARDROWNUM; i++)
    {
        int offset = BOARDBITSIZE - ((i + 1) * BOARDCOLNUM);
        int row = (board & ((BitBoard)0x1FF << offset)) >> offset;
        char buffer[16];
        snprintf(buffer, 16, PRINTF_BINARY_PATTERN_INT9, i, PRINTF_BYTE_TO_BINARY_INT9(row));
        strcat(dest, buffer);
    }

    return dest;
}

static void printBoard(BitBoard board)
{
    char boardStr[(BOARDROWNUM + 2) * 16];
    sprintBitBoard(boardStr, board);
    printf("%s", boardStr);
}

static void printBoardArray(const BitBoard *boards, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%d:\n", i);
        printBoard(boards[i]);
        printf("\n");
    }
}

void testMask()
{
    printBoardArray(mask, BOARDLENGTH);
}

void testFenPieChars()
{
    const char *fens[] = {
        FEN,
        "5a3/4ak2r/6R2/8p/9/9/9/B4N2B/4K4/3c5",
        "2b1kab2/4a4/4c4/9/9/3R5/9/1C7/4r4/2BK2B2",
        "4kab2/4a4/4b4/3N5/9/4N4/4n4/4B4/4A4/3AK1B2",
    };

    for (int i = 0; i < 4; ++i)
    {
        printf("FEN: %s\n", fens[i]);

        char pieChars[BOARDLENGTH + 3];
        setPieChars(pieChars, fens[i]);
        printf("PieChars: %s\n", pieChars);

        char fen[BOARDLENGTH];
        setFen(fen, pieChars);
        printf("fen: %s\nfen.Equal: %d\n\n", fen, strcmp(fen, fens[i]));
    }
}

void testChessPosition()
{
    ChessPosition chess = {};
    setChessPosition(&chess, FEN);

    printf("player: %d\n", chess.curPlayer);
    printBoardArray(chess.pieces, PLAYERNUM * PIECEKINDNUM);

    char fen[BOARDLENGTH] = {};
    setFen_Chess(fen, &chess);
    printf("setFen: %s\nFEN.Equal: %d\n\n", fen, strcmp(fen, FEN));
}