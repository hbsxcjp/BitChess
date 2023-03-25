#include "data.h"
#include "board.h"
#include "move.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

static const char NullChar = '_';

static const char SplitChar = '/';

static const char EndChar = '\x0';

static const char Chars[COLORNUM][KINDNUM] = {"KABNRCP", "kabnrcp"};

static const char FEN[] = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";

static const wchar_t Names[COLORNUM][KINDNUM] = {L"帅仕相马车炮兵", L"将士象马车炮卒"};

// static const wchar_t NumChar[COLORNUM][BOARDCOLNUM] = {L"一二三四五六七八九", L"１２３４５６７８９"};

// static const wchar_t PreChar[] = L"前中后";

// static const wchar_t MoveChar[] = L"退平进";

static inline Color getColor(char ch)
{
    return isupper(ch) ? RED : BLACK;
}

static inline Kind getKind(char ch)
{
    Color color = getColor(ch);
    return strchr(Chars[color], ch) - Chars[color];
}

static inline wchar_t getPrintName(Color color, Kind kind)
{
    return (color == BLACK && kind > BISHOP && kind < PAWN) ? L"馬車砲"[kind - KNIGHT] : Names[color][kind];
}

static char *setPieCharsFromFen(char *pieChars, const char *fen)
{
    int len = strlen(fen);
    for (int index = 0, fenIndex = 0; fenIndex < len && index < BOARDLENGTH; ++fenIndex)
    {
        char ch = fen[fenIndex];
        if (isdigit(ch))
            for (int j = ch - '0'; j > 0; --j)
                pieChars[index++] = NullChar;
        else if (isalpha(ch))
            pieChars[index++] = ch;
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

// 求最低位非零位的序号，调用前判断参数非全零位
static int getLowNonZeroIndex(Board board)
{
    int index = BOARDBITSIZE - 1;
    uint64_t value = board & 0XFFFFFFFFFFFFFFFFUL; // 00-63 位
    if (value)
        index -= 64;
    else
        value = board >> 64; // 64-89 位

    if (value & 0X00000000FFFFFFFFUL)
    {
        index -= 32;
        value &= 0X00000000FFFFFFFFUL;
    }

    if (value & 0X0000FFFF0000FFFFUL)
    {
        index -= 16;
        value &= 0X0000FFFF0000FFFFUL;
    }

    if (value & 0X00FF00FF00FF00FFUL)
    {
        index -= 8;
        value &= 0X00FF00FF00FF00FFUL;
    }

    if (value & 0X0F0F0F0F0F0F0F0FUL)
    {
        index -= 4;
        value &= 0X0F0F0F0F0F0F0F0FUL;
    }

    if (value & 0X3333333333333333UL)
    {
        index -= 2;
        value &= 0X3333333333333333UL;
    }

    if (value & 0X5555555555555555UL)
        index -= 1;

    return index;
}

// 求最高位非零位的序号，调用前判断参数非全零位
static int getHighNonZeroIndex(Board board)
{
    int index = 0;
    uint64_t value = board >> 64; // 64-89 位
    if (value)
        index += 64;
    else
        value = board & 0XFFFFFFFFFFFFFFFFUL; // 00-63 位

    if (value & 0XFFFFFFFF00000000UL)
    {
        index += 32;
        value &= 0XFFFFFFFF00000000UL;
    }

    if (value & 0XFFFF0000FFFF0000UL)
    {
        index += 16;
        value &= 0XFFFF0000FFFF0000UL;
    }

    if (value & 0XFF00FF00FF00FF00UL)
    {
        index += 8;
        value &= 0XFF00FF00FF00FF00UL;
    }

    if (value & 0XF0F0F0F0F0F0F0F0UL)
    {
        index += 4;
        value &= 0XF0F0F0F0F0F0F0F0UL;
    }

    if (value & 0XCCCCCCCCCCCCCCCCUL)
    {
        index += 2;
        value &= 0XCCCCCCCCCCCCCCCCUL;
    }

    if (value & 0XAAAAAAAAAAAAAAAAUL)
        index += 1;

    return index;
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

        chess->pieces[getColor(ch)][getKind(ch)] |= BoardMask[index];
    }

    // 设置分颜色棋盘
    for (Color color = RED; color <= BLACK; ++color)
        for (Kind kind = KING; kind <= PAWN; ++kind)
            chess->colorPieces[color] |= chess->pieces[color][kind];

    chess->colorPieces[ALLCOLOR] = chess->colorPieces[RED] | chess->colorPieces[BLACK];
    return chess;
}

char *setFenFromChessPosition(char *fen, const ChessPosition *chess)
{
    char pieChars[BOARDLENGTH + 1];
    for (int i = 0; i < BOARDLENGTH; ++i)
        pieChars[i] = NullChar;

    for (Color color = RED; color <= BLACK; ++color)
    {
        int (*getNonZeroIndex)(Board) = color == BLACK ? getHighNonZeroIndex : getLowNonZeroIndex;
        for (Kind kind = KING; kind <= PAWN; ++kind)
        {
            char ch = Chars[color][kind];
            Board board = chess->pieces[color][kind];
            while (board)
            {
                int index = getNonZeroIndex(board);
                // int index = getLowNonZeroIndex(board);
                // int index = getHighNonZeroIndex(board);
                pieChars[index] = ch;
                board ^= BoardMask[index];
            }
        }
    }

    pieChars[BOARDLENGTH] = EndChar;
    return setFenFromPieChars(fen, pieChars);
}

ChessPosition *doMoveChessPosition(ChessPosition *chess, Color color, Kind kind, int fromIndex, int toIndex)
{
    chess->player = !chess->player;

    Board moveBoard = BoardMask[fromIndex] | BoardMask[toIndex];
    chess->colorPieces[color] ^= moveBoard;
    chess->pieces[color][kind] ^= moveBoard; // 清除原位置，置位新位置

    Color toColor = !color;
    chess->colorPieces[toColor] ^= BoardMask[toIndex];
    for (Kind toKind = KING; toKind <= PAWN; ++toKind)
        chess->pieces[toColor][toKind] ^= BoardMask[toIndex]; // 清除新位置

    chess->colorPieces[ALLCOLOR] = chess->colorPieces[RED] | chess->colorPieces[BLACK];
    return chess;
}

static char *getChessPositionStr(char *chessStr, const ChessPosition *chess)
{
    static const char *colorStrs[] = {"RED", "BLACK", "ALLCOLOR"};
    char temp[1024];
    snprintf(temp, 32, "player: %s\n", colorStrs[chess->player]);
    strcpy(chessStr, temp);
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

wchar_t *getChessPositionBoardStr(wchar_t *boardStr, const ChessPosition *chess)
{
    static const wchar_t *BoardStr = L"┏━┯━┯━┯━┯━┯━┯━┯━┓\n"
                                     "┃　│　│　│╲│╱│　│　│　┃\n"
                                     "┠─┼─┼─┼─╳─┼─┼─┼─┨\n"
                                     "┃　│　│　│╱│╲│　│　│　┃\n"
                                     "┠─╬─┼─┼─┼─┼─┼─╬─┨\n"
                                     "┃　│　│　│　│　│　│　│　┃\n"
                                     "┠─┼─╬─┼─╬─┼─╬─┼─┨\n"
                                     "┃　│　│　│　│　│　│　│　┃\n"
                                     "┠─┴─┴─┴─┴─┴─┴─┴─┨\n"
                                     "┃　　　　　　　　　　　　　　　┃\n"
                                     "┠─┬─┬─┬─┬─┬─┬─┬─┨\n"
                                     "┃　│　│　│　│　│　│　│　┃\n"
                                     "┠─┼─╬─┼─╬─┼─╬─┼─┨\n"
                                     "┃　│　│　│　│　│　│　│　┃\n"
                                     "┠─╬─┼─┼─┼─┼─┼─╬─┨\n"
                                     "┃　│　│　│╲│╱│　│　│　┃\n"
                                     "┠─┼─┼─┼─╳─┼─┼─┼─┨\n"
                                     "┃　│　│　│╱│╲│　│　│　┃\n"
                                     "┗━┷━┷━┷━┷━┷━┷━┷━┛\n"; // 边框粗线
                                     
    wcscpy(boardStr, BoardStr);
    for (Color color = RED; color <= BLACK; ++color)
    {
        int (*getNonZeroIndex)(Board) = color == BLACK ? getHighNonZeroIndex : getLowNonZeroIndex;
        for (Kind kind = KING; kind <= PAWN; ++kind)
        {
            wchar_t name = getPrintName(color, kind);
            Board board = chess->pieces[color][kind];
            while (board)
            {
                int index = getNonZeroIndex(board);
                // int index = getLowNonZeroIndex(board);
                // int index = getHighNonZeroIndex(board);
                Seat seat = Seats[index];
                boardStr[(BOARDROWNUM - seat.row - 1) * 2 * (BOARDCOLNUM * 2) + seat.col * 2] = name;

                // pieChars[index] = ch;
                board ^= BoardMask[index];
            }
        }
    }

    return boardStr;
}

void testChessPosition()
{

    const char *fens[] = {
        FEN,
        "5a3/4ak2r/6R2/8p/9/9/9/B4N2B/4K4/3c5",
        "2b1kab2/4a4/4c4/9/9/3R5/9/1C7/4r4/2BK2B2",
        "4kab2/4a4/4b4/3N5/9/4N4/4n4/4B4/4A4/3AK1B2",
    };

    for (int i = 0; i < 4; ++i)
    {
        const char *afen = fens[i];
        char fen[BOARDLENGTH],
            pieChars[BOARDLENGTH + 1];
        setPieCharsFromFen(pieChars, afen);
        setFenFromPieChars(fen, pieChars);
        // printf("testFenPieChars:\npieChars: %s\nafen: %s\n fen: %s\n fen.Equal: %d\n\n",
        //        pieChars, afen, fen, strcmp(fen, afen));

        char chessStr[4 * 4096],
            setFen[BOARDLENGTH];
        ChessPosition chess = {};
        setChessPositionFromFen(&chess, afen);

        getChessPositionStr(chessStr, &chess);
        setFenFromChessPosition(setFen, &chess);

        // printf("testChessPosition:\n%s  afen: %s\nsetFen: %s\nsetFen.Equal: %d\n\n",
        //        chessStr, afen, setFen, strcmp(setFen, afen));

        wchar_t chessBoardStr[4096];
        getChessPositionBoardStr(chessBoardStr, &chess);
        // wprintf(L"chessPositionBoardStr:\n%ls\n", chessBoardStr);
        wprintf(L"chessPositionBoardStr:中国人d\n");
    }
}
