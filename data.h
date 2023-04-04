#ifndef DATA_H
#define DATA_H

#include <stdbool.h>

#define COLORNUM 2
#define KINDNUM 7

#define BOARDROWNUM 10
#define BOARDCOLNUM 9
#define BOARDLENGTH (BOARDROWNUM * BOARDCOLNUM)
#define MOVEBOARDMAXCOUNT 256

#define BOARDBITSIZE 128
#define BOARDOTHERSIZE (BOARDBITSIZE - BOARDLENGTH)
#define BOARDAT(i) ((Board)1 << (i))
#define BOARDFROM(i, offset) ((Board)(i) << (offset))

#define INTBITAT(i) (1 << (i))
#define INTBITHAS(state, index) ((state)&INTBITAT(index))

#define LEGCOUNT 4
#define ROWSTATEMAX (1 << BOARDCOLNUM)
#define COLSTATEMAX (1 << BOARDROWNUM)
#define ROWBASEOFFSET(row) ((row)*BOARDCOLNUM)
#define COLBASEOFFSET(col) ((col)*BOARDROWNUM)
// #define INDEXFROMROWCOL(row, col) ((row)*BOARDCOLNUM + (col))

#define BINARYPATTERN9 "%c%c%c%c%c%c%c%c%c "
#define BINARYPATTERN10 ("%c" BINARYPATTERN9)

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
#define BYTEBINARY10(i) \
    BYTEBINARY9(i),     \
        (((i)&0x200) ? '1' : '-')

typedef __uint128_t Board; // 只使用最低的90位

typedef enum Color {
    RED,
    BLACK,
    ALLCOLOR,
    ROTATE
} Color;

typedef enum Kind {
    KING,
    ADVISOR,
    BISHOP,
    KNIGHT,
    ROOK,
    CANNON,
    PAWN
} Kind;

// typedef struct Piece {
//     Color color;
//     Kind kind;
// } Piece;

typedef struct Seat {
    int row;
    int col;
} Seat;

typedef struct ChessPosition {
    // 当前
    Color player;

    // 基本局面
    Board pieces[COLORNUM][KINDNUM];

    // 计算中间存储数据(基本局面改动时更新)
    Color bottomColor;
    Board calPieces[ROTATE + 1];
} ChessPosition;

typedef struct MoveBoard {
    Color color;
    Kind kind;
    int fromIndex;

    Board moveTo;
} MoveBoard;

extern Seat Seats[BOARDLENGTH];
extern int Rotate[BOARDLENGTH];

extern Board BoardMask[BOARDLENGTH];

extern Board KingMove[BOARDLENGTH];
extern Board AdvisorMove[BOARDLENGTH];
extern Board PawnMove[BOARDLENGTH][2];

// 获取非0位的索引位置
unsigned int getLowNonZeroIndexFromUInt(unsigned int value);

unsigned int getLowNonZeroIndexFromRowCol(unsigned int value);

int getLowNonZeroIndexs(int indexs[], int value);

int getLowNonZeroIndex(Board board);

int getHighNonZeroIndex(Board board);

// 获取某种棋子在当前状态某个位置可移动的位棋盘
// Board getKingMove(int fromIndex);return KingMove[fromIndex];

// Board getAdvisorMove(int fromIndex);return AdvisorMove[fromIndex];

Board getBishopMove(int fromIndex, Board allPieces);

Board getKnightMove(int fromIndex, Board allPieces);

Board getRookMove(int fromIndex, Board allPieces, Board rotatePieces);

Board getCannonMove(int fromIndex, Board allPieces, Board rotatePieces);

// Board getPawnMove(int fromIndex, bool isBottom);return PawnMove[fromIndex][isBottom];

// 打印显示位状态
char* getBitStr(char* bitStr, int value, bool isCol);

char* getBoardStr(char* boardStr, Board board);

char* getBoardArrayStr(char* boardStr, const Board* boards, int length, int colNum, bool showZero, bool isCol);

char* getMoveBoardsStr(char* moveStr, const MoveBoard* moveBoards, int count);

// 初始化预计算的数据
void initData();

#endif /* DATA_H */