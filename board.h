#ifndef BOARD_H
#define BOARD_H

#include "data.h"

void traverseColorKindPieces(Board* board, Color color, Kind kind, GetIndexFunc getIndexFunc, BitBoard bitBoard,
    void func(Board* board, Color color, Kind kind, int index, void* arg1, void* arg2),
    void* arg1, void* arg2);

void traverseColorPieces(Board* board, Color color,
    void func(Board* board, Color color, Kind kind, int index, void* arg1, void* arg2),
    void* arg1, void* arg2);

void traverseAllColorPieces(Board* board,
    void func(Board* board, Color color, Kind kind, int index, void* arg1, void* arg2),
    void* arg1, void* arg2);

char* getPieCharsFromFen(char* pieChars, const char* fen);

char* getFenFromPieChars(char* fen, const char* pieChars);

Board* getBoardFromFen(Board* board, const char* fen);

char* getFenFromBoard(char* fen, Board* board);

U64 getZobristFromBoard(Board* board);

bool isEqual(Board aboard, Board bboard);

char* getBoardStr(char* chessStr, Board* board);

wchar_t* getBoardWStr(wchar_t* chessStr, Board* board);

void testBoard();

#endif /* BOARD_H */
