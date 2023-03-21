#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "board.h"


#define BINARYPATTERN9 "%c%c%c%c%c%c%c%c%c "
#define BYTEBINARY9(i)            \
    (((i)&0x100) ? '1' : '0'),    \
        (((i)&0x80) ? '1' : '0'), \
        (((i)&0x40) ? '1' : '0'), \
        (((i)&0x20) ? '1' : '0'), \
        (((i)&0x10) ? '1' : '0'), \
        (((i)&0x08) ? '1' : '0'), \
        (((i)&0x04) ? '1' : '0'), \
        (((i)&0x02) ? '1' : '0'), \
        (((i)&0x01) ? '1' : '0')

typedef enum Color
{
    RED,
    BLACK,
    ALLCOLOR
} Color;

typedef enum Kind
{
    KING,
    ADVISOR,
    BISHOP,
    KNIGHT,
    ROOK,
    CANON,
    PAWN
} Kind;

typedef struct ColorKind
{
    Color color;
    Kind kind;
} ColorKind;

typedef struct ChessPosition
{
    // 当前
    Color player;

    // 基本局面
    Board pieces[COLORNUM][KINDNUM];

    // 计算中间存储数据(基本局面改动时更新)
    Board colorPieces[COLORNUM + 1];

} ChessPosition;

const char NullChar = '_';
const char SplitChar = '/';
const char EndChar = '\x0';

const int PieceNum[] = {1, 2, 2, 2, 2, 2, 5};
const char *Chars[] = {"KABNRCP", "kabnrcp"};

const char FEN[] = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";

// 打印初始化器的内容
// for (int i = 0; i < BOARDLENGTH; ++i)
//     printf("(Board)1 << (BOARDBITSIZE - 1 - %d),\n", i);
const Board BoardMask[BOARDLENGTH] = {
    (Board)1 << (BOARDBITSIZE - 1 - 0),
    (Board)1 << (BOARDBITSIZE - 1 - 1),
    (Board)1 << (BOARDBITSIZE - 1 - 2),
    (Board)1 << (BOARDBITSIZE - 1 - 3),
    (Board)1 << (BOARDBITSIZE - 1 - 4),
    (Board)1 << (BOARDBITSIZE - 1 - 5),
    (Board)1 << (BOARDBITSIZE - 1 - 6),
    (Board)1 << (BOARDBITSIZE - 1 - 7),
    (Board)1 << (BOARDBITSIZE - 1 - 8),
    (Board)1 << (BOARDBITSIZE - 1 - 9),
    (Board)1 << (BOARDBITSIZE - 1 - 10),
    (Board)1 << (BOARDBITSIZE - 1 - 11),
    (Board)1 << (BOARDBITSIZE - 1 - 12),
    (Board)1 << (BOARDBITSIZE - 1 - 13),
    (Board)1 << (BOARDBITSIZE - 1 - 14),
    (Board)1 << (BOARDBITSIZE - 1 - 15),
    (Board)1 << (BOARDBITSIZE - 1 - 16),
    (Board)1 << (BOARDBITSIZE - 1 - 17),
    (Board)1 << (BOARDBITSIZE - 1 - 18),
    (Board)1 << (BOARDBITSIZE - 1 - 19),
    (Board)1 << (BOARDBITSIZE - 1 - 20),
    (Board)1 << (BOARDBITSIZE - 1 - 21),
    (Board)1 << (BOARDBITSIZE - 1 - 22),
    (Board)1 << (BOARDBITSIZE - 1 - 23),
    (Board)1 << (BOARDBITSIZE - 1 - 24),
    (Board)1 << (BOARDBITSIZE - 1 - 25),
    (Board)1 << (BOARDBITSIZE - 1 - 26),
    (Board)1 << (BOARDBITSIZE - 1 - 27),
    (Board)1 << (BOARDBITSIZE - 1 - 28),
    (Board)1 << (BOARDBITSIZE - 1 - 29),
    (Board)1 << (BOARDBITSIZE - 1 - 30),
    (Board)1 << (BOARDBITSIZE - 1 - 31),
    (Board)1 << (BOARDBITSIZE - 1 - 32),
    (Board)1 << (BOARDBITSIZE - 1 - 33),
    (Board)1 << (BOARDBITSIZE - 1 - 34),
    (Board)1 << (BOARDBITSIZE - 1 - 35),
    (Board)1 << (BOARDBITSIZE - 1 - 36),
    (Board)1 << (BOARDBITSIZE - 1 - 37),
    (Board)1 << (BOARDBITSIZE - 1 - 38),
    (Board)1 << (BOARDBITSIZE - 1 - 39),
    (Board)1 << (BOARDBITSIZE - 1 - 40),
    (Board)1 << (BOARDBITSIZE - 1 - 41),
    (Board)1 << (BOARDBITSIZE - 1 - 42),
    (Board)1 << (BOARDBITSIZE - 1 - 43),
    (Board)1 << (BOARDBITSIZE - 1 - 44),
    (Board)1 << (BOARDBITSIZE - 1 - 45),
    (Board)1 << (BOARDBITSIZE - 1 - 46),
    (Board)1 << (BOARDBITSIZE - 1 - 47),
    (Board)1 << (BOARDBITSIZE - 1 - 48),
    (Board)1 << (BOARDBITSIZE - 1 - 49),
    (Board)1 << (BOARDBITSIZE - 1 - 50),
    (Board)1 << (BOARDBITSIZE - 1 - 51),
    (Board)1 << (BOARDBITSIZE - 1 - 52),
    (Board)1 << (BOARDBITSIZE - 1 - 53),
    (Board)1 << (BOARDBITSIZE - 1 - 54),
    (Board)1 << (BOARDBITSIZE - 1 - 55),
    (Board)1 << (BOARDBITSIZE - 1 - 56),
    (Board)1 << (BOARDBITSIZE - 1 - 57),
    (Board)1 << (BOARDBITSIZE - 1 - 58),
    (Board)1 << (BOARDBITSIZE - 1 - 59),
    (Board)1 << (BOARDBITSIZE - 1 - 60),
    (Board)1 << (BOARDBITSIZE - 1 - 61),
    (Board)1 << (BOARDBITSIZE - 1 - 62),
    (Board)1 << (BOARDBITSIZE - 1 - 63),
    (Board)1 << (BOARDBITSIZE - 1 - 64),
    (Board)1 << (BOARDBITSIZE - 1 - 65),
    (Board)1 << (BOARDBITSIZE - 1 - 66),
    (Board)1 << (BOARDBITSIZE - 1 - 67),
    (Board)1 << (BOARDBITSIZE - 1 - 68),
    (Board)1 << (BOARDBITSIZE - 1 - 69),
    (Board)1 << (BOARDBITSIZE - 1 - 70),
    (Board)1 << (BOARDBITSIZE - 1 - 71),
    (Board)1 << (BOARDBITSIZE - 1 - 72),
    (Board)1 << (BOARDBITSIZE - 1 - 73),
    (Board)1 << (BOARDBITSIZE - 1 - 74),
    (Board)1 << (BOARDBITSIZE - 1 - 75),
    (Board)1 << (BOARDBITSIZE - 1 - 76),
    (Board)1 << (BOARDBITSIZE - 1 - 77),
    (Board)1 << (BOARDBITSIZE - 1 - 78),
    (Board)1 << (BOARDBITSIZE - 1 - 79),
    (Board)1 << (BOARDBITSIZE - 1 - 80),
    (Board)1 << (BOARDBITSIZE - 1 - 81),
    (Board)1 << (BOARDBITSIZE - 1 - 82),
    (Board)1 << (BOARDBITSIZE - 1 - 83),
    (Board)1 << (BOARDBITSIZE - 1 - 84),
    (Board)1 << (BOARDBITSIZE - 1 - 85),
    (Board)1 << (BOARDBITSIZE - 1 - 86),
    (Board)1 << (BOARDBITSIZE - 1 - 87),
    (Board)1 << (BOARDBITSIZE - 1 - 88),
    (Board)1 << (BOARDBITSIZE - 1 - 89)};

static inline char getPieceChar(ColorKind colorKind)
{
    return Chars[colorKind.color][colorKind.kind];
}

static inline ColorKind getColorKind(char ch)
{
    Color color = isupper(ch) ? RED : BLACK;
    Kind kind = strchr(Chars[color], ch) - Chars[color];
    return (ColorKind){color, kind};
}

static char *setPieCharsFromFen(char *pieChars, const char *fen)
{
    int len = strlen(fen);
    for (int pieIndex = 0, fenIndex = 0; fenIndex < len && pieIndex < BOARDLENGTH; ++fenIndex)
    {
        char ch = fen[fenIndex];
        if (isdigit(ch))
            for (int j = ch - '0'; j > 0; --j)
                pieChars[pieIndex++] = NullChar;
        else if (isalpha(ch))
            pieChars[pieIndex++] = ch;
    }

    pieChars[BOARDLENGTH] = EndChar;
    return pieChars;
}

static char *setFenFromPieChars(char *fen, const char *pieChars)
{
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
            else if (pieChars[index_p] == NullChar)
                blankNum++;
        }
        if (blankNum > 0)
            fen[index_F++] = '0' + blankNum;

        fen[index_F++] = SplitChar; // 插入行分隔符
    }

    fen[--index_F] = EndChar;
    return fen;
}

// 求最高位非零位的序号，调用前判断参数非全零位
static int getNonZeroIndex(Board board)
{
    int index = 0;
    __uint64_t value = board & 0XFFFFFFC000000000UL; // 64-89 位
    if (value)
        index = 64;
    else
        value = board >> 64; // 00-63 位

    if (value & 0X00000000FFFFFFFFUL)
    {
        index += 32;
        value &= 0X00000000FFFFFFFFUL;
    }

    if (value & 0X0000FFFF0000FFFFUL)
    {
        index += 16;
        value &= 0X0000FFFF0000FFFFUL;
    }

    if (value & 0X00FF00FF00FF00FFUL)
    {
        index += 8;
        value &= 0X00FF00FF00FF00FFUL;
    }

    if (value & 0X0F0F0F0F0F0F0F0FUL)
    {
        index += 4;
        value &= 0X0F0F0F0F0F0F0F0FUL;
    }

    if (value & 0X3333333333333333UL)
    {
        index += 2;
        value &= 0X3333333333333333UL;
    }

    if (value & 0X5555555555555555UL)
        index += 1;

    return index;
}

ChessPosition *updateChessPositionData(ChessPosition *chess)
{
    for (Color color = RED; color <= BLACK; ++color)
        for (Kind kind = KING; kind <= PAWN; ++kind)
            chess->colorPieces[color] |= chess->pieces[color][kind];

    chess->colorPieces[ALLCOLOR] = chess->colorPieces[RED] | chess->colorPieces[BLACK];
    return chess;
}

ChessPosition *setChessPositionFromFen(ChessPosition *chess, const char *fen)
{
    char pieChars[BOARDLENGTH + 1] = {};
    setPieCharsFromFen(pieChars, fen);

    for (int index = 0; index < BOARDLENGTH; ++index)
    {
        char ch = pieChars[index];
        if (ch == NullChar)
            continue;

        ColorKind colorKind = getColorKind(ch);
        chess->pieces[colorKind.color][colorKind.kind] |= BoardMask[index];
    }

    updateChessPositionData(chess);
    return chess;
}

char *setFenFromChessPosition(char *fen, const ChessPosition *chess)
{
    char pieChars[BOARDLENGTH + 1];
    for (int i = 0; i < BOARDLENGTH; ++i)
        pieChars[i] = NullChar;

    for (Color color = RED; color <= BLACK; ++color)
    {
        for (Kind kind = KING; kind <= PAWN; ++kind)
        {
            char ch = getPieceChar((ColorKind){color, kind});
            Board board = chess->pieces[color][kind];
            while (board) // >> BOARDOTHERSIZE
            {
                int index = getNonZeroIndex(board);
                pieChars[index] = ch;
                board ^= BoardMask[index];
            }
        }
    }

    pieChars[BOARDLENGTH] = EndChar;
    return setFenFromPieChars(fen, pieChars);
}

static char *getBoardStr(char *boardStr, const Board *boards, int length, int colNum)
{
    if (length < colNum)
        colNum = length;

    char temp[16],
        indexRowStr[colNum * 16],
        nullRowStr[colNum * 16];
    strcpy(nullRowStr, "  ");
    for (int col = 0; col < colNum; ++col)
        strcat(nullRowStr, " ABCDEFGHI");
    strcat(nullRowStr, "\n");

    strcpy(boardStr, "");
    for (int index = 0; index < length; index += colNum)
    {
        strcpy(indexRowStr, "   ");
        for (int col = 0; col < colNum; ++col)
        {
            snprintf(temp, 16, "%02d:       ", index + col);
            strcat(indexRowStr, temp);
        }
        strcat(indexRowStr, "\n");
        strcat(boardStr, indexRowStr);
        strcat(boardStr, nullRowStr);

        for (int row = 0; row < BOARDROWNUM; ++row)
        {
            snprintf(temp, 16, "%d: ", row);
            strcat(boardStr, temp);
            int offset = BOARDOTHERSIZE + (BOARDROWNUM - 1 - row) * BOARDCOLNUM;
            for (int col = 0; col < colNum; ++col)
            {
                int rowInt = (boards[index + col] >> offset) & 0x1FF;
                snprintf(temp, 16, BINARYPATTERN9, BYTEBINARY9(rowInt));
                strcat(boardStr, temp);
            }
            strcat(boardStr, "\n");
        }
        // strcat(boardStr, "\n");
    }

    return boardStr;
}

static char *getChessPositionStr(char *chessStr, ChessPosition *chess)
{
    static const char *colorStrs[] = {"RED", "BLACK", "ALLCOLOR"};
    char temp[1024];
    strcpy(chessStr, "");
    for (Color color = RED; color <= ALLCOLOR; ++color)
    {
        snprintf(temp, 32, "color: %s\n", colorStrs[color]);
        strcat(chessStr, temp);

        char boardStr[KINDNUM * (BOARDROWNUM + 2) * 16];
        getBoardStr(boardStr, chess->pieces[color], color == ALLCOLOR ? COLORNUM + 1 : KINDNUM, KINDNUM);
        strcat(chessStr, boardStr);
    }

    return chessStr;
}

void testBoardMask()
{
    char boardStr[BOARDLENGTH * (BOARDROWNUM + 2) * 16];
    getBoardStr(boardStr, BoardMask, BOARDLENGTH, BOARDCOLNUM);
    printf("testBoardMask:\n%s\n", boardStr);
}

const char *fens[] = {
    FEN,
    "5a3/4ak2r/6R2/8p/9/9/9/B4N2B/4K4/3c5",
    "2b1kab2/4a4/4c4/9/9/3R5/9/1C7/4r4/2BK2B2",
    "4kab2/4a4/4b4/3N5/9/4N4/4n4/4B4/4A4/3AK1B2",
};

void testFenPieChars()
{
    printf("testFenPieChars:\n");
    for (int i = 0; i < 4; ++i)
    {
        const char *afen = fens[i];
        char pieChars[BOARDLENGTH + 1] = {};
        setPieCharsFromFen(pieChars, afen);

        char fen[BOARDLENGTH] = {};
        setFenFromPieChars(fen, pieChars);
        printf("PieChars: %s\nafen: %s\n fen: %s\n fen.Equal: %d\n\n",
               pieChars, afen, fen, strcmp(fen, afen));
    }
}

void testChessPosition()
{
    printf("testChessPosition:\n");
    for (int i = 0; i < 4; ++i)
    {
        const char *afen = fens[i];
        ChessPosition chess = {};
        setChessPositionFromFen(&chess, afen);

        char chessStr[4 * 4096];
        getChessPositionStr(chessStr, &chess);

        char fen[BOARDLENGTH] = {};
        setFenFromChessPosition(fen, &chess);

        printf("afen: %s\n%ssetFen: %s\nfen.Equal: %d\n\n", afen, chessStr, fen, strcmp(fen, afen));
    }
}