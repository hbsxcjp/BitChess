#include "board.h"
#include "data.h"
#include "move.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

// #define DEBUGTESTBOARD

void traverseColorKindPieces(Board* board, Color color, Kind kind, GetIndexFunc getIndexFunc, BitBoard bitBoard,
    void func(Board* board, Color color, Kind kind, int index, void* arg1, void* arg2),
    void* arg1, void* arg2)
{
    while (bitBoard) {
        int index = getIndexFunc(bitBoard);
        // 执行针对遍历元素的操作函数
        func(board, color, kind, index, arg1, arg2);

        bitBoard ^= BoardMask[index];
    }
}

void traverseColorPieces(Board* board, Color color,
    void func(Board* board, Color color, Kind kind, int index, void* arg1, void* arg2),
    void* arg1, void* arg2)
{
    GetIndexFunc getIndexFunc = getNonZeroIndex(board, color);
    for (Kind kind = KING; kind <= PAWN; ++kind)
        traverseColorKindPieces(board, color, kind, getIndexFunc, board->pieces[color][kind],
            func, arg1, arg2);
}

void traverseAllColorPieces(Board* board,
    void func(Board* board, Color color, Kind kind, int index, void* arg1, void* arg2),
    void* arg1, void* arg2)
{
    for (Color color = RED; color <= BLACK; ++color)
        traverseColorPieces(board, color, func, arg1, arg2);
}

static inline Color getColor(char ch)
{
    return isupper(ch) ? RED : BLACK;
}

static inline Kind getKind(char ch)
{
    Color color = getColor(ch);
    return strchr(Chars[color], ch) - Chars[color];
}

static inline wchar_t getPrintName(Color color, Kind kind)
{
    return (color == BLACK && kind > BISHOP && kind < PAWN) ? L"馬車砲"[kind - KNIGHT] : Names[color][kind];
}

char* getPieCharsFromFen(char* pieChars, const char* fen)
{
    int len = strlen(fen);
    for (int index = 0, fenIndex = 0; fenIndex < len && index < BOARDLENGTH; ++fenIndex) {
        char ch = fen[fenIndex];
        if (isdigit(ch))
            for (int j = ch - '0'; j > 0; --j)
                pieChars[index++] = NullChar;
        else if (isalpha(ch))
            pieChars[index++] = ch;
    }

    pieChars[BOARDLENGTH] = EndChar;
    return pieChars;
}

char* getFenFromPieChars(char* fen, const char* pieChars)
{
    int index_F = 0;
    for (int row = 0; row < BOARDROWNUM; ++row) { // 从最高行开始
        int blankNum = 0;
        for (int col = 0; col < BOARDCOLNUM; ++col) {
            int index_p = row * BOARDCOLNUM + col;
            if (isalpha(pieChars[index_p])) {
                if (blankNum > 0)
                    fen[index_F++] = '0' + blankNum;

                fen[index_F++] = pieChars[index_p];
                blankNum = 0;
            } else if (pieChars[index_p] == NullChar)
                blankNum++;
        }
        if (blankNum > 0)
            fen[index_F++] = '0' + blankNum;

        fen[index_F++] = SplitChar; // 插入行分隔符
    }

    fen[--index_F] = EndChar;
    return fen;
}

Board* getBoardFromFen(Board* board, const char* fen)
{
    char pieChars[BOARDLENGTH + 1] = {};
    getPieCharsFromFen(pieChars, fen);

    char ch;
    for (int index = 0; index < BOARDLENGTH; ++index) {
        if ((ch = pieChars[index]) != NullChar) {
            Color color = getColor(ch);
            Kind kind = getKind(ch);
            BitBoard turnBoard = BoardMask[index],
                     rotateTurnBoard = BoardMask[Rotate[index]];

            board->pieces[color][kind] ^= turnBoard;
            board->colorPieces[color] ^= turnBoard;

            board->allPieces ^= turnBoard;
            board->rotatePieces ^= rotateTurnBoard;
        }
    }

    board->bottomColor = getNonZeroIndex(board, RED)(board->pieces[RED][KING]) > (BOARDLENGTH >> 1) ? RED : BLACK;
    return board;
}

static void setPieChars(Board* board, Color color, Kind kind, int index, void* pieChars, void* arg2)
{
    ((char*)pieChars)[index] = Chars[color][kind];
}

char* getFenFromBoard(char* fen, Board* board)
{
    char pieChars[BOARDLENGTH + 1];
    for (int i = 0; i < BOARDLENGTH; ++i)
        pieChars[i] = NullChar;

    traverseAllColorPieces(board, setPieChars, pieChars, NULL);
    pieChars[BOARDLENGTH] = EndChar;

    return getFenFromPieChars(fen, pieChars);
}

static void setZobrist(Board* board, Color color, Kind kind, int index, void* zobrist, void* arg2)
{
    *(U64*)zobrist ^= Zobrist[color][kind][index];
}

U64 getZobristFromBoard(Board* board)
{
    U64 zobrist = 0;
    if (board->player == BLACK)
        zobrist ^= ZobristBlack;

    traverseAllColorPieces(board, setZobrist, &zobrist, NULL);
    return zobrist;
}

bool isEqual(Board aboard, Board bboard)
{
    if (aboard.player != bboard.player)
        return false;

    for (Color color = RED; color <= BLACK; color++) {
        for (Kind kind = KING; kind <= PAWN; kind++)
            if (aboard.pieces[color][kind] != bboard.pieces[color][kind])
                return false;
    }

    if (aboard.bottomColor != bboard.bottomColor)
        return false;

    for (Color color = RED; color <= BLACK; color++) {
        if (aboard.colorPieces[color] != bboard.colorPieces[color])
            return false;
    }

    return true;
}

static void setBoardChars(Board* board, Color color, Kind kind, int index, void* boardStr, void* arg2)
{
    Coord coord = Coords[index];
    ((char*)boardStr)[coord.row * (BOARDCOLNUM + 1) + coord.col] = Chars[color][kind];
}

char* getBoardStr(char* boardStr, Board* board)
{
    static const char* BoardStr = "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n";

    const char* colorStrs[] = { "RED", "BLACK" };
    char temp[KINDNUM * (BOARDROWNUM + 2) * 16];
    snprintf(temp, 128, "player: %s\n", colorStrs[board->player]);
    strcpy(boardStr, temp);
    for (Color color = RED; color <= BLACK; ++color) {
        snprintf(temp, 32, "board->pieces[%s][Kind]:\n", colorStrs[!color]);
        strcat(boardStr, temp);

        getBitBoardArrayStr(temp, board->pieces[!color], KINDNUM, KINDNUM, true, false);
        strcat(boardStr, temp);
    }

    snprintf(temp, 128, "colorPieces[Color]: \nRED:         BLACK:\n");
    strcat(boardStr, temp);
    getBitBoardArrayStr(temp, board->colorPieces, COLORNUM, KINDNUM, true, false);
    strcat(boardStr, temp);

    strcat(boardStr, "allPieces: \n");
    getBitBoardArrayStr(temp, &board->allPieces, 1, KINDNUM, true, false);
    strcat(boardStr, temp);

    strcat(boardStr, "rotatePieces: \n");
    getBitBoardArrayStr(temp, &board->rotatePieces, 1, KINDNUM, true, true);
    strcat(boardStr, temp);

    strcat(boardStr, "chessBoardStr:\n");
    strcpy(temp, BoardStr);
    traverseAllColorPieces(board, setBoardChars, temp, NULL);
    strcat(boardStr, temp);

    return boardStr;
}

static void setBoardNames(Board* board, Color color, Kind kind, int index, void* boardWStr, void* arg2)
{
    Coord coord = Coords[index];
    ((wchar_t*)boardWStr)[coord.row * (BOARDCOLNUM + 1) + coord.col] = Names[color][kind];
}

wchar_t* getBoardWStr(wchar_t* boardWStr, Board* board)
{
    static const wchar_t* BoardStr = L"－－－－－－－－－\n"
                                     "－－－－－－－－－\n"
                                     "－－－－－－－－－\n"
                                     "－－－－－－－－－\n"
                                     "－－－－－－－－－\n"
                                     "－－－－－－－－－\n"
                                     "－－－－－－－－－\n"
                                     "－－－－－－－－－\n"
                                     "－－－－－－－－－\n"
                                     "－－－－－－－－－\n";

    wchar_t temp[KINDNUM * (BOARDROWNUM + 2) * 16];
    wcscpy(boardWStr, L"chessBoardWStr:\n");
    wcscpy(temp, BoardStr);
    traverseAllColorPieces(board, setBoardNames, temp, NULL);
    wcscat(boardWStr, temp);

    return boardWStr;
}

void testBoard()
{

    const char* fens[] = {
        FEN,
        "5a3/4ak2r/6R2/8p/9/9/9/B4N2B/4K4/3c5",
        "2b1kab2/4a4/4c4/9/9/3R5/9/1C7/4r4/2BK2B2",
        "4kab2/4a4/4b4/3N5/9/4N4/4n4/4B4/4A4/3AK1B2",
    };

    for (int i = 0; i < 4; ++i) {
        const char* afen = fens[i];
        char fen[BOARDLENGTH],
            pieChars[BOARDLENGTH + 1];
        getPieCharsFromFen(pieChars, afen);
        getFenFromPieChars(fen, pieChars);

#ifdef DEBUGTESTBOARD
        printf("testFenPieChars:\npieChars: %s\nafen: %s\n fen: %s\n fen.Equal: %d\n\n",
            pieChars, afen, fen, strcmp(fen, afen));
#endif
        assert(strcmp(afen, fen) == 0);

        char boardStr[4 * 4096],
            setFen[BOARDLENGTH];
        Board board = {};
        getBoardFromFen(&board, afen);
        getBoardStr(boardStr, &board);
        getFenFromBoard(setFen, &board);

#ifdef DEBUGTESTBOARD
        printf("testBoard:\n%s  afen: %s\nsetFen: %s\nsetFen.Equal: %d\n",
            boardStr, afen, setFen, strcmp(setFen, afen));
#endif
        assert(strcmp(afen, setFen) == 0);

#ifdef DEBUGTESTBOARD
        printf("getBoardCanMove:\n");
#endif
        BitMove bitMoves[MOVEBOARDMAXCOUNT];
        Color colors[COLORNUM] = { RED, BLACK };
        for (int i = 0; i < COLORNUM; ++i) {
            int count = getBoardCanMove(bitMoves, &board, colors[i]);
            getMoveArrayStr(boardStr, bitMoves, count, KINDNUM + 1);
#ifdef DEBUGTESTBOARD
            printf("%smoveCount:%2d\n", boardStr, count);
#endif
        }

        wchar_t boardWStr[1024];
        getBoardWStr(boardWStr, &board);
#ifdef DEBUGTESTBOARD
        wprintf(L"%ls\n", boardWStr);
#endif

#ifdef DEBUGTESTBOARD
        U64 zobrist = getZobristFromBoard(&board);
        printf("getZobristFromBoard: %016lX\n", zobrist);
#endif
    }
}
