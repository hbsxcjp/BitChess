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

static const char NullChar = '_';

static const char SplitChar = '/';

static const char EndChar = '\x0';

static const char FEN[] = "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR";

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

ChessPosition* getChessPositionFromFen(ChessPosition* chess, const char* fen)
{
    char pieChars[BOARDLENGTH + 1] = {};
    getPieCharsFromFen(pieChars, fen);

    char ch;
    Board turnBoard[COLORNUM][KINDNUM] = {},
          rotateTurnBoard[COLORNUM][KINDNUM] = {};
    for (int index = 0; index < BOARDLENGTH; ++index) {
        if ((ch = pieChars[index]) != NullChar) {
            Color color = getColor(ch);
            Kind kind = getKind(ch);
            turnBoard[color][kind] |= BoardMask[index];
            rotateTurnBoard[color][kind] |= BoardMask[Rotate[index]];
        }
    }

    // 设置分颜色棋盘
    for (Color color = RED; color <= BLACK; ++color)
        for (Kind kind = KING; kind <= PAWN; ++kind)
            turnColorKindPieces(chess, color, kind, turnBoard[color][kind], rotateTurnBoard[color][kind]);

    chess->bottomColor = getNonZeroIndex(chess, RED)(chess->pieces[RED][KING]) > (BOARDLENGTH >> 1) ? RED : BLACK;
    return chess;
}

static void setPieChars(ChessPosition* chess, Color color, Kind kind, int index, void* pieChars, void* arg2)
{
    ((char*)pieChars)[index] = Chars[color][kind];
}

char* getFenFromChessPosition(char* fen, ChessPosition* chess)
{
    char pieChars[BOARDLENGTH + 1];
    for (int i = 0; i < BOARDLENGTH; ++i)
        pieChars[i] = NullChar;

    traverseAllColorPieces(chess, setPieChars, pieChars, NULL);
    pieChars[BOARDLENGTH] = EndChar;

    return getFenFromPieChars(fen, pieChars);
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

        printf("getChessPositionCanMove:\n");
        Move moves[MOVEBOARDMAXCOUNT];
        Color colors[COLORNUM] = { RED, BLACK };
        for (int i = 0; i < COLORNUM; ++i) {
            int count = getChessPositionCanMove(moves, &chess, colors[i]);
            printf("%smoveCount:%2d\n", getMoveArrayStr(chessStr, moves, count, KINDNUM + 1), count);
        }

        wchar_t chessWStr[1024];
        getChessPositionWStr(chessWStr, &chess);
        wprintf(L"%ls\n", chessWStr);
    }
}
