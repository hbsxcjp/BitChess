#include "move.h"
#include "board.h"
#include "data.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// #define DEBUGROOKCANNON1
// #define DEBUGROOKCANNON2
#define DEBUGKNIGHT
#define DEBUGBISHOP

#define LEGCOUNT 4
#define ROWSTATEMAX (1 << BOARDCOLNUM)
#define COLSTATEMAX (1 << BOARDROWNUM)
#define ROWBASEOFFSET(row) ((row)*BOARDCOLNUM)
#define COLBASEOFFSET(col) ((col)*BOARDROWNUM)
// #define ROWOFFSET(row, col) (ROWBASEOFFSET(row) + (col))
// #define COLOFFSET(col, row) (COLBASEOFFSET(col) + (row))

// 车炮处于每个位置的每种位置状态可移动位棋盘
static Board RookRowCanMove[BOARDCOLNUM][ROWSTATEMAX];
static Board RookColCanMove[BOARDROWNUM][COLSTATEMAX];
static Board CannonRowCanMove[BOARDCOLNUM][ROWSTATEMAX];
static Board CannonColCanMove[BOARDROWNUM][COLSTATEMAX];

static Board KnightCanMove[BOARDLENGTH][INTBITAT(LEGCOUNT)];
static Board BishopCanMove[BOARDLENGTH][INTBITAT(LEGCOUNT)];

// // 通过二分搜索计算右侧的连续零位（尾随）
// static unsigned int getLowNonZeroIndexFromUInt(unsigned int value)
// {
//     // unsigned int v; // 32-bit word input to count zero bits on right
//     unsigned int index; // c will be the number of zero bits on the right,
//     //                 // so if v is 1101000 (base 2), then c will be 3
//     // // NOTE: if 0 == v, then c = 31.
//     if (value & 0x1)
//     {
//         // special case for odd v (assumed to happen half of the time)
//         index = 0;
//     }
//     else
//     {
//         index = 1;
//         if ((value & 0xffff) == 0)
//         {
//             value >>= 16;
//             index += 16;
//         }
//         if ((value & 0xff) == 0)
//         {
//             value >>= 8;
//             index += 8;
//         }
//         if ((value & 0xf) == 0)
//         {
//             value >>= 4;
//             index += 4;
//         }
//         if ((value & 0x3) == 0)
//         {
//             value >>= 2;
//             index += 2;
//         }
//         index -= value & 0x1;
//     }

//     return index;
// }
// // 上面的代码类似于前面的方法，但它计算数字 通过以类似于二叉搜索的方式累加 c 来尾随零。 第一步，它检查 v 的底部 16 位是否为零， 如果是这样，则将 v 右移 16 位并将 16 位加到 c，从而减少 V 中要考虑的位数减半。每个后续 条件步骤同样将位数减半，直到只有 1。 这种方法比上一种方法快（大约33 %），因为身体 的 if 语句执行频率较低。
// //         马特·惠特洛克（Matt Whitlock）在25年2006月5日提出了这个建议。安德鲁·夏皮拉剃光 2007 年 1 月<>
// //             日关闭了几次操作（通过设置 c = <>
// //     和无条件 最后减去）。

// // 求最低位非零位的序号，调用前判断参数非零
// static unsigned int getLowNonZeroIndexFromRowCol(unsigned int rowOrCol)
// {
//     int index = 16 - 1;
//     if (rowOrCol & 0X00FF)
//     {
//         index -= 8;
//         rowOrCol &= 0X00FF;
//     }

//     if (rowOrCol & 0X0F0F)
//     {
//         index -= 4;
//         rowOrCol &= 0X0F0F;
//     }

//     if (rowOrCol & 0X3333)
//     {
//         index -= 2;
//         rowOrCol &= 0X3333;
//     }

//     if (rowOrCol & 0X5555)
//         index -= 1;

//     return index;
// }

// static int getLowNonZeroIndexs(int indexs[], int value)
// {
//     int count = 0;
//     while (value)
//     {
//         int index = getLowNonZeroIndexFromRowCol(value);
//         indexs[count++] = index;
//         value ^= INTBITAT(index);
//     }

//     return count;
// }

static int getMatch(int state, int rowColIndex, bool isCannon, bool isCol)
{
    int match = 0;
    for (int isHigh = 0; isHigh < 2; ++isHigh)
    {
        int direction = isHigh ? 1 : -1,
            endIndex = isHigh ? (isCol ? BOARDROWNUM : BOARDCOLNUM) - 1 : 0; // 每行列数或每列行数
        bool skip = false;                                                   // 炮是否已跳
        for (int i = direction * (rowColIndex + direction); i <= endIndex; ++i)
        {
            int index = direction * i;
            bool hasPiece = INTBITHAS(state, index);
            if (isCannon)
            {
                if (!skip)
                {
                    if (hasPiece)
                        skip = true;
                    else
                        match |= INTBITAT(index);
                }
                else if (hasPiece)
                {
                    match |= INTBITAT(index);
                    break;
                }
            }
            else
            {
                match |= INTBITAT(index);
                if (hasPiece) // 遇到棋子
                    break;
            }
        }
    }

    return match;
}

static void initRookCannonCanMove()
{
    for (Kind kind = ROOK; kind <= CANNON; ++kind)
    {
        bool isCannon = kind == CANNON;
        for (int isCol = 0; isCol < 2; ++isCol)
        {
            int stateTotal = isCol ? COLSTATEMAX : ROWSTATEMAX,
                length = isCol ? BOARDROWNUM : BOARDCOLNUM;
            for (int rowCloIndex = 0; rowCloIndex < length; ++rowCloIndex)
            {
#ifdef DEBUGROOKCANNON1
                char temp[32], temp2[32];
                int count = 0;
                printf("printRookCannonCanMove: Format:[state][match] %s, %s: %s\n",
                       isCannon ? "Cannon" : "Rook",
                       isCol ? "Col" : "Row",
                       getRowColBit(temp, INTBITAT(rowCloIndex), isCol));
#endif

                Board *moveMatchs = (isCannon
                                         ? (isCol ? CannonColCanMove[rowCloIndex] : CannonRowCanMove[rowCloIndex])
                                         : (isCol ? RookColCanMove[rowCloIndex] : RookRowCanMove[rowCloIndex]));
                for (int state = 0; state < stateTotal; ++state)
                {
                    // 本状态当前行或列位置无棋子
                    if (!INTBITHAS(state, rowCloIndex))
                        continue;

                    int match = getMatch(state, rowCloIndex, isCannon, isCol);
                    if (match == 0)
                    {
                        // printf("match==0: %s %s", getRowColBit(temp, state, isCol), getRowColBit(temp2, match, isCol));
                        continue;
                    }

                    if (isCol)
                    {
                        Board colMatch = 0;
                        for (int row = 0; row < BOARDROWNUM; ++row)
                        {
                            if (match & INTBITAT(row))
                                colMatch |= BoardMask[ROWBASEOFFSET(row)]; // 每行的首列置位
                        }
                        moveMatchs[state] = colMatch;
                    }
                    else
                        moveMatchs[state] = match;
#ifdef DEBUGROOKCANNON1
                    printf("%s %s", getRowColBit(temp, state, isCol), getRowColBit(temp2, match, isCol));
                    if (count % 5 == 4)
                        printf("\n");
                    else if (count != (stateTotal >> 1) - 1)
                        printf(" | ");
                    count++;
#endif
                }
#ifdef DEBUGROOKCANNON1
                printf("count: %d\n\n", count);
#endif

#ifdef DEBUGROOKCANNON2
                char temp[32];
                printf("printRookCannonCanMove: Format:[state][match] %s, %s: %s\n",
                       isCannon ? "Cannon" : "Rook",
                       isCol ? "Col" : "Row",
                       getRowColBit(temp, INTBITAT(rowCloIndex), isCol));

                char boardStr[stateTotal * (BOARDROWNUM + 2) * 16];
                getBoardStr(boardStr, moveMatchs, stateTotal, BOARDCOLNUM, false, false);
                printf("%s\n", boardStr);
#endif
            }
        }
    }
}

static void initKinghtCanMove()
{
    for (int index = 0; index < BOARDLENGTH; ++index)
    {
        Seat fromSeat = Seats[index];
        int row = fromSeat.row, col = fromSeat.col;
        for (int state = 0; state < INTBITAT(LEGCOUNT); ++state)
        {
            if (row == 0)
                state |= INTBITAT(LEGCOUNT - 1);
            else if (row == BOARDROWNUM - 1)
                state |= INTBITAT(LEGCOUNT - 4);
            if (col == 0)
                state |= INTBITAT(LEGCOUNT - 2);
            else if (col == BOARDCOLNUM - 1)
                state |= INTBITAT(LEGCOUNT - 3);

            Board match = 0;
            if (!(state & INTBITAT(LEGCOUNT - 1)) && row > 1)
            {
                if (col > 0)
                    match |= BoardMask[index - 2 * BOARDCOLNUM - 1];
                if (col < BOARDCOLNUM - 1)
                    match |= BoardMask[index - 2 * BOARDCOLNUM + 1];
            }
            if (!(state & INTBITAT(LEGCOUNT - 2)) && col > 1)
            {
                if (row > 0)
                    match |= BoardMask[index - 2 - BOARDCOLNUM];
                if (row < BOARDROWNUM - 1)
                    match |= BoardMask[index - 2 + BOARDCOLNUM];
            }
            if (!(state & INTBITAT(LEGCOUNT - 3)) && col < BOARDCOLNUM - 2)
            {
                if (row > 0)
                    match |= BoardMask[index + 2 - BOARDCOLNUM];
                if (row < BOARDROWNUM - 1)
                    match |= BoardMask[index + 2 + BOARDCOLNUM];
            }
            if (!(state & INTBITAT(LEGCOUNT - 4)) && row < BOARDROWNUM - 2)
            {
                if (col > 0)
                    match |= BoardMask[index + 2 * BOARDCOLNUM - 1];
                if (col < BOARDCOLNUM - 1)
                    match |= BoardMask[index + 2 * BOARDCOLNUM + 1];
            }

            KnightCanMove[index][state] = match;
        }
    }
}

static void initBishopCanMove()
{
    for (int index = 0; index < BOARDLENGTH; ++index)
    {
        Seat fromSeat = Seats[index];
        int row = fromSeat.row, col = fromSeat.col;
        if (!isValidBishop(row, col))
            continue;

        for (int state = 0; state < INTBITAT(LEGCOUNT); ++state)
        {
            if (row == 0 || row == 5)
                state |= (INTBITAT(LEGCOUNT - 1) | INTBITAT(LEGCOUNT - 4));
            else if (row == 4 || row == BOARDROWNUM - 1)
                state |= (INTBITAT(LEGCOUNT - 2) | INTBITAT(LEGCOUNT - 3));
            if (col == 0)
                state |= INTBITAT(LEGCOUNT - 2);
            else if (col == BOARDCOLNUM - 1)
                state |= INTBITAT(LEGCOUNT - 3);

            Board match = 0;
            if (!(state & INTBITAT(LEGCOUNT - 1)))
                match |= BoardMask[index - 2 * BOARDCOLNUM - 2];

            if (!(state & INTBITAT(LEGCOUNT - 2)))
                match |= BoardMask[index - 2 * BOARDCOLNUM + 2];

            if (!(state & INTBITAT(LEGCOUNT - 3)))
                match |= BoardMask[index + 2 * BOARDCOLNUM - 2];

            if (!(state & INTBITAT(LEGCOUNT - 4)))
                match |= BoardMask[index + 2 * BOARDCOLNUM + 2];

            BishopCanMove[index][state] = match;
        }

#ifdef DEBUGBISHOP
        printf("printBishopCanMove: fromIndex:%2d (%2d,%2d)\n", index, row, col);

        char boardStr[INTBITAT(LEGCOUNT) * (BOARDROWNUM + 2) * 16];
        getBoardStr(boardStr, BishopCanMove[index], INTBITAT(LEGCOUNT), INTBITAT(LEGCOUNT - 1), true, false);
        printf("%s\n", boardStr);
#endif
    }
}

void initPieceCanMove()
{
    initRookCannonCanMove();
    // initKinghtCanMove();
    initBishopCanMove();
}

Board getRookCannonCanMove(bool isCannon, int fromIndex, Board allPieces, Board rotatePieces)
{
    Seat fromSeat = Seats[fromIndex];
    int row = fromSeat.row, col = fromSeat.col,
        rowOffset = ROWBASEOFFSET(row);
    Board(*rowCanMove)[ROWSTATEMAX] = isCannon ? CannonRowCanMove : RookRowCanMove;
    Board(*colCanMove)[COLSTATEMAX] = isCannon ? CannonColCanMove : RookColCanMove;

    return ((rowCanMove[col][(allPieces >> rowOffset) & 0x1FF] << rowOffset) |
            (colCanMove[row][(rotatePieces >> COLBASEOFFSET(col)) & 0x3FF] << col)); // 每行首列置位全体移动数列
}

Board getKnightCanMove(int fromIndex, Board allPieces)
{
    Seat fromSeat = Seats[fromIndex];
    int row = fromSeat.row, col = fromSeat.col;
    int state = (((row > 0 ? allPieces & BoardMask[fromIndex - BOARDCOLNUM] : 0x1) << (LEGCOUNT - 1)) |
                 ((col > 0 ? allPieces & BoardMask[fromIndex - 1] : 0x1) << (LEGCOUNT - 2)) |
                 ((col < BOARDCOLNUM - 1 ? allPieces & BoardMask[fromIndex + 1] : 0x1) << (LEGCOUNT - 3)) |
                 ((row < BOARDROWNUM - 1 ? allPieces & BoardMask[fromIndex + BOARDCOLNUM] : 0x1) << (LEGCOUNT - 4)));

    return KnightCanMove[fromIndex][state];
}

Board getBishopCanMove(int fromIndex, Board allPieces)
{
    Seat fromSeat = Seats[fromIndex];
    int row = fromSeat.row, col = fromSeat.col;
    int state = 0;
    if (row != 0 && row != 5)
    {
        if (col > 0)
            state |= (allPieces & BoardMask[fromIndex - BOARDCOLNUM - 1]) << (LEGCOUNT - 1);
        if (col < BOARDCOLNUM - 1)
            state |= (allPieces & BoardMask[fromIndex - BOARDCOLNUM + 1]) << (LEGCOUNT - 2);
    }
    if (row != 4 && row != BOARDROWNUM - 1)
    {
        if (col > 0)
            state |= (allPieces & BoardMask[fromIndex + BOARDCOLNUM - 1]) << (LEGCOUNT - 3);
        if (col < BOARDCOLNUM - 1)
            state |= (allPieces & BoardMask[fromIndex + BOARDCOLNUM + 1]) << (LEGCOUNT - 4);
    }

    return BishopCanMove[fromIndex][state];
}
