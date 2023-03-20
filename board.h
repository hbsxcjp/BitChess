#include <stdint.h>

#define COLORNUM 2
#define KINDNUM 7

#define BOARDROWNUM 10
#define BOARDCOLNUM 9
#define BOARDLENGTH (BOARDROWNUM * BOARDCOLNUM)

typedef __uint128_t Board;

typedef enum Color
{
    RED,
    BLACK
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
    // 当前走器
    // Color curPlayer;
    Color player;

    // 基本局面
    Board pieces[COLORNUM][KINDNUM];

    // 计算中间存储数据(基本局面改动时更新)

} ChessPosition;

// extern const Board* BoardMask;

// extern const char *FEN;

ChessPosition *setChessPosition(ChessPosition *chess, const char *fen);

void initMask();

void testBoardMask();

void testFenPieChars();

void testChessPosition();