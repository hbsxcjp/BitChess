#ifndef BOARD_H
#define BOARD_H

#include "data.h"

ChessPosition* getChessPositionFromFen(ChessPosition* chess, const char* fen);

char* getFenFromChessPosition(char* fen, ChessPosition* chess);

void testChessPosition();

#endif /* BOARD_H */
