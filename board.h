
#define BOARDBITSIZE 128
#define BOARDOTHERSIZE (BOARDBITSIZE - BOARDLENGTH)

#define COLORNUM 2
#define KINDNUM 7

#define BOARDROWNUM 10
#define BOARDCOLNUM 9
#define BOARDLENGTH (BOARDROWNUM * BOARDCOLNUM)

typedef __uint128_t Board; // 只使用最高的90位

typedef enum Color Color;

typedef enum Kind Kind;

typedef struct ColorKind ColorKind;

typedef struct ChessPosition ChessPosition;

extern const Board BoardMask[BOARDLENGTH];

extern const char FEN[];

ChessPosition *updateChessPositionData(ChessPosition *chess);

ChessPosition *setChessPositionFromFen(ChessPosition *chess, const char *fen);

char *setFenFromChessPosition(char *fen, const ChessPosition *chess);

void testBoardMask();

void testFenPieChars();

void testChessPosition();