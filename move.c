#include "move.h"
#include "board.h"
#include "data.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// #define DEBUGMODE
// #define DEBUGMOVE
// #define DEBUGMOVE2
#define DEBUGCOMPAREMOVES

#define MODEMAX 96
#define ROWSTATEMAX (1 << BOARDCOLNUM)
#define COLSTATEMAX (1 << BOARDROWNUM)
#define ROWOFFSET(row) (row * BOARDCOLNUM)
#define COLOFFSET(col) (col * BOARDROWNUM)

#define HasPiece(state, index) (state & (1 << index))

typedef struct RowColMode
{
    int count;

    int mode[MODEMAX];
    int fetch[MODEMAX];
    int match[MODEMAX];
} RowColMode;

// 车炮所处行列位置的匹配模式
static RowColMode RookRowMode[BOARDCOLNUM];
static RowColMode RookColMode[BOARDROWNUM];
static RowColMode CannonRowMode[BOARDCOLNUM];
static RowColMode CannonColMode[BOARDROWNUM];

// 车炮处于每个位置的每种位置状态可移动位棋盘
static int RookRowCanMove[BOARDCOLNUM][ROWSTATEMAX];
static int RookColCanMove[BOARDROWNUM][COLSTATEMAX];
static int CannonRowCanMove[BOARDCOLNUM][ROWSTATEMAX];
static int CannonColCanMove[BOARDROWNUM][COLSTATEMAX];

static void initRookCannonRowColMode()
{
    for (Kind kind = ROOK; kind <= CANNON; ++kind)
    {
        bool isCannon = kind == CANNON;
        for (int isCol = 0; isCol < 2; ++isCol)
        {
            int length = isCol ? BOARDROWNUM : BOARDCOLNUM;
            RowColMode *rowColModes = (isCannon
                                           ? (isCol ? CannonColMode : CannonRowMode)
                                           : (isCol ? RookColMode : RookRowMode));
            for (int index = 0; index < length; ++index)
            {
                int count = 0, atIndex = INTBITAT(index);
                RowColMode rowColMode = {};
                for (int lowBit = index - 1; lowBit >= -1; --lowBit)
                {
                    int lowMode = atIndex, lowFetch = atIndex, lowMatch = 0;
                    for (int i = lowBit == -1 ? 0 : lowBit; i < index; ++i)
                        lowFetch |= INTBITAT(i);

                    lowMatch = lowFetch ^ atIndex;
                    if (lowBit > -1)
                    {
                        lowMode |= INTBITAT(lowBit);
                        if (isCannon)
                            lowMatch ^= INTBITAT(lowBit);
                    }

                    // 取0时，只一次循环
                    int skipLow = isCannon ? lowBit - 1 : 0;
                    if (skipLow < 0)
                        skipLow = 0;
                    // int skipHigh = isCannon ? (highBit == index + 1 ? highBit + 1 : highBit) : length - 1;
                    for (int skipLowBit = skipLow; skipLowBit >= 0; --skipLowBit)
                    {
                        int skipLowMode = 0, skipLowFetch = 0, skipLowMatch = 0;
                        if (isCannon && lowBit > 0)
                        {
                            skipLowMode = INTBITAT(skipLowBit);
                            skipLowMatch = INTBITAT(skipLowBit);
                            for (int i = skipLowBit; i <= skipLow; ++i)
                                skipLowFetch |= INTBITAT(i);
                        }

                        for (int highBit = index + 1; highBit <= length; ++highBit)
                        {
                            int highMode = 0, highFetch = 0, highMatch = 0;
                            for (int i = index + 1; i <= (highBit == length ? length - 1 : highBit); ++i)
                                highFetch |= INTBITAT(i);

                            highMatch = highFetch;
                            if (highBit < length)
                            {
                                highMode = INTBITAT(highBit);
                                if (isCannon)
                                    highMatch ^= INTBITAT(highBit);
                            }

                            // 取length-1时，只一次循环
                            int skipHigh = isCannon ? highBit + 1 : length - 1;
                            if (skipHigh >= length)
                                skipHigh = length - 1;
                            for (int skipHighBit = skipHigh; skipHighBit < length; ++skipHighBit)
                            {
                                int skipHighMode = 0, skipHighFetch = 0, skipHighMatch = 0;
                                if (isCannon && highBit < length - 1)
                                {
                                    skipHighMode = INTBITAT(skipHighBit);
                                    skipHighMatch = INTBITAT(skipHighBit);
                                    for (int i = skipHigh; i <= skipHighBit; ++i)
                                        skipHighFetch |= INTBITAT(i);
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

#ifdef DEBUGMODE
                char temp[32];
                printf("printRookCannonRowColMode:\nFormat:[mode][fetch][match]\n%s, %s: %s count: %d\n",
                       isCannon ? "Cannon" : "Rook", isCol ? "Col" : "Row", getRowColBit(temp, INTBITAT(index), isCol), count);
                for (int i = 0; i < count; ++i)
                {
                    printf(getRowColBit(temp, rowColMode.mode[i], isCol));
                    printf(getRowColBit(temp, rowColMode.fetch[i], isCol));
                    printf(getRowColBit(temp, rowColMode.match[i], isCol));
                    printf("\n");
                }
                printf("\n");
#endif
            }
        }
    }
}

static void initRookCannonRowColMove()
{
    for (Kind kind = ROOK; kind <= CANNON; ++kind)
    {
        bool isCannon = kind == CANNON;
        for (int isCol = 0; isCol < 2; ++isCol)
        {
            int stateTotal = isCol ? COLSTATEMAX : ROWSTATEMAX,
                length = isCol ? BOARDROWNUM : BOARDCOLNUM;
            for (int index = 0; index < length; ++index)
            {
#ifdef DEBUGMOVE
                char temp[32];
                int count = 0;
                printf("printRookCannonCanMove:\nFormat:[state][match] %s, %s: %s\n",
                       isCannon ? "Cannon" : "Rook", isCol ? "Col" : "Row", getRowColBit(temp, INTBITAT(index), isCol));
#endif

                RowColMode rowColMode = (isCannon
                                             ? (isCol ? CannonColMode[index] : CannonRowMode[index])
                                             : (isCol ? RookColMode[index] : RookRowMode[index]));
                int *moveMatchs = (isCannon
                                       ? (isCol ? CannonColCanMove[index] : CannonRowCanMove[index])
                                       : (isCol ? RookColCanMove[index] : RookRowCanMove[index]));
                for (int state = 0; state < stateTotal; ++state)
                {
                    // 棋子在此状态上
                    if (state & BoardMask[index])
                    {
                        for (int i = 0; i < rowColMode.count; ++i)
                            if ((state & rowColMode.fetch[i]) == rowColMode.mode[i])
                            {
                                moveMatchs[state] = rowColMode.match[i];
#ifdef DEBUGMOVE
                                printf(getRowColBit(temp, state, isCol));
                                printf(getRowColBit(temp, moveMatchs[state], isCol));
                                if (count % 5 == 4)
                                    printf("\n");
                                else if (state != stateTotal - 1)
                                    printf("| ");

                                count++;
#endif

                                break;
                            }
                    }
                }

#ifdef DEBUGMOVE
                printf("\ncount: %d\n\n", count);
#endif
            }
        }
    }
}

// 通过二分搜索计算右侧的连续零位（尾随）
static unsigned int getLowNonZeroIndexFromUInt(unsigned int value)
{
    // unsigned int v; // 32-bit word input to count zero bits on right
    unsigned int index; // c will be the number of zero bits on the right,
    //                 // so if v is 1101000 (base 2), then c will be 3
    // // NOTE: if 0 == v, then c = 31.
    if (value & 0x1)
    {
        // special case for odd v (assumed to happen half of the time)
        index = 0;
    }
    else
    {
        index = 1;
        if ((value & 0xffff) == 0)
        {
            value >>= 16;
            index += 16;
        }
        if ((value & 0xff) == 0)
        {
            value >>= 8;
            index += 8;
        }
        if ((value & 0xf) == 0)
        {
            value >>= 4;
            index += 4;
        }
        if ((value & 0x3) == 0)
        {
            value >>= 2;
            index += 2;
        }
        index -= value & 0x1;
    }

    return index;
}
// 上面的代码类似于前面的方法，但它计算数字 通过以类似于二叉搜索的方式累加 c 来尾随零。 第一步，它检查 v 的底部 16 位是否为零， 如果是这样，则将 v 右移 16 位并将 16 位加到 c，从而减少 V 中要考虑的位数减半。每个后续 条件步骤同样将位数减半，直到只有 1。 这种方法比上一种方法快（大约33 %），因为身体 的 if 语句执行频率较低。
//         马特·惠特洛克（Matt Whitlock）在25年2006月5日提出了这个建议。安德鲁·夏皮拉剃光 2007 年 1 月<>
//             日关闭了几次操作（通过设置 c = <>
//     和无条件 最后减去）。

// 求最低位非零位的序号，调用前判断参数非零
static unsigned int getLowNonZeroIndexFromRowCol(unsigned int rowOrCol)
{
    int index = 16 - 1;
    if (rowOrCol & 0X00FF)
    {
        index -= 8;
        rowOrCol &= 0X00FF;
    }

    if (rowOrCol & 0X0F0F)
    {
        index -= 4;
        rowOrCol &= 0X0F0F;
    }

    if (rowOrCol & 0X3333)
    {
        index -= 2;
        rowOrCol &= 0X3333;
    }

    if (rowOrCol & 0X5555)
        index -= 1;

    return index;
}

static int getLowNonZeroIndexs(int indexs[], int value)
{
    int count = 0;
    while (value)
    {
        int index = getLowNonZeroIndexFromRowCol(value);
        indexs[count++] = index;
        value ^= INTBITAT(index);
    }

    return count;
}

static int getRowColMatch(int state, int rowOrCol, bool isCannon, bool isCol, int high)
{
    int match = 0;
    int direction = high ? 1 : -1,
        end = high ? (isCol ? BOARDROWNUM : BOARDCOLNUM) - 1 : 0; // 每行列数或每列行数
    bool skip = false;                                            // 炮是否已跳
    for (int i = direction * (rowOrCol + direction); i <= end; ++i)
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
                    match |= 1 << index;
            }
            else if (hasPiece)
            {
                match |= 1 << index;
                break;
            }
        }
        else
        {
            match |= 1 << index;
            if (hasPiece) // 遇到棋子
                break;
        }
    }

    return match;
}

// 车炮处于每个位置的每种位置状态可移动位棋盘
static int RookRowCanMove2[BOARDCOLNUM][ROWSTATEMAX];
static int RookColCanMove2[BOARDROWNUM][COLSTATEMAX];
static int CannonRowCanMove2[BOARDCOLNUM][ROWSTATEMAX];
static int CannonColCanMove2[BOARDROWNUM][COLSTATEMAX];

static void initRookCannonRowColMove2()
{
    for (Kind kind = ROOK; kind <= CANNON; ++kind)
    {
        bool isCannon = kind == CANNON;
        for (int isCol = 0; isCol < 2; ++isCol)
        {
            int stateTotal = isCol ? COLSTATEMAX : ROWSTATEMAX,
                length = isCol ? BOARDROWNUM : BOARDCOLNUM;
            for (int index = 0; index < length; ++index)
            {
#ifdef DEBUGMOVE2
                char temp[32];
                int count = 0;
                printf("printRookCannonCanMove2:\nFormat:[state][match] %s, %s: %s\n",
                       isCannon ? "Cannon" : "Rook", isCol ? "Col" : "Row", getRowColBit(temp, INTBITAT(index), isCol));
#endif

                int *moveMatchs = (isCannon
                                       ? (isCol ? CannonColCanMove2[index] : CannonRowCanMove2[index])
                                       : (isCol ? RookColCanMove2[index] : RookRowCanMove2[index]));
                for (int state = 0; state < stateTotal; ++state)
                {
                    if (!HasPiece(state, index)) // 本状态curColOrRow位置无棋子，置0后继续
                        continue;

                    int match = 0;
                    for (int high = 0; high < 2; ++high)
                        match = getRowColMatch(state, index, isCannon, isCol, high);

                    if (match == 0)
                        continue;

                    moveMatchs[state] = match;
#ifdef DEBUGMOVE2
                    printf(getRowColBit(temp, state, isCol));
                    printf(getRowColBit(temp, moveMatchs[state], isCol));
                    if (count % 5 == 4)
                        printf("\n");
                    else if (state != stateTotal - 1)
                        printf("| ");

                    count++;
#endif
                }

#ifdef DEBUGMOVE2
                printf("\ncount: %d\n\n", count);
#endif
            }
        }
    }
}

static void compareRookCannonRowColMoves()
{
    for (Kind kind = ROOK; kind <= CANNON; ++kind)
    {
        bool isCannon = kind == CANNON;
        for (int isCol = 0; isCol < 2; ++isCol)
        {
            int stateTotal = isCol ? COLSTATEMAX : ROWSTATEMAX,
                length = isCol ? BOARDROWNUM : BOARDCOLNUM;
            for (int index = 0; index < length; ++index)
            {
                int *moveMatchs = (isCannon
                                       ? (isCol ? CannonColCanMove[index] : CannonRowCanMove[index])
                                       : (isCol ? RookColCanMove[index] : RookRowCanMove[index]));
                int *moveMatchs2 = (isCannon
                                        ? (isCol ? CannonColCanMove2[index] : CannonRowCanMove2[index])
                                        : (isCol ? RookColCanMove2[index] : RookRowCanMove2[index]));
                // int count = 0;
                int count1 = 0, count2 = 0;
                for (int state = 0; state < stateTotal; ++state)
                {
                    if (moveMatchs[state])
                        ++count1;

                    if (moveMatchs2[state])
                        ++count2;
                }

#ifdef DEBUGCOMPAREMOVES
                char temp[32];
                printf("compareRookCannonCanMove: Format:[count1][count2] %s, %s: %s %3d %3d %s\n\n",
                       isCannon ? "Cannon" : "Rook", isCol ? "Col" : "Row", getRowColBit(temp, INTBITAT(index), isCol),
                       count1, count2, (count1 == count2 ? "" : "Error."));
#endif
            }
        }
    }
}

// int offset = isCol ? COLOFFSET(index) : ROWOFFSET(index);

void initPieceCanMove()
{
    initRookCannonRowColMode();
    initRookCannonRowColMove();

    initRookCannonRowColMove2();

    compareRookCannonRowColMoves();
}