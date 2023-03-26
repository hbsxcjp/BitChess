#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "move.h"
#include "board.h"

// 个人令牌
// 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

// git remote set-url origin git@github.com:hbsxcjp/BitChess.git

int main()
{
    // Test data.c
    initData();
    // printData();

    // Test board.c
    // printChessPosition();

    // Test move.c
    initPieceCanMove();
    printPieceCanMove();

    return 0;
}