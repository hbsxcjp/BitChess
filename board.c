#include "board.h"
#include "data.h"
#include "move.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

static const char NullChar = '_';

static const char SplitChar = '/';

static const char EndChar = '\x0';

static const char Chars[COLORNUM][KINDNUM] = { "KABNRCP", "kabnrcp" };

static const char FEN[] = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";

static const wchar_t Names[COLORNUM][KINDNUM] = { L"帅仕相马车炮兵", L"将士象马车炮卒" };

// static const wchar_t NumChar[COLORNUM][BOARDCOLNUM] = { L"一二三四五六七八九", L"１２３４５６７８９" };

// static const wchar_t PreChar[] = L"前中后";

// static const wchar_t MoveChar[] = L"退平进";

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

static char* getPieCharsFromFen(char* pieChars, const char* fen)
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

static char* getFenFromPieChars(char* fen, const char* pieChars)
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

ChessPosition* updateAllPieces(ChessPosition* chess)
{
    Board rotatePieces = 0,
          allPieces = chess->calPieces[RED] | chess->calPieces[BLACK];

    chess->calPieces[ALLCOLOR] = allPieces;
    // 更新旋转位棋盘
    for (int index = 0; index < BOARDLENGTH; ++index) {
        if (allPieces & BoardMask[index])
            rotatePieces |= BoardMask[Rotate[index]];
    }

    chess->calPieces[ROTATE] = rotatePieces;
    return chess;
}

ChessPosition* getChessPositionFromFen(ChessPosition* chess, const char* fen)
{
    char pieChars[BOARDLENGTH + 1] = {};
    getPieCharsFromFen(pieChars, fen);

    for (int index = 0; index < BOARDLENGTH; ++index) {
        char ch;
        if ((ch = pieChars[index]) != NullChar) {
            chess->pieces[getColor(ch)][getKind(ch)] |= BoardMask[index];
        }
    }

    chess->bottomColor = getLowNonZeroIndex(chess->pieces[RED][KING]) > (BOARDLENGTH >> 1) ? RED : BLACK;
    // 设置分颜色棋盘
    for (Color color = RED; color <= BLACK; ++color)
        for (Kind kind = KING; kind <= PAWN; ++kind)
            chess->calPieces[color] |= chess->pieces[color][kind];

    return updateAllPieces(chess);
}

static void traverseChessPieces(const ChessPosition* chess, Color color,
    void func(const ChessPosition* chess, Color color, Kind kind, int index, void* arg1, void* arg2),
    void* arg1, void* arg2)
{
    int (*getNonZeroIndex)(Board) = color == chess->bottomColor ? getHighNonZeroIndex : getLowNonZeroIndex;
    for (Kind kind = KING; kind <= PAWN; ++kind) {
        Board board = chess->pieces[color][kind];
        while (board) {
            int index = getNonZeroIndex(board);
            // 执行针对遍历元素的操作函数
            func(chess, color, kind, index, arg1, arg2);

            board ^= BoardMask[index];
        }
    }
}

static void setPieChars(const ChessPosition* chess, Color color, Kind kind, int index, void* pieChars, void* arg2)
{
    ((char*)pieChars)[index] = Chars[color][kind];
}

char* getFenFromChessPosition(char* fen, const ChessPosition* chess)
{
    char pieChars[BOARDLENGTH + 1];
    for (int i = 0; i < BOARDLENGTH; ++i)
        pieChars[i] = NullChar;

    Color colors[COLORNUM] = { RED, BLACK };
    for (int i = 0; i < COLORNUM; ++i)
        traverseChessPieces(chess, colors[i], setPieChars, pieChars, NULL);

    pieChars[BOARDLENGTH] = EndChar;
    return getFenFromPieChars(fen, pieChars);
}

static Board getMoveTo(const ChessPosition* chess, Color color, Kind kind, int index)
{
    Board board;
    switch (kind) {
    case KING:
        board = KingMove[index];
        break;
    case ADVISOR:
        board = AdvisorMove[index];
        break;
    case BISHOP:
        board = getBishopMove(index, chess->calPieces[ALLCOLOR]);
        break;
    case KNIGHT:
        board = getKnightMove(index, chess->calPieces[ALLCOLOR]);
        break;
    case ROOK:
        board = getRookMove(index, chess->calPieces[ALLCOLOR], chess->calPieces[ROTATE]);
        break;
    case CANNON:
        board = getCannonMove(index, chess->calPieces[ALLCOLOR], chess->calPieces[ROTATE]);
        break;
    default:
        board = PawnMove[index][chess->bottomColor == color];
        break;
    }

    // 去掉同色棋子
    return board ^ (board & chess->calPieces[color]);
}

static void getMoveBoards(const ChessPosition* chess, Color color, Kind kind, int index, void* moveBoards, void* count)
{
    ((MoveBoard*)moveBoards)[(*(int*)count)++] = (MoveBoard) { color, kind, index, getMoveTo(chess, color, kind, index) };
}

int getChessPositionMoveBoards(MoveBoard moveBoards[], const ChessPosition* chess, Color color)
{
    int count = 0;
    traverseChessPieces(chess, color, getMoveBoards, moveBoards, &count);

    return count;
}

static void setBoardNames(const ChessPosition* chess, Color color, Kind kind, int index, void* boardStr, void* arg2)
{
    Seat seat = Seats[index];
    ((char*)boardStr)[seat.row * (BOARDCOLNUM + 1) + seat.col] = Chars[color][kind];
}

static char* getChessPositionStr(char* chessStr, const ChessPosition* chess)
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
    snprintf(temp, 128, "player: %s\n", colorStrs[chess->player]);
    strcpy(chessStr, temp);
    for (Color color = RED; color <= BLACK; ++color) {
        snprintf(temp, 32, "chess->pieces[%s][Kind]:\n", colorStrs[!color]);
        strcat(chessStr, temp);

        getBoardArrayStr(temp, chess->pieces[!color], KINDNUM, KINDNUM, true, false);
        strcat(chessStr, temp);
    }

    snprintf(temp, 128, "calPieces[Color]: \nRED:         BLACK:    ALLCOLOR:\n");
    strcat(chessStr, temp);
    getBoardArrayStr(temp, chess->calPieces, ALLCOLOR + 1, KINDNUM, true, false);
    strcat(chessStr, temp);

    strcat(chessStr, "ROTATE: \n");
    getBoardArrayStr(temp, &chess->calPieces[ROTATE], 1, KINDNUM, true, true);
    strcat(chessStr, temp);

    strcat(chessStr, "chessBoardStr:\n");
    strcpy(temp, BoardStr);
    Color colors[COLORNUM] = { RED, BLACK };
    for (int i = 0; i < COLORNUM; ++i)
        traverseChessPieces(chess, colors[i], setBoardNames, temp, NULL);
    strcat(chessStr, temp);

    return chessStr;
}

void testChessPosition()
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
        printf("testFenPieChars:\npieChars: %s\nafen: %s\n fen: %s\n fen.Equal: %d\n\n",
            pieChars, afen, fen, strcmp(fen, afen));

        char chessStr[4 * 4096],
            setFen[BOARDLENGTH];
        ChessPosition chess = {};
        getChessPositionFromFen(&chess, afen);
        getChessPositionStr(chessStr, &chess);
        getFenFromChessPosition(setFen, &chess);

        printf("testChessPosition:\n%s  afen: %s\nsetFen: %s\nsetFen.Equal: %d\n",
            chessStr, afen, setFen, strcmp(setFen, afen));

        printf("testChessPositionMoveBoard:\n");
        MoveBoard moveBoards[MOVEBOARDMAXCOUNT];
        Color colors[COLORNUM] = { RED, BLACK };
        for (int i = 0; i < COLORNUM; ++i) {
            int count = getChessPositionMoveBoards(moveBoards, &chess, colors[i]);
            printf("color:%2d\n%smoveCount:%2d\n", colors[i], getMoveBoardsStr(chessStr, moveBoards, count), count);
        }
    }
}
