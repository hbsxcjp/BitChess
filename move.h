#ifndef MOVE_H
#define MOVE_H

#include "data.h"


void initPieceCanMove();

Board getRookCannonCanMove(bool isCannon, int fromIndex, Board allPieces, Board rotatePieces);

#endif /* MOVE_H */
