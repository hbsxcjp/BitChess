#include "move.h"
#include "board.h"
#include "data.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define DEBUGMOVE

#define MODEMAX 96
#define ROWSTATEMAX (1 << (BOARDCOLNUM - 1))
#define COLSTATEMAX (1 << (BOARDROWNUM - 1))
#define ROWOFFSET(row, col) ((row)*BOARDCOLNUM + (col))
#define COLOFFSET(col, row) ((col)*BOARDROWNUM + (row))

#define HasPiece(state, index) ((state) & (1 << (index)))

// 车炮处于每个位置的每种位置状态可移动位棋盘
static Board RookRowCanMove[BOARDLENGTH][ROWSTATEMAX];
static Board RookColCanMove[BOARDLENGTH][COLSTATEMAX];
static Board CannonRowCanMove[BOARDLENGTH][ROWSTATEMAX];
static Board CannonColCanMove[BOARDLENGTH][COLSTATEMAX];

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

static Board getMatch(int state, int outIndex, int inIndex, bool isCannon, bool isCol)
{
    int match = 0;
    for (int isHigh = 0; isHigh < 2; ++isHigh)
    {
        int direction = isHigh ? 1 : -1,
            endIndex = isHigh ? (isCol ? BOARDROWNUM : BOARDCOLNUM) - 1 : 0; // 每行列数或每列行数
        bool skip = false;                                                   // 炮是否已跳
        for (int i = direction * (inIndex + direction); i <= endIndex; ++i)
        {
            int index = direction * i;
            bool hasPiece = HasPiece(state, index);
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

    if (isCol)
    {
        Board colMatch = 0;
        for (int row = 0; row < BOARDROWNUM; ++row)
            if (match & INTBITAT(row))
                colMatch |= BoardMask[ROWOFFSET(row, outIndex)];

        return colMatch;
    }

    return BOARDFROM(match, ROWOFFSET(outIndex, inIndex));
}

static void initRookCannonCanMove()
{
    for (Kind kind = ROOK; kind <= CANNON; ++kind)
    {
        bool isCannon = kind == CANNON;
        for (int isCol = 0; isCol < 2; ++isCol)
        {
            int stateTotal = isCol ? COLSTATEMAX : ROWSTATEMAX,
                outLength = isCol ? BOARDCOLNUM : BOARDROWNUM,
                inLength = isCol ? BOARDROWNUM : BOARDCOLNUM,
                boardIndex = 0;
            for (int outIndex = 0; outIndex < outLength; ++outIndex)
            {
                for (int inIndex = 0; inIndex < inLength; ++inIndex)
                {
#ifdef DEBUGMOVE
                    char temp[32],
                        boardStr[BOARDLENGTH * (BOARDROWNUM + 2) * 16];
                    int count = 0;
#endif

                    Board *moveMatchs = (isCannon
                                             ? (isCol ? CannonColCanMove[boardIndex] : CannonRowCanMove[boardIndex])
                                             : (isCol ? RookColCanMove[boardIndex] : RookRowCanMove[boardIndex]));
                    for (int state = 0; state < stateTotal; ++state)
                    {
                        // 本状态当前行或列位置无棋子
                        if (!HasPiece(state, inIndex))
                            continue;

                        int match = getMatch(state, outIndex, inIndex, isCannon, isCol);
                        if (match == 0)
                            continue;

                        moveMatchs[state] = match;
#ifdef DEBUGMOVE
                        count++;
#endif
                    }
#ifdef DEBUGMOVE
                    printf("printRookCannonCanMove:\nFormat:[state][match] %s, %s: %s\n",
                           isCannon ? "Cannon" : "Rook",
                           isCol ? "Col" : "Row",
                           getRowColBit(temp, INTBITAT(inIndex), isCol));
                    getBoardStr(boardStr, moveMatchs, count, BOARDCOLNUM, false);
                    printf("state:%s\n%s\n", getRowColBit(temp, state, isCol), boardStr);

                    count++;
#endif

#ifdef DEBUGMOVE
                    printf("\ncount: %d\n\n", count);
#endif
                    ++boardIndex;
                }
            }
        }
    }
}

void initPieceCanMove()
{
    initRookCannonCanMove();
}