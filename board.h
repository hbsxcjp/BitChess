#include <stdint.h>

#define PLAYERNUM 2
#define PIECEKINDNUM 7
#define ALLPIECEKINDNUM (PLAYERNUM * PIECEKINDNUM)

#define BOARDROWNUM 10
#define BOARDCOLNUM 9
#define BOARDLENGTH (BOARDROWNUM * BOARDCOLNUM)

typedef __uint128_t Board;

// typedef enum Player
// {
//     RED,
//     BLACK
// } Player;

// typedef enum Kind
// {
//     KING,
//     ADVISOR,
//     BISHOP,
//     KNIGHT,
//     ROOK,
//     CANON,
//     PAWN,
//     ALLPIECE
// } Kind;

// typedef struct PlayerKind
// {
//     Player player;
//     Kind kind;
// } PlayerKind;

typedef struct ChessPosition
{
    // 当前走器
    // Player curPlayer;
    int player;

    // 基本局面
    Board pieces[ALLPIECEKINDNUM];

    // 计算中间存储数据(基本局面改动时更新)

} ChessPosition;

// extern const Board* BoardMask;

// extern const char *FEN;

ChessPosition *setChessPosition(ChessPosition *chess, const char *fen);

void initMask();

void testBoardMask();

void testFenPieChars();

void testChessPosition();