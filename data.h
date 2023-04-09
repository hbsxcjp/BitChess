#ifndef DATA_H
#define DATA_H

#include <stdbool.h>
#include <sys/types.h>
#include <wchar.h>

#define COLORNUM 2
#define KINDNUM 7

#define BOARDROWNUM 10
#define BOARDCOLNUM 9
#define BOARDLENGTH (BOARDROWNUM * BOARDCOLNUM)
#define MOVEBOARDMAXCOUNT 256

#define BOARDBITSIZE 128
#define BOARDOTHERSIZE (BOARDBITSIZE - BOARDLENGTH)
#define BOARDAT(i) ((BitBoard)1 << (i))
#define BOARDFROM(i, offset) ((BitBoard)(i) << (offset))

#define INTBITAT(i) (1 << (i))
#define INTBITHAS(state, index) ((state)&INTBITAT(index))

#define LEGCOUNT 4
#define ROWSTATEMAX (1 << BOARDCOLNUM)
#define COLSTATEMAX (1 << BOARDROWNUM)
#define ROWBASEOFFSET(row) ((row)*BOARDCOLNUM)
#define COLBASEOFFSET(col) ((col)*BOARDROWNUM)
#define INDEXFROMROWCOL(row, col) ((row)*BOARDCOLNUM + (col))

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

#define INFOSIZE 32
#define INFOFIELDNUM 2
#define REMARKSIZE 1024
#define MOVESTRSIZE (4 * 4096)

typedef __uint128_t BitBoard; // 只使用最低的90位

typedef __uint64_t U64; // Zobrist键值

typedef int (*GetIndexFunc)(BitBoard);

typedef enum Color {
    RED,
    BLACK,
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

// 棋盘变换类型
typedef enum {
    EXCHANGE,
    ROTATE,
    SYMMETRY
} ChangeType;

// 棋局存储类型
typedef enum RecFormat {
    XQF,
    PGN_ICCS,
    PGN_ZH,
    PGN_CC,
    BIN,
    JSON
} RecFormat;

typedef struct Coord {
    int row;
    int col;
} Coord;

typedef struct Board {
    // 基本局面
    Color player;
    BitBoard pieces[COLORNUM][KINDNUM];

    // 计算中间存储数据(基本局面改动时更新)
    Color bottomColor;
    BitBoard colorPieces[COLORNUM];
    BitBoard allPieces;
    BitBoard rotatePieces;
} Board;

typedef struct BitMove {
    Color color;
    Kind kind;
    int index;

    BitBoard moveTo;
} BitMove;

typedef struct Move {
    Color color;
    Kind kind;
    int fromIndex;
    int toIndex;

    struct Move* front;
    struct Move* next;
    struct Move* other;

    wchar_t* remark;
} Move;

typedef struct Instance {
    Board* board;
    Move* root;
    Move* current;

    int infoCount;
    wchar_t* info[INFOSIZE][INFOFIELDNUM];
} Instance;

extern const char Chars[COLORNUM][KINDNUM];
extern const wchar_t Names[COLORNUM][KINDNUM];
extern const wchar_t NumChar[COLORNUM][BOARDCOLNUM];
extern const wchar_t PreChar[];
extern const wchar_t MoveChar[];
extern const wchar_t IccsChar[];

extern const char NullChar;
extern const char SplitChar;
extern const char EndChar;
extern const char FEN[];

extern Coord Coords[BOARDLENGTH];
extern int Rotate[BOARDLENGTH];

extern BitBoard BoardMask[BOARDLENGTH];

extern BitBoard KingMove[BOARDLENGTH];
extern BitBoard AdvisorMove[BOARDLENGTH];
extern BitBoard PawnMove[BOARDLENGTH][2];

// 生成Zobrist键所需的随机值
extern U64 ZobristBlack;
extern U64 Zobrist[COLORNUM][KINDNUM][BOARDLENGTH];

int code_convert(char* from_charset, char* to_charset, char* inbuf, size_t inlen, char* outbuf, size_t outlen);

// 获取非0位的索引位置
// unsigned int getLowNonZeroIndexFromUInt(unsigned int value);

// unsigned int getLowNonZeroIndexFromRowCol(unsigned int value);

// int getLowNonZeroIndexs(int indexs[], int value);

int getLowNonZeroIndex(BitBoard bitBoard);

// int getHighNonZeroIndex(BitBoard bitBoard);

GetIndexFunc getNonZeroIndex(Board* board, Color color);

// 获取某种棋子在当前状态某个位置可移动的位棋盘
// BitBoard getKingMove(int fromIndex);return KingMove[fromIndex];

// BitBoard getAdvisorMove(int fromIndex);return AdvisorMove[fromIndex];

BitBoard getBishopMove(int fromIndex, BitBoard allPieces);

BitBoard getKnightMove(int fromIndex, BitBoard allPieces);

BitBoard getRookMove(int fromIndex, BitBoard allPieces, BitBoard rotatePieces);

BitBoard getCannonMove(int fromIndex, BitBoard allPieces, BitBoard rotatePieces);

// BitBoard getPawnMove(int fromIndex, bool isBottom);return PawnMove[fromIndex][isBottom];

char* getBitStr(char* bitStr, int value, bool isRotate);

void getBitBoardStr(char boardStr[][16], BitBoard bitBoard, bool isRotate);

char* getBitBoardArrayStr(char* boardArrayStr, const BitBoard* boards, int length, int colNum, bool showZero, bool isRotate);

// 初始化预计算的数据
void initData();

#endif /* DATA_H */