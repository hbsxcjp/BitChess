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

typedef int (*GetIndexFunc)(Board);

typedef enum Color {
    RED,
    BLACK,
    ALLCOLOR,
    ROTATE,
    NONCOLOR
} Color;

typedef enum Kind {
    KING,
    ADVISOR,
    BISHOP,
    KNIGHT,
    ROOK,
    CANNON,
    PAWN,
    NONKIND
} Kind;

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
    Board calPieces[NONCOLOR];

} ChessPosition;

typedef struct Move {
    Color color;
    Kind kind;
    int index;

    Board moveTo;
} Move;

extern const char Chars[COLORNUM][KINDNUM];

extern Seat Seats[BOARDLENGTH];
extern int Rotate[BOARDLENGTH];

extern Board BoardMask[BOARDLENGTH];

extern Board KingMove[BOARDLENGTH];
extern Board AdvisorMove[BOARDLENGTH];
extern Board PawnMove[BOARDLENGTH][2];

// 获取非0位的索引位置
// unsigned int getLowNonZeroIndexFromUInt(unsigned int value);

// unsigned int getLowNonZeroIndexFromRowCol(unsigned int value);

// int getLowNonZeroIndexs(int indexs[], int value);

// int getLowNonZeroIndex(Board board);

// int getHighNonZeroIndex(Board board);

GetIndexFunc getNonZeroIndex(ChessPosition* chess, Color color);

// 获取某种棋子在当前状态某个位置可移动的位棋盘
// Board getKingMove(int fromIndex);return KingMove[fromIndex];

// Board getAdvisorMove(int fromIndex);return AdvisorMove[fromIndex];

Board getBishopMove(int fromIndex, Board allPieces);

Board getKnightMove(int fromIndex, Board allPieces);

Board getRookMove(int fromIndex, Board allPieces, Board rotatePieces);

Board getCannonMove(int fromIndex, Board allPieces, Board rotatePieces);

// Board getPawnMove(int fromIndex, bool isBottom);return PawnMove[fromIndex][isBottom];

// 清除原位置，置位新位置
void turnColorKindPieces(ChessPosition* chess, Color color, Kind kind, Board turnBoard, Board rotateTurnBoard);

void traverseColorPieces(ChessPosition* chess, Color color,
    void func(ChessPosition* chess, Color color, Kind kind, int index, void* arg1, void* arg2),
    void* arg1, void* arg2);

bool isEqual(ChessPosition achess, ChessPosition bchess);

char* getBoardArrayStr(char* boardArrayStr, const Board* boards, int length, int colNum, bool showZero, bool isRotate);

char* getMoveArrayStr(char* moveArrayStr, const Move* moves, int length, int colNum);

char* getChessPositionStr(char* chessStr, ChessPosition* chess);

// 初始化预计算的数据
void initData();

#endif /* DATA_H */