#include "move.h"
#include "board.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct Seat
{
    int row;
    int col;
} Seat;

// 打印初始化器的内容
typedef bool IsValid(int frow, int fcol);

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
        {frow, fcol - 1}, // W
        {frow, fcol + 1}, // E
        {frow - 1, fcol}, // S
        {frow + 1, fcol}  // N
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
            select[0] = select[1] = false;
        else if (frow > 7)
            select[2] = select[3] = false;
    }
    else if (fcol == 7) // 最右第二列
    {
        select[5] = select[7] = false;
        if (frow < 2)
            select[0] = select[1] = false;
        else if (frow > 7)
            select[2] = select[3] = false;
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
    IsValid *isValids[] = {
        isValidKing,
        isValidAdvisor,
        isValidBishop,
        isValid,
        isValid,
        isValid,
        isValid,
    }; //
    GetMoveTo *getMoveTos[] = {
        getKingMoveTo,
        getAdvisorMoveTo,
        getBishopMoveTo,
        getKnightMoveTo,
        getPawnMoveTo,
    };
    //

    printf("{");
    for (int i = 0; i < 5; ++i) // KINDNUM
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
    initPiecePut();

    initPieceMove();
}

const Board PiecePut[KINDNUM] = {
    (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 4) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 12) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 14) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 75) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 77) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 85) | (Board)1 << (BOARDBITSIZE - 1 - 86),
    (Board)1 << (BOARDBITSIZE - 1 - 3) | (Board)1 << (BOARDBITSIZE - 1 - 5) | (Board)1 << (BOARDBITSIZE - 1 - 13) | (Board)1 << (BOARDBITSIZE - 1 - 21) | (Board)1 << (BOARDBITSIZE - 1 - 23) | (Board)1 << (BOARDBITSIZE - 1 - 66) | (Board)1 << (BOARDBITSIZE - 1 - 68) | (Board)1 << (BOARDBITSIZE - 1 - 76) | (Board)1 << (BOARDBITSIZE - 1 - 84) | (Board)1 << (BOARDBITSIZE - 1 - 86),
    (Board)1 << (BOARDBITSIZE - 1 - 2) | (Board)1 << (BOARDBITSIZE - 1 - 6) | (Board)1 << (BOARDBITSIZE - 1 - 18) | (Board)1 << (BOARDBITSIZE - 1 - 22) | (Board)1 << (BOARDBITSIZE - 1 - 26) | (Board)1 << (BOARDBITSIZE - 1 - 38) | (Board)1 << (BOARDBITSIZE - 1 - 42) | (Board)1 << (BOARDBITSIZE - 1 - 47) | (Board)1 << (BOARDBITSIZE - 1 - 51) | (Board)1 << (BOARDBITSIZE - 1 - 63) | (Board)1 << (BOARDBITSIZE - 1 - 67) | (Board)1 << (BOARDBITSIZE - 1 - 71) | (Board)1 << (BOARDBITSIZE - 1 - 83) | (Board)1 << (BOARDBITSIZE - 1 - 87),
    (Board)(-1),
    (Board)(-1),
    (Board)(-1),
    (Board)(-1)};

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
