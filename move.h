#ifndef MOVE_H
#define MOVE_H

#include "data.h"

ChessPosition* doMoveChessPosition(ChessPosition* chess, Color color, Kind kind, int fromIndex, int toIndex);

#endif /* MOVE_H */
