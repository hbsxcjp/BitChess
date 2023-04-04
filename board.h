#ifndef BOARD_H
#define BOARD_H

#include "data.h"

ChessPosition* updateAllPieces(ChessPosition* chess);

ChessPosition* getChessPositionFromFen(ChessPosition* chess, const char* fen);

char* getFenFromChessPosition(char* fen, const ChessPosition* chess);

int getChessPositionMoveBoards(MoveBoard moveBoards[], const ChessPosition* chess, Color color);

void testChessPosition();

#endif /* BOARD_H */
