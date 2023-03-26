#include "move.h"
#include "board.h"
#include "data.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define HasPiece(rowOrColInt, index) (rowOrColInt & (1 << index))

#define MODEMAX 32
#define ROWSTATEMAX (1 << BOARDCOLNUM)
#define COLSTATEMAX (1 << BOARDROWNUM)
#define ROWOFFSET(row) (row * BOARDCOLNUM)
#define COLOFFSET(col) (col * BOARDROWNUM)

typedef struct IndexMode
{
    int count;

    int mode[MODEMAX];
    int modeFetch[MODEMAX];
    int modeMatch[MODEMAX];
} IndexMode;

IndexMode RowIndexMode[BOARDCOLNUM];

IndexMode ColIndexMode[BOARDROWNUM];

Board RookRowCanMove[BOARDLENGTH][ROWSTATEMAX];

Board RookColCanMove[BOARDLENGTH][COLSTATEMAX];

Board CannonRowCanMove[BOARDLENGTH][ROWSTATEMAX];

Board CannonColCanMove[BOARDLENGTH][COLSTATEMAX];

static void initIndexMode()
{
    int lengths[] = {BOARDCOLNUM, BOARDROWNUM};
    IndexMode *indexModes[] = {RowIndexMode, ColIndexMode};
    for (int no = 0; no < 2; ++no)
    {
        for (int index = 0; index < lengths[no]; ++index)
        {
            IndexMode indexMode = {};
            int count = 0,
                startMode = 1 << index;
            for (int lowBit = index; lowBit >= 0; --lowBit)
            {
                int lowMode = startMode | (1 << lowBit); // 位重叠
                for (int highBit = index; highBit < lengths[no]; ++highBit)
                {
                    indexMode.mode[count] = lowMode | (1 << highBit); // 高低的界限置位
                    int low = lowBit == index ? 0 : lowBit,
                        high = highBit == index ? lengths[no] - 1 : highBit; // 位重叠时，提取位到底
                    for (int i = low; i <= high; ++i)
                        indexMode.modeFetch[count] |= (1 << i); // 高至低之间范围置位

                    indexMode.modeMatch[count] = indexMode.modeFetch[count] ^ startMode; // 提取范围去掉棋子自身所占位置
                    ++count;
                }
            }
            indexMode.count = count;

            indexModes[no][index] = indexMode;
        }
    }
}

static Board getModeMatchBoard(int state, IndexMode indexMode, int offset)
{
    for (int i = 0; i < indexMode.count; ++i)
        if ((state & indexMode.modeFetch[i]) == indexMode.mode[i])
            return BOARDFROM(indexMode.modeMatch[i], offset);

    return 0;
}

static void initRookRowCanMove()
{
    int index = 0;
    for (int row = 0; row < BOARDROWNUM; ++row)
    {
        int offset = ROWOFFSET(row);
        for (int col = 0; col < BOARDCOLNUM; ++col)
        {
            IndexMode indexMode = RowIndexMode[col];
            for (int state = 0; state < ROWSTATEMAX; ++state)
            {
                if (state & BoardMask[col]) // 棋子在此状态上
                {
                    RookRowCanMove[index][state] = getModeMatchBoard(state, indexMode, offset);
                }
            }

            ++index;
        }
    }
}

static void initRookColCanMove()
{
    int index = 0;
    for (int col = 0; col < BOARDCOLNUM; ++col)
    {
        int offset = COLOFFSET(col);
        for (int row = 0; row < BOARDROWNUM; ++row)
        {
            IndexMode indexMode = ColIndexMode[row];
            for (int state = 0; state < COLSTATEMAX; ++state)
            {
                if (state & BoardMask[row]) // 棋子在此状态上
                {
                    RookColCanMove[index][state] = getModeMatchBoard(state, indexMode, offset); // ?
                }
            }

            ++index;
        }
    }
}

void initPieceCanMove()
{
    initIndexMode();
    initRookRowCanMove();
    initRookColCanMove();
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
void printIndexMode()
{
    int lengths[] = {BOARDCOLNUM, BOARDROWNUM};
    IndexMode *indexModes[] = {RowIndexMode, ColIndexMode};
    for (int no = 0; no < 2; ++no)
    {
        printf("IndexModes[%d]:\n", no);
        for (int index = 0; index < lengths[no]; ++index)
        {
            IndexMode indexMode = indexModes[no][index];
            int count = indexMode.count;
            printf("indexMode[%d]: count: %d\n", index, count);
            for (int i = 0; i < count; ++i)
            {
                if (no == 0)
                {
                    printf(BINARYPATTERN9, BYTEBINARY9(indexMode.mode[i]));
                    printf(BINARYPATTERN9, BYTEBINARY9(indexMode.modeFetch[i]));
                    printf(BINARYPATTERN9, BYTEBINARY9(indexMode.modeMatch[i]));
                }
                else
                {
                    printf(BINARYPATTERN10, BYTEBINARY10(indexMode.mode[i]));
                    printf(BINARYPATTERN10, BYTEBINARY10(indexMode.modeFetch[i]));
                    printf(BINARYPATTERN10, BYTEBINARY10(indexMode.modeMatch[i]));
                }
                printf("\n");
            }
        }
        printf("\n");
    }
}

static void printRookRowCanMove()
{
    printf("printRookRowCanMove:\n");
    char temp[32];
    for (int index = 0; index < BOARDLENGTH; ++index)
    {
        int row = Seats[index].row,
            col = Seats[index].col;
        int offset = ROWOFFSET(row);
        snprintf(temp, 32, BINARYPATTERN9, BYTEBINARY9(1 << col));
        printf("col: %s canMove:\n", temp);
        int count = 0;
        Board *canMove = RookRowCanMove[index];
        for (int state = 0; state < BoardMask[BOARDCOLNUM]; ++state)
        {
            if (canMove[state])
            {
                printf(BINARYPATTERN9, BYTEBINARY9(state));
                printf(BINARYPATTERN9, BYTEBINARY9(canMove[state] >> offset));
                if (count % 5 == 4)
                    printf("\n");
                else
                    printf("| ");

                count++;
            }
        }
        printf("\n\n");
    }
}

static void printRookColCanMove()
{
    printf("printRookColCanMove:\n");
    char temp[32];
    for (int index = 0; index < BOARDLENGTH; ++index)
    {
        int row = Seats[index].row,
            col = Seats[index].col;
        int offset = COLOFFSET(col);
        snprintf(temp, 32, BINARYPATTERN10, BYTEBINARY10(1 << row));
        printf("row: %s canMove:\n", temp);
        int count = 0;
        Board *canMove = RookColCanMove[index];
        for (int state = 0; state < BoardMask[BOARDROWNUM]; ++state)
        {
            if (canMove[state])
            {
                printf(BINARYPATTERN10, BYTEBINARY10(state));
                printf(BINARYPATTERN10, BYTEBINARY10(canMove[state] >> offset));
                if (count % 5 == 4)
                    printf("\n");
                else
                    printf("| ");

                count++;
            }
        }
        printf("\n\n");
    }
}

void printPieceCanMove()
{
    // printRookRowCanMove();
    printRookColCanMove();
}