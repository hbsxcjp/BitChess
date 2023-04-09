#include "board.h"
#include "instance.h"
#include "move.h"
#include <ctype.h>
#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// 个人令牌
// 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

// git remote set-url origin git@github.com:hbsxcjp/BitChess.git

int main()
{
    setlocale(LC_ALL, "zh_CN");
    // wprintf(L"中华人们共和国\n"); //？

    // Test data.c
    initData();

    // Test board.c
    testBoard();

    // Test move.c

    // Test instance.c
    testInstance();

    return 0;
}