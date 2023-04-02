#include "move.h"
#include "board.h"
#include "data.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

ChessPosition* doMoveChessPosition(ChessPosition* chess, Color color, Kind kind, int fromIndex, int toIndex)
{
    chess->player = !chess->player;

    // 清除原位置，置位新位置
    Board toBoard = BoardMask[toIndex],
          moveBoard = BoardMask[fromIndex] | toBoard;
    chess->calPieces[color] ^= moveBoard;
    chess->pieces[color][kind] ^= moveBoard;

    // 新位置有对方棋子时
    Color toColor = !color;
    if (chess->calPieces[toColor] & toBoard) {
        chess->calPieces[toColor] ^= toBoard;
        for (Kind toKind = KING; toKind <= PAWN; ++toKind)
            chess->pieces[toColor][toKind] ^= toBoard;
    }

    return updateAllPieces(chess);
}