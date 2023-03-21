#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "board.h"
#include "move.h"

typedef struct Seat
{
    int row;
    int col;
} Seat;

// 打印初始化器的内容
typedef int IsValid(int frow, int fcol);

typedef int GetMoveTo(int toIndex[], int frow, int fcol);

static int getIndexFromSeat(Seat seat)
{
    return seat.row * BOARDCOLNUM + seat.col;
}

static char *getPieceMoveStr(char *moveStr, IsValid *isValid, GetMoveTo *getMoveTo)
{
    strcpy(moveStr, "{");
    for (int row = 0; row < BOARDROWNUM; ++row)
    {
        for (int col = 0; col < BOARDCOLNUM; ++col)
        {
            if (isValid(row, col))
            {
                int toIndex[BOARDCOLNUM + BOARDROWNUM] = {};
                int count = getMoveTo(toIndex, row, col);
                char oneStr[64] = {}, allStr[count * 64];
                strcpy(allStr, "");
                for (int i = 0; i < count; ++i)
                {
                    snprintf(oneStr, 64, "(Board)1 << (BOARDBITSIZE - 1 - %d)|", toIndex[i]);
                    strcat(allStr, oneStr);
                }
                allStr[strlen(allStr) - 1] = ','; // 最后的'|'替换成‘,’
                strcat(moveStr, allStr);
                strcat(moveStr, "\n");
            }
            else
                strcat(moveStr, "0,\n");
        }
    }
    moveStr[strlen(moveStr) - 2] = '\x0'; // 去掉最后的",\n"
    strcat(moveStr, "},\n");

    return moveStr;
}

static int isValidKing(int frow, int fcol)
{
    return (frow < 3 || frow > 6) && (fcol > 2 && fcol < 6);
}

static int getKingMoveTo(int toIndex[], int frow, int fcol)
{
    int count = 0;
    // 方向   W,   E,    S,    N
    int select[] = {1, 1, 1, 1};
    const Seat tseats[] = {
        {frow, fcol - 1}, // W
        {frow, fcol + 1}, // E
        {frow - 1, fcol}, // S
        {frow + 1, fcol}  // N
    };

    if (fcol == 3) // 最左列
        select[0] = 0;
    else if (fcol == 5) // 最右列
        select[1] = 0;
    if (frow == 0 || frow == 7)
        select[2] = 0;
    else if (frow == 2 || frow == 9)
        select[3] = 0;

    for (int i = 0; i < sizeof(tseats) / sizeof(tseats[0]); ++i)
        if (select[i])
            toIndex[count++] = getIndexFromSeat(tseats[i]);

    return count;
}

static int isValidAdvisor(int frow, int fcol)
{
    return (frow < 3 || frow > 6) && (fcol > 2 && fcol < 6);
}

static int getAdvisorMoveTo(int toIndex[], int frow, int fcol)
{
    int count = 0;
    // 方向   W,   E,    S,    N
    int select[] = {1, 1, 1, 1, 1};
    const Seat tseats[] = {
        {frow - 1, fcol - 1},
        {frow - 1, fcol + 1},
        {frow + 1, fcol - 1},
        {frow + 1, fcol + 1},
        {(frow > 4 ? 0 : 7) + 1, 4}};

    if (fcol == 4)
        select[4] = 0;
    else
        select[0] = select[1] = select[2] = select[3] = 0;

    for (int i = 0; i < sizeof(tseats) / sizeof(tseats[0]); ++i)
        if (select[i])
            toIndex[count++] = getIndexFromSeat(tseats[i]);

    return count;
}

void initPieceMove()
{
    IsValid *isValids[] = {
        isValidKing,
    }; // isValidAdvisor
    GetMoveTo *getMoveTos[] = {
        getKingMoveTo,
    };
    // getAdvisorMoveTo

    printf("{");
    for (int i = 0; i < 1; ++i) // KINDNUM
    {
        char moveStr[BOARDLENGTH * (BOARDCOLNUM + BOARDROWNUM) * 64];
        getPieceMoveStr(moveStr, isValids[i], getMoveTos[i]);
        printf(moveStr);
    }

    printf("}");
}

const Board PieceMove[KINDNUM][BOARDLENGTH] = {
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
    {},
    {},
    {},
    {},
    {},
    {}};
