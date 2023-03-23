#include "move.h"
#include "board.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "move.h"
#include "board.h"

typedef struct Seat {
    int row;
    int col;
} Seat;


const Board PiecePut[KINDNUM] = {
    (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 86),
    (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 86),
    (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 87),
    (Board)(-1),
    (Board)(-1),
    (Board)(-1),
    (Board)(-1)};

const Board PieceMove[KINDNUM][BOARDLENGTH] = {
    // Piece Kind: 0
    {0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 12),
     (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 13),
     (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 14),
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 21),
     (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 22),
     (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 23),
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 12),
     (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 13),
     (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 14),
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 75),
     (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 76),
     (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 77),
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 84),
     (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 85),
     (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 86),
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 75),
     (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 76),
     (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 77),
     0,
     0,
     0},

    // Piece Kind: 1
    {0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 13),
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 13),
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 23),
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 13),
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 13),
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 76),
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 76),
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 86),
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 76),
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 76),
     0,
     0,
     0},

    // Piece Kind: 2
    {0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 22),
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 38),
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 42),
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 42),
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 22),
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 67),
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 83),
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 87),
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 87),
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 67),
     0,
     0,
     0,
     (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     0,
     0},

    // Piece Kind: 3
    {(Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 11),
     (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 12),
     (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 13),
     (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 14),
     (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 15),
     (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 16),
     (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 14),
     (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 15),
     (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 20),
     (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 21),
     (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 22),
     (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 23),
     (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 24),
     (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 25),
     (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 23),
     (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 24),
     (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 29),
     (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 30),
     (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 31),
     (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 32),
     (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 33),
     (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 34),
     (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 32),
     (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 33),
     (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 38),
     (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 39),
     (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 40),
     (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 41),
     (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 42),
     (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 43),
     (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 41),
     (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 42),
     (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 47),
     (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 48),
     (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 49),
     (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 50),
     (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 51),
     (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 52),
     (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 50),
     (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 51),
     (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 56),
     (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 57),
     (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 58),
     (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 59),
     (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 60),
     (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 61),
     (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 59),
     (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 60),
     (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 65),
     (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 66),
     (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 67),
     (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 68),
     (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 69),
     (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 70),
     (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 68),
     (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 69),
     (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 74),
     (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 75),
     (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 76),
     (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 77),
     (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 78),
     (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 79),
     (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 77),
     (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 78),
     (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 83),
     (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 84),
     (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 85),
     (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 86),
     (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 87),
     (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 88),
     (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 86),
     (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 87),
     (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 74),
     (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 75),
     (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 76),
     (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 77),
     (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 78),
     (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 79),
     (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 77),
     (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 78)},

    // Piece Kind: 4
    {(Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0),
     (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9),
     (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18),
     (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27),
     (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36),
     (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45),
     (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54),
     (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63),
     (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72),
     (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81)},

    // Piece Kind: 5
    {(Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 8),
     (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 0),
     (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 9),
     (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 18),
     (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 27),
     (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 36),
     (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 45),
     (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 54),
     (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 63),
     (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 72),
     (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 81)},

    // Piece Kind: 6
    {(Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 9),
     (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 10),
     (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 11),
     (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 12),
     (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 13),
     (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 14),
     (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 15),
     (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 16),
     (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 17),
     (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 0) | (Board)1 << (BOARDBITSIZE - 1 - 18),
     (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 1) | (Board)1 << (BOARDBITSIZE - 1 - 19),
     (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 20),
     (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 21),
     (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 22),
     (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 23),
     (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 24),
     (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 7) | (Board)1 << (BOARDBITSIZE - 1 - 25),
     (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 8) | (Board)1 << (BOARDBITSIZE - 1 - 26),
     (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 9) | (Board)1 << (BOARDBITSIZE - 1 - 27),
     (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 10) | (Board)1 << (BOARDBITSIZE - 1 - 28),
     (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 11) | (Board)1 << (BOARDBITSIZE - 1 - 29),
     (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 30),
     (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 31),
     (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 32),
     (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 15) | (Board)1 << (BOARDBITSIZE - 1 - 33),
     (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 16) | (Board)1 << (BOARDBITSIZE - 1 - 34),
     (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 17) | (Board)1 << (BOARDBITSIZE - 1 - 35),
     (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 36),
     (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 19) | (Board)1 << (BOARDBITSIZE - 1 - 37),
     (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 20) | (Board)1 << (BOARDBITSIZE - 1 - 38),
     (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 39),
     (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 40),
     (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 41),
     (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 24) | (Board)1 << (BOARDBITSIZE - 1 - 42),
     (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 25) | (Board)1 << (BOARDBITSIZE - 1 - 43),
     (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 44),
     (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 27) | (Board)1 << (BOARDBITSIZE - 1 - 45),
     (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 28) | (Board)1 << (BOARDBITSIZE - 1 - 46),
     (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 29) | (Board)1 << (BOARDBITSIZE - 1 - 47),
     (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 30) | (Board)1 << (BOARDBITSIZE - 1 - 48),
     (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 31) | (Board)1 << (BOARDBITSIZE - 1 - 49),
     (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 32) | (Board)1 << (BOARDBITSIZE - 1 - 50),
     (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 33) | (Board)1 << (BOARDBITSIZE - 1 - 51),
     (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 34) | (Board)1 << (BOARDBITSIZE - 1 - 52),
     (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 35) | (Board)1 << (BOARDBITSIZE - 1 - 53),
     (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 36) | (Board)1 << (BOARDBITSIZE - 1 - 54),
     (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 37) | (Board)1 << (BOARDBITSIZE - 1 - 55),
     (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 56),
     (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 39) | (Board)1 << (BOARDBITSIZE - 1 - 57),
     (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 40) | (Board)1 << (BOARDBITSIZE - 1 - 58),
     (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 41) | (Board)1 << (BOARDBITSIZE - 1 - 59),
     (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 60),
     (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 43) | (Board)1 << (BOARDBITSIZE - 1 - 61),
     (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 44) | (Board)1 << (BOARDBITSIZE - 1 - 62),
     (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 45) | (Board)1 << (BOARDBITSIZE - 1 - 63),
     (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 46) | (Board)1 << (BOARDBITSIZE - 1 - 64),
     (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 65),
     (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 48) | (Board)1 << (BOARDBITSIZE - 1 - 66),
     (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 49) | (Board)1 << (BOARDBITSIZE - 1 - 67),
     (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 50) | (Board)1 << (BOARDBITSIZE - 1 - 68),
     (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 69),
     (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 52) | (Board)1 << (BOARDBITSIZE - 1 - 70),
     (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 53) | (Board)1 << (BOARDBITSIZE - 1 - 71),
     (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 54) | (Board)1 << (BOARDBITSIZE - 1 - 72),
     (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 55) | (Board)1 << (BOARDBITSIZE - 1 - 73),
     (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 56) | (Board)1 << (BOARDBITSIZE - 1 - 74),
     (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 57) | (Board)1 << (BOARDBITSIZE - 1 - 75),
     (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 58) | (Board)1 << (BOARDBITSIZE - 1 - 76),
     (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 59) | (Board)1 << (BOARDBITSIZE - 1 - 77),
     (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 60) | (Board)1 << (BOARDBITSIZE - 1 - 78),
     (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 61) | (Board)1 << (BOARDBITSIZE - 1 - 79),
     (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 62) | (Board)1 << (BOARDBITSIZE - 1 - 80),
     (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 81),
     (Board)1 << (BOARDBITSIZE - 1 - 72) | (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 64) | (Board)1 << (BOARDBITSIZE - 1 - 82),
     (Board)1 << (BOARDBITSIZE - 1 - 73) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 65) | (Board)1 << (BOARDBITSIZE - 1 - 83),
     (Board)1 << (BOARDBITSIZE - 1 - 74) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 84),
     (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 85),
     (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 86),
     (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 69) | (Board)1 << (BOARDBITSIZE - 1 - 87),
     (Board)1 << (BOARDBITSIZE - 1 - 78) | (Board)1 << (BOARDBITSIZE - 1 - 80) | (Board)1 << (BOARDBITSIZE - 1 - 70) | (Board)1 << (BOARDBITSIZE - 1 - 88),
     (Board)1 << (BOARDBITSIZE - 1 - 79) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 89),
     (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 72),
     (Board)1 << (BOARDBITSIZE - 1 - 81) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 73),
     (Board)1 << (BOARDBITSIZE - 1 - 82) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 74),
     (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 75),
     (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 76),
     (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 77),
     (Board)1 << (BOARDBITSIZE - 1 - 86) | (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 78),
     (Board)1 << (BOARDBITSIZE - 1 - 87) | (Board)1 << (BOARDBITSIZE - 1 - 89) | (Board)1 << (BOARDBITSIZE - 1 - 79),
     (Board)1 << (BOARDBITSIZE - 1 - 88) | (Board)1 << (BOARDBITSIZE - 1 - 80)},
};

// 打印初始化器的内容
void initBoardMask()
{
    for (int i = 0; i < BOARDLENGTH; ++i)
        printf("(Board)1 << (BOARDBITSIZE - 1 - %d),\n", i);
}

// 打印初始化器的内容
typedef bool IsValid(int frow, int fcol);

typedef int GetMoveTo(int toIndex[], int frow, int fcol);

bool isValid(int frow, int fcol) { return true; } // 所有位置均有效

static int getIndexFromSeat(Seat seat)
{
    return seat.row * BOARDCOLNUM + seat.col;
}

static char* getPieceMoveStr(char* moveStr, IsValid* isValid, GetMoveTo* getMoveTo)
{
    strcpy(moveStr, "{");
    for (int row = 0; row < BOARDROWNUM; ++row) {
        for (int col = 0; col < BOARDCOLNUM; ++col) {
            if (isValid(row, col)) {
                int toIndex[BOARDCOLNUM + BOARDROWNUM] = {};
                int count = getMoveTo(toIndex, row, col);
                char oneStr[64] = {}, allStr[count * 64];
                strcpy(allStr, "");
                for (int i = 0; i < count; ++i) {
                    snprintf(oneStr, 64, "(Board)1 << (BOARDBITSIZE - 1 - %d)|", toIndex[i]);
                    strcat(allStr, oneStr);
                }
                allStr[strlen(allStr) - 1] = ','; // 最后的'|'替换成‘,’
                strcat(moveStr, allStr);
                strcat(moveStr, "\n");
            } else
                strcat(moveStr, "0,\n");
        }
    }
    moveStr[strlen(moveStr) - 2] = '\x0'; // 去掉最后的",\n"
    strcat(moveStr, "},\n");

    return moveStr;
}

static bool isValidKing(int frow, int fcol)
{
    return (frow < 3 || frow > 6) && (fcol > 2 && fcol < 6);
}

static int getKingMoveTo(int toIndex[], int frow, int fcol)
{
    int count = 0;
    // 方向   W,   E,    S,    N
    bool select[] = {true, true, true, true};
    const Seat tseats[] = {
        { frow, fcol - 1 }, // W
        { frow, fcol + 1 }, // E
        { frow - 1, fcol }, // S
        { frow + 1, fcol } // N
    };

    if (fcol == 3) // 最左列
        select[0] = false;
    else if (fcol == 5) // 最右列
        select[1] = false;
    if (frow == 0 || frow == 7)
        select[2] = false;
    else if (frow == 2 || frow == 9)
        select[3] = false;

    for (int i = 0; i < sizeof(tseats) / sizeof(tseats[0]); ++i)
        if (select[i])
            toIndex[count++] = getIndexFromSeat(tseats[i]);

    return count;
}

static bool isValidAdvisor(int frow, int fcol)
{
    return (((frow == 0 || frow == 2 || frow == 7 || frow == 9) && (fcol == 3 || fcol == 5)) || ((frow == 1 || frow == 8) && fcol == 4));
}

static int getAdvisorMoveTo(int toIndex[], int frow, int fcol)
{
    int count = 0;
    // 方向   W,   E,    S,    N
    bool select[] = {true, true, true, true, true};
    const Seat tseats[] = {
        {frow - 1, fcol - 1},
        {frow - 1, fcol + 1},
        {frow + 1, fcol - 1},
        {frow + 1, fcol + 1},
        {frow < 4 ? 1 : 8, 4}};

    if (fcol == 4)
        select[4] = false;
    else
        select[0] = select[1] = select[2] = select[3] = false;

    for (int i = 0; i < sizeof(tseats) / sizeof(tseats[0]); ++i)
        if (select[i])
            toIndex[count++] = getIndexFromSeat(tseats[i]);

    return count;
}

static bool isValidBishop(int frow, int fcol)
{
    return (((frow == 0 || frow == 4 || frow == 5 || frow == 9) && (fcol == 2 || fcol == 6)) || ((frow == 2 || frow == 7) && (fcol == 0 || fcol == 4 || fcol == 8)));
}

static int getBishopMoveTo(int toIndex[], int frow, int fcol)
{
    int count = 0;
    bool select[] = {true, true, true, true};
    const Seat tseats[] = {
        {frow - 2, fcol - 2}, // SW
        {frow - 2, fcol + 2}, // SE
        {frow + 2, fcol - 2}, // NW
        {frow + 2, fcol + 2}  // NE
    };
    if (fcol == 0)
        select[0] = select[2] = false;
    else if (fcol == 8)
        select[1] = select[3] = false;
    if (frow == 0 || frow == 5)
        select[0] = select[1] = false;
    else if (frow == 4 || frow == 9)
        select[2] = select[3] = false;

    for (int i = 0; i < sizeof(tseats) / sizeof(tseats[0]); ++i)
        if (select[i])
            toIndex[count++] = getIndexFromSeat(tseats[i]);

    return count;
}

static int getKnightMoveTo(int toIndex[], int frow, int fcol)
{
    int count = 0;
    // {SW, SE, NW, NE, WS, ES, WN, EN}
    bool select[] = {true, true, true, true, true, true, true, true};
    const Seat tseats[] = {
        {frow - 2, fcol - 1}, // SW
        {frow - 2, fcol + 1}, // SE
        {frow + 2, fcol - 1}, // NW
        {frow + 2, fcol + 1}, // NE
        {frow - 1, fcol - 2}, // WS
        {frow - 1, fcol + 2}, // ES
        {frow + 1, fcol - 2}, // WN
        {frow + 1, fcol + 2}  // EN
    };

    if (fcol == 0) // 最左列
    {
        select[0] = select[2] = select[4] = select[6] = false;
        if (frow == 0) // 最底行
            select[1] = select[5] = false;
        else if (frow == 9) // 最顶行
            select[3] = select[7] = false;
        else if (frow == 1) // 最底第二行
            select[1] = false;
        else if (frow == 8) // 最顶第二行
            select[3] = false;
    }
    else if (fcol == 8) // 最右列
    {
        select[1] = select[3] = select[5] = select[7] = false;
        if (frow == 0)
            select[0] = select[4] = false;
        else if (frow == 9)
            select[2] = select[6] = false;
        else if (frow == 1)
            select[0] = false;
        else if (frow == 8)
            select[2] = false;
    }
    else if (fcol == 1) // 最左第二列
    {
        select[4] = select[6] = false;
        if (frow < 2)
        {
            select[0] = select[1] = false;
            if (frow == 0)
                select[4] = select[5] = false;
        }
        else if (frow > 7)
        {
            select[2] = select[3] = false;
            if (frow == 9)
                select[6] = select[7] = false;
        }
    }
    else if (fcol == 7) // 最右第二列
    {
        select[5] = select[7] = false;
        if (frow < 2)
        {
            select[0] = select[1] = false;
            if (frow == 0)
                select[4] = select[5] = false;
        }
        else if (frow > 7)
        {
            select[2] = select[3] = false;
            if (frow == 9)
                select[6] = select[7] = false;
        }
    }
    else
    {
        if (frow == 0)
            select[0] = select[4] = select[1] = select[5] = false;
        else if (frow == 9)
            select[3] = select[7] = select[6] = select[2] = false;
        else if (frow == 1)
            select[0] = select[1] = false;
        else if (frow == 8)
            select[2] = select[3] = false;
    }

    for (int i = 0; i < sizeof(tseats) / sizeof(tseats[0]); ++i)
        if (select[i])
            toIndex[count++] = getIndexFromSeat(tseats[i]);

    return count;
}

static int getRookCannonMoveTo(int toIndex[], int frow, int fcol)
{
    int count = 0;
    for (int row = frow - 1; row >= 0; --row)
        toIndex[count++] = getIndexFromSeat((Seat){row, fcol});

    for (int row = frow + 1; row <= 9; ++row)
        toIndex[count++] = getIndexFromSeat((Seat){row, fcol});

    for (int col = fcol - 1; col >= 0; --col)
        toIndex[count++] = getIndexFromSeat((Seat){frow, col});

    for (int col = fcol + 1; col <= 8; ++col)
        toIndex[count++] = getIndexFromSeat((Seat){frow, col});

    return count;
}

static int getPawnMoveTo(int toIndex[], int frow, int fcol)
{
    int count = 0;
    //           方向   W,   E,    S,    N
    bool select[] = {true, true, true, true};
    const Seat tseats[] = {
        {frow, fcol - 1}, // W
        {frow, fcol + 1}, // E
        {frow - 1, fcol}, // S
        {frow + 1, fcol}  // N
    };
    if (frow == 0)
        select[2] = false;
    else if (frow == 9)
        select[3] = false;

    if (fcol == 0)
        select[0] = false;
    else if (fcol == 8)
        select[1] = false;

    for (int i = 0; i < sizeof(tseats) / sizeof(tseats[0]); ++i)
        if (select[i])
            toIndex[count++] = getIndexFromSeat(tseats[i]);

    return count;
}

void initPiecePut()
{
    IsValid *isValids[] = {
        isValidKing,
        isValidAdvisor,
        isValidBishop,
    };

    char putStr[4096];
    strcpy(putStr, "");
    for (int i = 0; i < sizeof(isValids) / sizeof(isValids[0]); ++i)
    {
        char oneStr[64] = {}, allStr[4096];
        snprintf(oneStr, 64, "\n// Piece Kind: %d\n", i);
        strcpy(allStr, oneStr);
        for (int row = 0; row < BOARDROWNUM; ++row)
        {
            for (int col = 0; col < BOARDCOLNUM; ++col)
            {
                if (isValids[i](row, col))
                {
                    snprintf(oneStr, 64, "(Board)1 << (BOARDBITSIZE - 1 - %d)|", getIndexFromSeat((Seat){row, col}));
                    strcat(allStr, oneStr);
                }
            }
        }
        allStr[strlen(allStr) - 1] = ','; // 最后的'|'替换成‘,’
        strcat(putStr, allStr);
        strcat(putStr, "\n");
    }

    printf("{%s\n// Piece Kind: 3\n(Board)(-1),\n\n// Piece Kind: 4\n(Board)(-1),"
           "\n\n// Piece Kind: 5\n(Board)(-1),\n\n// Piece Kind: 6\n(Board)(-1)\n}\n",
           putStr);
}

// 所有位置均有效
bool isValid(int frow, int fcol) { return true; }

void initPieceMove()
{
    IsValid* isValids[] = {
        isValidKing,
        isValidAdvisor,
        isValidBishop,
        isValid,
        isValid,
        isValid,
        isValid,
    };
    GetMoveTo *getMoveTos[] = {
        getKingMoveTo,
        getAdvisorMoveTo,
        getBishopMoveTo,
        getKnightMoveTo,
        getRookCannonMoveTo,
        getRookCannonMoveTo,
        getPawnMoveTo,
    };

    printf("{");
    for (int i = 0; i < KINDNUM; ++i) //
    {
        printf("\n// Piece Kind: %d\n", i);
        char moveStr[BOARDLENGTH * (BOARDCOLNUM + BOARDROWNUM) * 64];
        getPieceMoveStr(moveStr, isValids[i], getMoveTos[i]);
        printf(moveStr);
    }

    printf("}");
}

void testInitMoveStr()
{
    // initPiecePut();
    // initPieceMove();

    char boardStr[KINDNUM * (BOARDROWNUM + 2) * 16];
    getBoardStr(boardStr, PiecePut, KINDNUM, KINDNUM);
    printf("testPiecePut:\n%s\n", boardStr);

    for (int kind = 0; kind < KINDNUM; ++kind)
    {
        char boardStr[BOARDLENGTH * (BOARDROWNUM + 2) * 16];
        getBoardStr(boardStr, PieceMove[kind], BOARDLENGTH, BOARDCOLNUM);
        printf("testPieceMove kind: %d\n%s\n", kind, boardStr);
    }
}
