#include "board.h"
#include "move.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
    Board colorPieces[COLORNUM];

} ChessPosition;

const char NullChar = '_';

const char SplitChar = '/';

const char EndChar = '\x0';

const int PieceNum[] = {1, 2, 2, 2, 2, 2, 5};

const char *Chars[] = {"KABNRCP", "kabnrcp"};

const char FEN[] = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";

const Board BoardMask[BOARDLENGTH] = {
    (Board)1 << (127 - 0),
    (Board)1 << (127 - 1),
    (Board)1 << (127 - 2),
    (Board)1 << (127 - 3),
    (Board)1 << (127 - 4),
    (Board)1 << (127 - 5),
    (Board)1 << (127 - 6),
    (Board)1 << (127 - 7),
    (Board)1 << (127 - 8),
    (Board)1 << (127 - 9),
    (Board)1 << (127 - 10),
    (Board)1 << (127 - 11),
    (Board)1 << (127 - 12),
    (Board)1 << (127 - 13),
    (Board)1 << (127 - 14),
    (Board)1 << (127 - 15),
    (Board)1 << (127 - 16),
    (Board)1 << (127 - 17),
    (Board)1 << (127 - 18),
    (Board)1 << (127 - 19),
    (Board)1 << (127 - 20),
    (Board)1 << (127 - 21),
    (Board)1 << (127 - 22),
    (Board)1 << (127 - 23),
    (Board)1 << (127 - 24),
    (Board)1 << (127 - 25),
    (Board)1 << (127 - 26),
    (Board)1 << (127 - 27),
    (Board)1 << (127 - 28),
    (Board)1 << (127 - 29),
    (Board)1 << (127 - 30),
    (Board)1 << (127 - 31),
    (Board)1 << (127 - 32),
    (Board)1 << (127 - 33),
    (Board)1 << (127 - 34),
    (Board)1 << (127 - 35),
    (Board)1 << (127 - 36),
    (Board)1 << (127 - 37),
    (Board)1 << (127 - 38),
    (Board)1 << (127 - 39),
    (Board)1 << (127 - 40),
    (Board)1 << (127 - 41),
    (Board)1 << (127 - 42),
    (Board)1 << (127 - 43),
    (Board)1 << (127 - 44),
    (Board)1 << (127 - 45),
    (Board)1 << (127 - 46),
    (Board)1 << (127 - 47),
    (Board)1 << (127 - 48),
    (Board)1 << (127 - 49),
    (Board)1 << (127 - 50),
    (Board)1 << (127 - 51),
    (Board)1 << (127 - 52),
    (Board)1 << (127 - 53),
    (Board)1 << (127 - 54),
    (Board)1 << (127 - 55),
    (Board)1 << (127 - 56),
    (Board)1 << (127 - 57),
    (Board)1 << (127 - 58),
    (Board)1 << (127 - 59),
    (Board)1 << (127 - 60),
    (Board)1 << (127 - 61),
    (Board)1 << (127 - 62),
    (Board)1 << (127 - 63),
    (Board)1 << (127 - 64),
    (Board)1 << (127 - 65),
    (Board)1 << (127 - 66),
    (Board)1 << (127 - 67),
    (Board)1 << (127 - 68),
    (Board)1 << (127 - 69),
    (Board)1 << (127 - 70),
    (Board)1 << (127 - 71),
    (Board)1 << (127 - 72),
    (Board)1 << (127 - 73),
    (Board)1 << (127 - 74),
    (Board)1 << (127 - 75),
    (Board)1 << (127 - 76),
    (Board)1 << (127 - 77),
    (Board)1 << (127 - 78),
    (Board)1 << (127 - 79),
    (Board)1 << (127 - 80),
    (Board)1 << (127 - 81),
    (Board)1 << (127 - 82),
    (Board)1 << (127 - 83),
    (Board)1 << (127 - 84),
    (Board)1 << (127 - 85),
    (Board)1 << (127 - 86),
    (Board)1 << (127 - 87),
    (Board)1 << (127 - 88),
    (Board)1 << (127 - 89),
};

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

// 求最低位非零位的序号，调用前判断参数非全零位
static int getFrontNonZeroIndex(Board board)
{
    int index = BOARDBITSIZE - 1;
    uint64_t value = board >> 64; // 00-63 位
    if (value)
        index -= 64;
    else
        value = board & 0XFFFFFFC000000000UL; // 64-89 位

    if (value & 0XFFFFFFFF00000000UL)
    {
        index -= 32;
        value &= 0XFFFFFFFF00000000UL;
    }

    if (value & 0XFFFF0000FFFF0000UL)
    {
        index -= 16;
        value &= 0XFFFF0000FFFF0000UL;
    }

    if (value & 0XFF00FF00FF00FF00UL)
    {
        index -= 8;
        value &= 0XFF00FF00FF00FF00UL;
    }

    if (value & 0XF0F0F0F0F0F0F0F0UL)
    {
        index -= 4;
        value &= 0XF0F0F0F0F0F0F0F0UL;
    }

    if (value & 0XCCCCCCCCCCCCCCCCUL)
    {
        index -= 2;
        value &= 0XCCCCCCCCCCCCCCCCUL;
    }

    if (value & 0XAAAAAAAAAAAAAAAAUL)
        index -= 1;

    return index;
}

// 求最高位非零位的序号，调用前判断参数非全零位
static int getLastNonZeroIndex(Board board)
{
    int index = 0;
    uint64_t value = board & 0XFFFFFFC000000000UL; // 64-89 位
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

    // 设置分颜色棋盘
    for (Color color = RED; color <= BLACK; ++color)
        for (Kind kind = KING; kind <= PAWN; ++kind)
            chess->colorPieces[color] |= chess->pieces[color][kind];

    return chess;
}

char *setFenFromChessPosition(char *fen, const ChessPosition *chess)
{
    char pieChars[BOARDLENGTH + 1];
    for (int i = 0; i < BOARDLENGTH; ++i)
        pieChars[i] = NullChar;

    for (Color color = RED; color <= BLACK; ++color)
    {
        int (*getNonZeroIndex)(Board) = color == RED ? getLastNonZeroIndex : getFrontNonZeroIndex;
        for (Kind kind = KING; kind <= PAWN; ++kind)
        {
            char ch = getPieceChar((ColorKind){color, kind});
            Board board = chess->pieces[color][kind];
            while (board)
            {
                int index = getNonZeroIndex(board);
                // int index = getFrontNonZeroIndex(board);
                // int index = getLastNonZeroIndex(board);
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

    return chess;
}

char *getBoardStr(char *boardStr, const Board *boards, int length, int colNum)
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
            snprintf(temp, 16, "%02d(%d,%d):  ", index + col, (index + col) / colNum, col);
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
        getBoardStr(boardStr, chess->pieces[color], color == ALLCOLOR ? COLORNUM : KINDNUM, KINDNUM);
        strcat(chessStr, boardStr);
    }

    return chessStr;
}

// 打印初始化器的内容
void initBoardMaskStr()
{
    char maskStr[BOARDLENGTH * 64];
    strcpy(maskStr, "const Board BoardMask[BOARDLENGTH] = {\n");
    for (int i = 0; i < BOARDLENGTH; ++i)
    {
        char oneStr[64];
        snprintf(oneStr, 64, "BOARDAT(%d),\n", i);
        strcat(maskStr, oneStr);
    }

    strcat(maskStr, "};\n");
    printf(maskStr);
}

void printBoardMask()
{
    char boardStr[BOARDLENGTH * (BOARDROWNUM + 2) * 16];
    getBoardStr(boardStr, BoardMask, BOARDLENGTH, BOARDCOLNUM);
    printf("testBoardMask:\n%s\n", boardStr);
}

void testBoardMask()
{
    // initBoardMaskStr();

    printBoardMask();
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