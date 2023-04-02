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

typedef struct ColorKind {
    Color color;
    Kind kind;
} ColorKind;

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
    Board calPieces[ROTATE + 1];
} ChessPosition;

extern Seat Seats[BOARDLENGTH];
extern int Rotate[BOARDLENGTH];

extern Board BoardMask[BOARDLENGTH];
extern Board KingMove[BOARDLENGTH];
extern Board AdvisorMove[BOARDLENGTH];
extern Board PawnMove[BOARDLENGTH][2];

// Board getKingMove(int fromIndex);return KingMove[fromIndex];

// Board getAdvisorMove(int fromIndex);return AdvisorMove[fromIndex];

Board getBishopMove(int fromIndex, Board allPieces);

Board getKnightMove(int fromIndex, Board allPieces);

Board getRookCannonMove(bool isCannon, int fromIndex, Board allPieces, Board rotatePieces);

// Board getPawnMove(int fromIndex, bool isBottom);return PawnMove[fromIndex][isBottom];

char* getIntBitStr(char* bitStr, int value, bool isCol);

char* getBoardStr(char* boardStr, const Board* boards, int length, int colNum, bool showZero, bool isCol);

void initData();

#endif /* DATA_H */