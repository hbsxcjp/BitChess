#include "board.h"
#include "move.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>

// 个人令牌
// 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

// git remote set-url origin git@github.com:hbsxcjp/BitChess.git

int main()
{
    setlocale(LC_ALL, "zh_CN");
    // Test data.c
    // initData();
    // printData();

    // Test board.c
    // printChessPosition();

    // Test move.c
    initPieceCanMove();

    return 0;
}