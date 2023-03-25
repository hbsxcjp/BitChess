#ifndef BOARD_H
#define BOARD_H

#include "data.h"

ChessPosition *updateChessPositionData(ChessPosition *chess);

ChessPosition *setChessPositionFromFen(ChessPosition *chess, const char *fen);

char *setFenFromChessPosition(char *fen, const ChessPosition *chess);

char *getBoardStr(char *boardStr, const Board *boards, int length, int colNum);


void testChessPosition();

#endif /* BOARD_H */
