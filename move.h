#ifndef MOVE_H
#define MOVE_H

#include "data.h"

Kind doMove(ChessPosition* chess, Color color, Kind kind, int fromIndex, int toIndex);

void undoMove(ChessPosition* chess, Color color, Kind kind, int fromIndex, int toIndex, Kind eatKind);

int getChessPositionCanMove(Move moves[], ChessPosition* chess, Color color);

#endif /* MOVE_H */
