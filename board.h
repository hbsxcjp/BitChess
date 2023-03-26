#ifndef BOARD_H
#define BOARD_H

#include "data.h"

ChessPosition *getChessPositionFromFen(ChessPosition *chess, const char *fen);

char *getFenFromChessPosition(char *fen, const ChessPosition *chess);

ChessPosition *doMoveChessPosition(ChessPosition *chess, Color color, Kind kind, int fromIndex, int toIndex);

void printChessPosition();

#endif /* BOARD_H */
