
#define BOARDBITSIZE 128
#define BOARDOTHERSIZE (BOARDBITSIZE - BOARDLENGTH)
#define BOARDAT(i) ((Board)1 << (BOARDBITSIZE - 1 - i))

#define COLORNUM 2
#define KINDNUM 7

#define BOARDROWNUM 10
#define BOARDCOLNUM 9
#define BOARDLENGTH (BOARDROWNUM * BOARDCOLNUM)

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

typedef __uint128_t Board; // 只使用最高的90位

typedef enum Color Color;

typedef enum Kind Kind;

typedef struct ColorKind ColorKind;

typedef struct ChessPosition ChessPosition;

extern const char FEN[];

extern const Board BoardMask[BOARDLENGTH];

ChessPosition *updateChessPositionData(ChessPosition *chess);

ChessPosition *setChessPositionFromFen(ChessPosition *chess, const char *fen);

char *setFenFromChessPosition(char *fen, const ChessPosition *chess);

char *getBoardStr(char *boardStr, const Board *boards, int length, int colNum);

void testBoardMask();

void testFenPieChars();

void testChessPosition();