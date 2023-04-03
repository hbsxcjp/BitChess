#include "move.h"
#include "board.h"
#include "data.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>


ChessPosition* doMoveChessPosition(ChessPosition* chess, Color color, Kind kind, int fromIndex, int toIndex)
{
    chess->player = !chess->player;

    // 清除原位置，置位新位置
    Board toBoard = BoardMask[toIndex],
          moveBoard = BoardMask[fromIndex] | toBoard;
    chess->calPieces[color] ^= moveBoard;
    chess->pieces[color][kind] ^= moveBoard;

    // 新位置有对方棋子时
    Color toColor = !color;
    if (chess->calPieces[toColor] & toBoard) {
        chess->calPieces[toColor] ^= toBoard;
        for (Kind toKind = KING; toKind <= PAWN; ++toKind)
            chess->pieces[toColor][toKind] ^= toBoard;
    }

    return updateAllPieces(chess);
}