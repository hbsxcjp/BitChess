#include "move.h"
#include "board.h"
#include "data.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// #define DEBUGFILTERKILLEDMOVE

Kind doMove(ChessPosition* chess, Color color, Kind kind, int fromIndex, int toIndex)
{
    Board toBoard = BoardMask[toIndex],
          rotateToBoard = BoardMask[Rotate[toIndex]];
    turnColorKindPieces(chess, color, kind, BoardMask[fromIndex] | toBoard, BoardMask[Rotate[fromIndex]] | rotateToBoard);

    Color toColor = !color;
    Kind thisToKind = NONKIND;
    // 新位置有对方棋子时
    if (chess->calPieces[toColor] & toBoard) {
        for (Kind toKind = KING; toKind < NONKIND; ++toKind) {
            if (chess->pieces[toColor][toKind] & toBoard) {
                thisToKind = toKind;
                break;
            }
        }
        // assert(thisToKind != NONKIND);

        turnColorKindPieces(chess, toColor, thisToKind, toBoard, rotateToBoard);
    }

    return thisToKind;
}

void undoMove(ChessPosition* chess, Color color, Kind kind, int fromIndex, int toIndex, Kind eatKind)
{
    // 清除原位置，置位新位置
    Board fromBoard = BoardMask[fromIndex],
          rotateFromBoard = BoardMask[Rotate[fromIndex]];
    turnColorKindPieces(chess, color, kind, fromBoard | BoardMask[toIndex], rotateFromBoard | BoardMask[Rotate[toIndex]]);
    if (eatKind != NONKIND)
        turnColorKindPieces(chess, !color, eatKind, fromBoard, rotateFromBoard);
}

static Board getMove(ChessPosition* chess, Color color, Kind kind, int index)
{
    Board move;
    switch (kind) {
    case KING:
        move = KingMove[index];
        break;
    case ADVISOR:
        move = AdvisorMove[index];
        break;
    case BISHOP:
        move = getBishopMove(index, chess->calPieces[ALLCOLOR]);
        break;
    case KNIGHT:
        move = getKnightMove(index, chess->calPieces[ALLCOLOR]);
        break;
    case ROOK:
        move = getRookMove(index, chess->calPieces[ALLCOLOR], chess->calPieces[ROTATE]);
        break;
    case CANNON:
        move = getCannonMove(index, chess->calPieces[ALLCOLOR], chess->calPieces[ROTATE]);
        break;
    default:
        move = PawnMove[index][chess->bottomColor == color];
        break;
    }

    // 去掉同色棋子
    return move ^ (move & chess->calPieces[color]);
}

static void addMove(ChessPosition* chess, Color color, Kind kind, int index, void* colorMove, void* arg2)
{
    *(Board*)colorMove |= getMove(chess, color, kind, index);
}

// 获取某种颜色棋子可移动至的全部位置
static Board getColorMove(ChessPosition* chess, Color color)
{
    Board colorMove = 0;
    traverseColorPieces(chess, color, addMove, &colorMove, NULL);
    return colorMove;
}

static void filterKilledMove(ChessPosition* chess, Color color, Kind kind, int toIndex, void* pfromIndex, void* pmoveTo)
{
#ifdef DEBUGFILTERKILLEDMOVE
    ChessPosition tempChess = *chess;
#endif
    int fromIndex = *(int*)pfromIndex;
    Kind eatKind = doMove(chess, color, kind, fromIndex, toIndex);
    // 排除被杀将的位置
    if (getColorMove(chess, !color) & chess->pieces[color][KING])
        *(Board*)pmoveTo ^= BoardMask[toIndex];

    undoMove(chess, color, kind, toIndex, fromIndex, eatKind);

#ifdef DEBUGFILTERKILLEDMOVE
    char chessStr[4 * 4096];
    if (!isEqual(tempChess, *chess)) {
        printf("testGetMoveTo:\ntempChess:\n%s", getChessPositionStr(chessStr, &tempChess));
        printf("*chess:\n%s\n", getChessPositionStr(chessStr, chess));
    }
    assert(isEqual(tempChess, *chess));
#endif
}

static Move getMoveTo(ChessPosition* chess, Color color, Kind kind, int fromIndex)
{
    Board moveTo = getMove(chess, color, kind, fromIndex),
          tempMove = moveTo;

    traverseColorKindPieces(chess, color, kind, getNonZeroIndex(chess, color), tempMove,
        filterKilledMove, &fromIndex, &moveTo);

    return (Move) { color, kind, fromIndex, moveTo };
}

static void getMoveTos(ChessPosition* chess, Color color, Kind kind, int index, void* moves, void* count)
{
    ((Move*)moves)[(*(int*)count)++] = getMoveTo(chess, color, kind, index);
}

int getChessPositionMoveTos(Move moves[], ChessPosition* chess, Color color)
{
    int count = 0;
    traverseColorPieces(chess, color, getMoveTos, moves, &count);

    return count;
}
