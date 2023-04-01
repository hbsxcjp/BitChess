#ifndef DATA_H
#define DATA_H

#include <stdbool.h>

#define BOARDBITSIZE 128
#define BOARDOTHERSIZE (BOARDBITSIZE - BOARDLENGTH)
#define BOARDAT(i) ((Board)1 << (i))
#define BOARDFROM(i, offset) ((Board)(i) << (offset))

#define INTBITAT(i) (1 << (i))
#define INTBITHAS(state, index) ((state)&INTBITAT(index))

#define COLORNUM 2
#define KINDNUM 7

#define BOARDROWNUM 10
#define BOARDCOLNUM 9
#define BOARDLENGTH (BOARDROWNUM * BOARDCOLNUM)
#define INDEXFROMROWCOL(row, col) ((row)*BOARDCOLNUM + (col))

#define BINARYPATTERN9 "%c%c%c%c%c%c%c%c%c "
#define BINARYPATTERN10 "%c%c%c%c%c%c%c%c%c%c "

#define BYTEBINARY9(i)            \
    (((i)&0x01) ? '1' : '-'),     \
        (((i)&0x02) ? '1' : '-'), \
        (((i)&0x04) ? '1' : '-'), \
        (((i)&0x08) ? '1' : '-'), \
        (((i)&0x10) ? '1' : '-'), \
        (((i)&0x20) ? '1' : '-'), \
        (((i)&0x40) ? '1' : '-'), \
        (((i)&0x80) ? '1' : '-'), \
        (((i)&0x100) ? '1' : '-')

#define BYTEBINARY10(i)            \
    (((i)&0x01) ? '1' : '-'),      \
        (((i)&0x02) ? '1' : '-'),  \
        (((i)&0x04) ? '1' : '-'),  \
        (((i)&0x08) ? '1' : '-'),  \
        (((i)&0x10) ? '1' : '-'),  \
        (((i)&0x20) ? '1' : '-'),  \
        (((i)&0x40) ? '1' : '-'),  \
        (((i)&0x80) ? '1' : '-'),  \
        (((i)&0x100) ? '1' : '-'), \
        (((i)&0x200) ? '1' : '-')

typedef __uint128_t Board; // 只使用最低的90位

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
    CANNON,
    PAWN
} Kind;

typedef struct ColorKind
{
    Color color;
    Kind kind;
} ColorKind;

typedef struct Seat
{
    int row;
    int col;
} Seat;

typedef struct ChessPosition
{
    // 当前
    Color player;

    // 基本局面
    Board pieces[COLORNUM][KINDNUM];

    // 计算中间存储数据(基本局面改动时更新)
    Board calPieces[COLORNUM + 1];
    Board rotatePieces;
} ChessPosition;

extern Seat Seats[BOARDLENGTH];

extern int Rotate[BOARDLENGTH];

extern Board BoardMask[BOARDLENGTH];

extern Board PiecePut[KINDNUM];

extern Board PieceMove[KINDNUM][BOARDLENGTH];

bool isValidKing(int row, int col);

bool isValidAdvisor(int row, int col);

bool isValidBishop(int row, int col);

char *getRowColBit(char *bitStr, int value, bool isCol);

char *getBoardStr(char *boardStr, const Board *boards, int length, int colNum, bool showZero, bool isCol);

void initData();

void printData();

#endif /* DATA_H */