#include "move.h"
#include "board.h"
#include "data.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define HasPiece(rowOrColInt, index) (rowOrColInt & (1 << index))

#define MODEMAX 600
#define ROWSTATEMAX (1 << BOARDCOLNUM)
#define COLSTATEMAX (1 << BOARDROWNUM)
#define ROWOFFSET(row) (row * BOARDCOLNUM)
#define COLOFFSET(col) (col * BOARDROWNUM)

typedef struct RowColMode {
    int count;

    int mode[MODEMAX];
    int fetch[MODEMAX];
    int match[MODEMAX];
} RowColMode;

// 车炮所处行列位置的匹配模式
RowColMode RookRowMode[BOARDCOLNUM];
RowColMode RookColMode[BOARDROWNUM];
RowColMode CannonRowMode[BOARDCOLNUM];
RowColMode CannonColMode[BOARDROWNUM];

// 车炮处于每个位置的每种位置状态可移动位棋盘
Board RookRowCanMove[BOARDLENGTH][ROWSTATEMAX];
Board RookColCanMove[BOARDLENGTH][COLSTATEMAX];
Board CannonRowCanMove[BOARDLENGTH][ROWSTATEMAX];
Board CannonColCanMove[BOARDLENGTH][COLSTATEMAX];

static void initRookCannonRowColMode()
{
    for (Kind kind = ROOK; kind <= CANNON; ++kind) {
        bool isCannon = kind == CANNON;
        for (int isCol = 0; isCol < 2; ++isCol) {
            int length = isCol ? BOARDROWNUM : BOARDCOLNUM;
            RowColMode* rowColModes = (isCannon
                    ? (isCol ? CannonColMode : CannonRowMode)
                    : (isCol ? RookColMode : RookRowMode));
            for (int index = 0; index < length; ++index) {
                int count = 0, atIndex = INTBITAT(index);
                RowColMode rowColMode = {};
                for (int lowBit = index - 1; lowBit >= -1; --lowBit) {
                    int lowMode = atIndex, lowFetch = atIndex, lowMatch = 0;
                    // 范围置位
                    for (int i = lowBit == -1 ? 0 : lowBit; i < index; ++i) {
                        lowFetch |= INTBITAT(i);
                        lowMatch |= INTBITAT(i);
                    }
                    if (lowBit > -1) {
                        lowMode |= INTBITAT(lowBit);
                        if (isCannon)
                            lowMatch ^= INTBITAT(lowBit);
                    }

                    // 当lowBit在0或-1时，skipLowBit循环一次
                    bool lowSkip = isCannon && lowBit < index - 1;
                    int skipLow = lowSkip ? lowBit : 0;
                    for (int skipLowBit = skipLow; skipLowBit >= 0; --skipLowBit) {
                        int skipLowMode = 0, skipLowFetch = 0, skipLowMatch = 0;
                        // 左边已跳棋子
                        if (lowSkip) {
                            // 界限置位
                            skipLowMode = INTBITAT(skipLowBit);
                            // 范围置位
                            for (int i = skipLowBit; i <= skipLow; ++i)
                                skipLowFetch |= INTBITAT(i);

                            // 匹配置位
                            skipLowMatch = INTBITAT(skipLowBit);
                        }

                        for (int highBit = index + 1; highBit <= length; ++highBit) {
                            int highMode = 0, highFetch = 0, highMatch = 0;
                            // 范围置位
                            for (int i = index + 1; i <= (highBit == length ? length - 1 : highBit); ++i) {
                                highFetch |= INTBITAT(i);
                                highMatch |= INTBITAT(i);
                            }
                            if (highBit < length) {
                                highMode = INTBITAT(highBit);
                                if (isCannon)
                                    highMatch ^= INTBITAT(highBit);
                            }

                            // 当highBit在length-1或length时，循环一次
                            // bool highSkip = isCannon && highBit > index + 1;
                            int skipHigh = isCannon ? highBit : length - 1;
                            for (int skipHighBit = skipHigh; skipHighBit < length; ++skipHighBit) {
                                int skipHighMode = 0, skipHighFetch = 0, skipHighMatch = 0;
                                // 右边已跳棋子
                                if (isCannon) {
                                    // 界限置位
                                    skipHighMode = INTBITAT(skipHighBit);
                                    // 范围置位
                                    for (int i = skipHigh; i <= skipHighBit; ++i)
                                        skipHighFetch |= INTBITAT(i);

                                    // 匹配置位
                                    skipHighMatch = INTBITAT(skipHighBit);
                                }

                                rowColMode.mode[count] = skipLowMode | lowMode | highMode | skipHighMode;
                                rowColMode.fetch[count] = skipLowFetch | lowFetch | highFetch | skipHighFetch;
                                rowColMode.match[count] = skipLowMatch | lowMatch | highMatch | skipHighMatch;
                                ++count;
                            }
                        }
                    }
                }
                rowColMode.count = count;
                rowColModes[index] = rowColMode;
            }
        }
    }
}

static void initRookCannonRowColCanMove()
{
    for (Kind kind = ROOK; kind <= CANNON; ++kind) {
        for (int isCol = 0; isCol < 2; ++isCol) {
            int index = 0;
            int stateTotal = isCol ? COLSTATEMAX : ROWSTATEMAX;
            for (int row = 0; row < BOARDROWNUM; ++row) {
                for (int col = 0; col < BOARDCOLNUM; ++col) {
                    int offset = isCol ? COLOFFSET(col) : ROWOFFSET(row),
                        modeIndex = isCol ? row : col;
                    RowColMode rowColMode = (kind == ROOK
                            ? (isCol ? RookColMode[modeIndex] : RookRowMode[modeIndex])
                            : (isCol ? CannonColMode[modeIndex] : CannonRowMode[modeIndex]));
                    Board* boards = (kind == ROOK ? (isCol ? RookColCanMove[index] : RookRowCanMove[index])
                                                  : (isCol ? CannonColCanMove[index] : CannonRowCanMove[index]));
                    for (int state = 0; state < stateTotal; ++state) {
                        // 棋子在此状态上
                        if (state & BoardMask[modeIndex]) {
                            for (int i = 0; i < rowColMode.count; ++i)
                                if ((state & rowColMode.fetch[i]) == rowColMode.mode[i]) {
                                    boards[state] = BOARDFROM(rowColMode.match[i], offset);
                                    break;
                                }
                        }
                    }

                    ++index;
                }
            }
        }
    }
}

// static void initRookColCanMove()
// {
//     int index = 0;
//     for (int col = 0; col < BOARDCOLNUM; ++col) {
//         int offset = COLOFFSET(col);
//         for (int row = 0; row < BOARDROWNUM; ++row) {
//             RowColMode rowColMode = RookColMode[row];
//             for (int state = 0; state < COLSTATEMAX; ++state) {
//                 if (state & BoardMask[row]) // 棋子在此状态上
//                 {
//                     RookColCanMove[index][state] = getModeMatchBoard(state, rowColMode, offset); // ?
//                 }
//             }

//             ++index;
//         }
//     }
// }

void initPieceCanMove()
{
    initRookCannonRowColMode();

    initRookCannonRowColCanMove();
}

// Count the consecutive zero bits(trailing)
// on the right in parallel unsigned int v; // 32-bit word input to count zero bits on right
// unsigned int c = 32;                     // c will be the number of zero bits on the right
// v &= -signed(v);
// if (v)
//     c--;
// if (v & 0x0000FFFF)
//     c -= 16;
// if (v & 0x00FF00FF)
//     c -= 8;
// if (v & 0x0F0F0F0F)
//     c -= 4;
// if (v & 0x33333333)
//     c -= 2;
// if (v & 0x55555555)
//     c -= 1;
// Here, we are basically doing the same operations as finding the log base 2 in parallel, but we first isolate the lowest 1 bit, and then proceed with c starting at the maximum and decreasing. The number of operations is at most 3 * lg(N) + 4, roughly, for N bit words.
// Bill Burdick suggested an optimization, reducing the time from 4 * lg(N)
// on February 4, 2011.

// 通过二分搜索计算右侧的连续零位（尾随）
// unsigned int v; // 32-bit word input to count zero bits on right
// unsigned int c; // c will be the number of zero bits on the right,
//                 // so if v is 1101000 (base 2), then c will be 3
// // NOTE: if 0 == v, then c = 31.
// if (v & 0x1)
// {
//     // special case for odd v (assumed to happen half of the time)
//     c = 0;
// }
// else
// {
//     c = 1;
//     if ((v & 0xffff) == 0)
//     {
//         v >>= 16;
//         c += 16;
//     }
//     if ((v & 0xff) == 0)
//     {
//         v >>= 8;
//         c += 8;
//     }
//     if ((v & 0xf) == 0)
//     {
//         v >>= 4;
//         c += 4;
//     }
//     if ((v & 0x3) == 0)
//     {
//         v >>= 2;
//         c += 2;
//     }
//     c -= v & 0x1;
// }
// 上面的代码类似于前面的方法，但它计算数字 通过以类似于二叉搜索的方式累加 c 来尾随零。 第一步，它检查 v 的底部 16 位是否为零， 如果是这样，则将 v 右移 16 位并将 16 位加到 c，从而减少 V 中要考虑的位数减半。每个后续 条件步骤同样将位数减半，直到只有 1。 这种方法比上一种方法快（大约33 %），因为身体 的 if 语句执行频率较低。
//         马特·惠特洛克（Matt Whitlock）在25年2006月5日提出了这个建议。安德鲁·夏皮拉剃光 2007 年 1 月<>
//             日关闭了几次操作（通过设置 c = <>
//     和无条件 最后减去）。

// 求最低位非零位的序号，调用前判断参数非零
// static int getLowNonZeroIndexFromRowInt(int rowOrCol)
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

// static int getLowNonZeroIndexsFromRowOrCol(int indexs[], int rowOrCol)
// {
//     int count = 0;
//     while (rowOrCol)
//     {
//         int index = getLowNonZeroIndexFromRowInt(rowOrCol);
//         indexs[count++] = index;
//         rowOrCol ^= 1 << index;
//     }

//     return count;
// }

// static int
// getToInt(int state, int curColOrRow, bool isCannon, bool isRow, int high)
// {
//     int toInt = 0;
//     int direction = high ? 1 : -1,
//         end = high ? (isRow ? BOARDCOLNUM : BOARDROWNUM) - 1 : 0; // 每行列数或每列行数
//     bool skip = false;                                            // 炮是否已跳
//     for (int i = direction * (curColOrRow + direction); i <= end; ++i)
//     {
//         int index = direction * i;
//         bool hasPiece = HasPiece(state, index);
//         if (isCannon)
//         {
//             if (!skip)
//             {
//                 if (hasPiece)
//                     skip = true;
//                 else
//                     toInt |= 1 << index;
//             }
//             else if (hasPiece)
//             {
//                 toInt |= 1 << index;
//                 break;
//             }
//         }
//         else
//         {
//             toInt |= 1 << index;
//             if (hasPiece) // 遇到棋子
//                 break;
//         }
//     }

//     return toInt;
// }

// void initRookCannonCanMove(int stateCount, Board canMove[][stateCount], bool isCannon, bool isRow)
// {
//     for (int index = 0; index < BOARDLENGTH; ++index)
//     {
//         int curColOrRow = isRow ? Seats[index].col : Seats[index].row; // 当前序号的某行(列)
//         for (int state = 0; state < stateCount; ++state)               // 每行或每列的全部二进制状态
//         {
//             if (!HasPiece(state, curColOrRow)) // 本状态curColOrRow位置无棋子，置0后继续
//                 continue;

//             int toInt = 0;
//             for (int high = 0; high < 2; ++high)
//                 toInt = getToInt(state, curColOrRow, isCannon, isRow, high);

//             if (toInt == 0)
//                 continue;

//             canMove[index][state] = BOARDFROM(toInt, curColOrRow * (isRow ? BOARDCOLNUM : BOARDROWNUM));
//             if (!isRow)
//             {
//                 // 旋转90度 canMove[index][state]
//             }
//         }
//     }
// }

static void printRookCannonRowColMode()
{
    char temp[32];
    for (Kind kind = CANNON; kind <= CANNON; ++kind)
    // for (Kind kind = ROOK; kind <= ROOK; ++kind)
    // for (Kind kind = ROOK; kind <= CANNON; ++kind)
    {
        bool isCannon = kind == CANNON;
        for (int isCol = 0; isCol < 2; ++isCol) {
            printf("printRookCannonRowColMode: [%s]\n", isCannon ? "Cannon" : "Rook");
            int length = isCol ? BOARDROWNUM : BOARDCOLNUM;
            RowColMode* rowColModes = (kind == ROOK
                    ? (isCol ? RookColMode : RookRowMode)
                    : (isCol ? CannonColMode : CannonRowMode));
            for (int index = 0; index < length; ++index) {
                RowColMode rowColMode = rowColModes[index];
                int count = rowColMode.count;
                printf("Mode[%s][%d]:\t  count: %d\nmode:      fetch:     match:\n", isCol ? "Col" : "Row", index, count);
                for (int i = 0; i < count; ++i) {
                    printf(getRowColBit(temp, rowColMode.mode[i], isCol));
                    printf(getRowColBit(temp, rowColMode.fetch[i], isCol));
                    printf(getRowColBit(temp, rowColMode.match[i], isCol));
                    printf("\n");
                }
                printf("\n");
            }
        }
    }
}

static void printRookCannonCanMove(bool isCannon, bool isCol)
{
    printf("printRookCannonCanMove: [%s] [%s]\n", isCannon ? "Cannon" : "Rook", isCol ? "Col" : "Row");
    char temp[32];
    int index = 0;
    Board* canMove;
    for (int row = 0; row < BOARDROWNUM; ++row) {
        for (int col = 0; col < BOARDCOLNUM; ++col) {
            int count = 0,
                offset = isCol ? COLOFFSET(col) : ROWOFFSET(row);
            printf("%s: %s canMove:  Format[state][match]\n",
                isCol ? "Col" : "Row", getRowColBit(temp, 1 << (isCol ? row : col), isCol));
            if (isCannon) {
                canMove = isCol ? CannonColCanMove[index] : CannonRowCanMove[index];
            } else {
                canMove = isCol ? RookColCanMove[index] : RookRowCanMove[index];
            }
            int stateTotal = isCol ? COLSTATEMAX : ROWSTATEMAX;
            for (int state = 0; state < stateTotal; ++state) {
                if (canMove[state]) {
                    printf(getRowColBit(temp, state, isCol));
                    printf(getRowColBit(temp, canMove[state] >> offset, isCol));
                    if (count % 5 == 4)
                        printf("\n");
                    else if (state != stateTotal - 1)
                        printf("| ");

                    count++;
                }
            }
            printf("\n\n");

            ++index;
        }
    }
}

void printPieceCanMove()
{
    printRookCannonRowColMode();

    for (Kind kind = ROOK; kind <= CANNON; ++kind) {
        bool isCannon = kind == CANNON;
        for (int isCol = 0; isCol < 2; ++isCol) {
            // printRookCannonCanMove(isCannon, isCol);
        }
    }
}