
#define PLAYERNUM 2
#define PIECEKINDNUM 7

#define BOARDROWNUM 10
#define BOARDCOLNUM 9
#define BOARDLENGTH (BOARDROWNUM * BOARDCOLNUM)

typedef __int128_t BitBoard;

typedef enum Player
{
    RED,
    BLACK
} Player;

typedef enum PieceKind
{
    KING,
    ADVISOR,
    BISHOP,
    KNIGHT,
    ROOK,
    CANON,
    PAWN,
    ALLPIECE
} PieceKind;

typedef struct PlayerPieceKind
{
    Player player;
    PieceKind kind;
} PlayerPieceKind;

typedef struct ChessPosition
{
    // 当前走器
    Player curPlayer;

    BitBoard pieces[PLAYERNUM * PIECEKINDNUM];

} ChessPosition;

extern const char *FEN;

ChessPosition *setChessPosition(ChessPosition *chess, const char *fen);

void initMask();

void testMask();

void testFenPieChars();

void printChessPosition(const ChessPosition *chess);