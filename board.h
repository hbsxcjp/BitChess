#ifndef BOARD_H
#define BOARD_H

#include "data.h"

ChessPosition *updateChessPositionData(ChessPosition *chess);

ChessPosition *getChessPositionFromFen(ChessPosition *chess, const char *fen);

char *getFenFromChessPosition(char *fen, const ChessPosition *chess);

char *getBoardStr(char *boardStr, const Board *boards, int length, int colNum);

void printChessPosition();

#endif /* BOARD_H */
