#include "data.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// #define DEBUGBASEDATA
// #define DEBUGKING
// #define DEBUGADVISOR
// #define DEBUGBISHOP
// #define DEBUGKNIGHT
// #define DEBUGROOKCANNON1
// #define DEBUGROOKCANNON2
// #define DEBUGPAWN

Seat Seats[BOARDLENGTH];
int Rotate[BOARDLENGTH];
Board BoardMask[BOARDLENGTH];

// 帅仕根据所处的位置选取可移动位棋盘
Board KingMove[BOARDLENGTH];
Board AdvisorMove[BOARDLENGTH];

// 马相根据憋马腿或田心组成的四个位置状态选取可移动位棋盘
static Board BishopMove[BOARDLENGTH][INTBITAT(LEGCOUNT)];
static Board KnightMove[BOARDLENGTH][INTBITAT(LEGCOUNT)];

// 车炮根据每行和每列的位置状态选取可移动位棋盘
static Board RookRowMove[BOARDCOLNUM][ROWSTATEMAX];
static Board RookColMove[BOARDROWNUM][COLSTATEMAX];
static Board CannonRowMove[BOARDCOLNUM][ROWSTATEMAX];
static Board CannonColMove[BOARDROWNUM][COLSTATEMAX];

// 兵根据本方处于上或下的二个位置状态选取可移动位棋盘
Board PawnMove[BOARDLENGTH][2];

// 通过二分搜索计算右侧的连续零位（尾随）
unsigned int getLowNonZeroIndexFromUInt(unsigned int value)
{
    // unsigned int v; // 32-bit word input to count zero bits on right
    unsigned int index; // c will be the number of zero bits on the right,
    //                 // so if v is 1101000 (base 2), then c will be 3
    // // NOTE: if 0 == v, then c = 31.
    if (value & 0x1) {
        // special case for odd v (assumed to happen half of the time)
        index = 0;
    } else {
        index = 1;
        if ((value & 0xffff) == 0) {
            value >>= 16;
            index += 16;
        }
        if ((value & 0xff) == 0) {
            value >>= 8;
            index += 8;
        }
        if ((value & 0xf) == 0) {
            value >>= 4;
            index += 4;
        }
        if ((value & 0x3) == 0) {
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
unsigned int getLowNonZeroIndexFromRowCol(unsigned int value)
{
    int index = 16 - 1;
    if (value & 0X00FF) {
        index -= 8;
        value &= 0X00FF;
    }

    if (value & 0X0F0F) {
        index -= 4;
        value &= 0X0F0F;
    }

    if (value & 0X3333) {
        index -= 2;
        value &= 0X3333;
    }

    if (value & 0X5555)
        index -= 1;

    return index;
}

int getLowNonZeroIndexs(int indexs[], int value)
{
    int count = 0;
    while (value) {
        int index = getLowNonZeroIndexFromUInt(value);
        indexs[count++] = index;
        value ^= INTBITAT(index);
    }

    return count;
}

// 求最低位非零位的序号，调用前判断参数非零
int getLowNonZeroIndex(Board board)
{
    int index = BOARDBITSIZE - 1;
    uint64_t value = board & 0XFFFFFFFFFFFFFFFFUL; // 00-63 位
    if (value)
        index -= 64;
    else
        value = board >> 64; // 64-89 位

    if (value & 0X00000000FFFFFFFFUL) {
        index -= 32;
        value &= 0X00000000FFFFFFFFUL;
    }

    if (value & 0X0000FFFF0000FFFFUL) {
        index -= 16;
        value &= 0X0000FFFF0000FFFFUL;
    }

    if (value & 0X00FF00FF00FF00FFUL) {
        index -= 8;
        value &= 0X00FF00FF00FF00FFUL;
    }

    if (value & 0X0F0F0F0F0F0F0F0FUL) {
        index -= 4;
        value &= 0X0F0F0F0F0F0F0F0FUL;
    }

    if (value & 0X3333333333333333UL) {
        index -= 2;
        value &= 0X3333333333333333UL;
    }

    if (value & 0X5555555555555555UL)
        index -= 1;

    return index;
}

// 求最高位非零位的序号，调用前判断参数非全零位
int getHighNonZeroIndex(Board board)
{
    int index = 0;
    uint64_t value = board >> 64; // 64-89 位
    if (value)
        index += 64;
    else
        value = board & 0XFFFFFFFFFFFFFFFFUL; // 00-63 位

    if (value & 0XFFFFFFFF00000000UL) {
        index += 32;
        value &= 0XFFFFFFFF00000000UL;
    }

    if (value & 0XFFFF0000FFFF0000UL) {
        index += 16;
        value &= 0XFFFF0000FFFF0000UL;
    }

    if (value & 0XFF00FF00FF00FF00UL) {
        index += 8;
        value &= 0XFF00FF00FF00FF00UL;
    }

    if (value & 0XF0F0F0F0F0F0F0F0UL) {
        index += 4;
        value &= 0XF0F0F0F0F0F0F0F0UL;
    }

    if (value & 0XCCCCCCCCCCCCCCCCUL) {
        index += 2;
        value &= 0XCCCCCCCCCCCCCCCCUL;
    }

    if (value & 0XAAAAAAAAAAAAAAAAUL)
        index += 1;

    return index;
}

static bool isValidKing(int row, int col)
{
    return (row < 3 || row > 6) && (col > 2 && col < 6);
}

static bool isValidAdvisor(int row, int col)
{
    return (((row == 0 || row == 2 || row == 7 || row == 9) && (col == 3 || col == 5))
        || ((row == 1 || row == 8) && col == 4));
}

static bool isValidBishop(int row, int col)
{
    return (((row == 0 || row == 4 || row == 5 || row == 9) && (col == 2 || col == 6))
        || ((row == 2 || row == 7) && (col == 0 || col == 4 || col == 8)));
}

static bool isValidPawn(int row, int col, bool isBottom)
{
    return (isBottom ? (row < 5 || ((row == 5 || row == 6) && (col == 0 || col == 2 || col == 4 || col == 6 || col == 8)))
                     : (row > 4 || ((row == 3 || row == 4) && (col == 0 || col == 2 || col == 4 || col == 6 || col == 8))));
}

static void initBaseData()
{
    int index = 0;
    for (int row = 0; row < BOARDROWNUM; ++row) {
        for (int col = 0; col < BOARDCOLNUM; ++col) {
            Seats[index] = (Seat) { row, col };
            Rotate[index] = col * BOARDROWNUM + row;
            BoardMask[index] = BOARDAT(index);

            ++index;
        }
    }

#ifdef DEBUGBASEDATA
    char temp[64],
        boardStr[BOARDLENGTH * (BOARDROWNUM + 2) * 16];
    strcpy(boardStr, "Seats[90]:\n");
    for (int i = 0; i < BOARDLENGTH; ++i) {
        snprintf(temp, 64, "seat:(%d, %d), ", Seats[i].row, Seats[i].col);
        strcat(boardStr, temp);
        if (i % BOARDCOLNUM == BOARDCOLNUM - 1)
            strcat(boardStr, "\n");
    }

    strcat(boardStr, "\nRotate[90]:\n");
    for (int i = 0; i < BOARDLENGTH; ++i) {
        snprintf(temp, 64, "%3d", Rotate[i]);
        strcat(boardStr, temp);
        if (i % BOARDROWNUM == BOARDROWNUM - 1)
            strcat(boardStr, "\n");
    }
    printf("%s\n", boardStr);

    getBoardArrayStr(boardStr, BoardMask, BOARDLENGTH, BOARDCOLNUM, true, false);
    printf("testBoardMask:\n%s\n", boardStr);
#endif
}

static void initKingMove()
{
    for (int index = 0; index < BOARDLENGTH; ++index) {
        Seat fromSeat = Seats[index];
        int row = fromSeat.row, col = fromSeat.col;
        if (!isValidKing(row, col))
            continue;

        Board match = 0;
        if (col > 3)
            match |= BoardMask[index - 1];
        if (col < 5)
            match |= BoardMask[index + 1];
        if (row == 0 || row == 1 || row == 7 || row == 8)
            match |= BoardMask[index + BOARDCOLNUM];
        if (row == 1 || row == 2 || row == 8 || row == 9)
            match |= BoardMask[index - BOARDCOLNUM];

        KingMove[index] = match;
    }
#ifdef DEBUGKING
    printf("printKingMove:\n");

    char boardStr[BOARDLENGTH * (BOARDROWNUM + 2) * 16];
    getBoardArrayStr(boardStr, KingMove, BOARDLENGTH, 9, false, false);
    printf("%s\n", boardStr);
#endif
}

static void initAdvisorMove()
{
    for (int index = 0; index < BOARDLENGTH; ++index) {
        Seat fromSeat = Seats[index];
        int row = fromSeat.row, col = fromSeat.col;
        if (!isValidAdvisor(row, col))
            continue;

        Board match;
        if (col == 4)
            match = (BoardMask[index - BOARDCOLNUM - 1]
                | BoardMask[index - BOARDCOLNUM + 1]
                | BoardMask[index + BOARDCOLNUM - 1]
                | BoardMask[index + BOARDCOLNUM + 1]);
        else
            match = BoardMask[row < 3 ? 13 : 76];

        AdvisorMove[index] = match;
    }
#ifdef DEBUGADVISOR
    printf("printAdvisorMove:\n");

    char boardStr[BOARDLENGTH * (BOARDROWNUM + 2) * 16];
    getBoardArrayStr(boardStr, AdvisorMove, BOARDLENGTH, 5, false, false);
    printf("%s\n", boardStr);
#endif
}

static void initBishopMove()
{
    for (int index = 0; index < BOARDLENGTH; ++index) {
        Seat fromSeat = Seats[index];
        int row = fromSeat.row, col = fromSeat.col;
        if (!isValidBishop(row, col))
            continue;

        for (int state = 0; state < INTBITAT(LEGCOUNT); ++state) {
            int realState = state;
            if (row == 0 || row == 5)
                realState |= (INTBITAT(LEGCOUNT - 1) | INTBITAT(LEGCOUNT - 2));
            else if (row == 4 || row == BOARDROWNUM - 1)
                realState |= (INTBITAT(LEGCOUNT - 3) | INTBITAT(LEGCOUNT - 4));
            if (col == 0)
                realState |= (INTBITAT(LEGCOUNT - 1) | INTBITAT(LEGCOUNT - 3));
            else if (col == BOARDCOLNUM - 1)
                realState |= (INTBITAT(LEGCOUNT - 2) | INTBITAT(LEGCOUNT - 4));

            Board match = 0;
            if (!(realState & INTBITAT(LEGCOUNT - 1)))
                match |= BoardMask[index - 2 * BOARDCOLNUM - 2];

            if (!(realState & INTBITAT(LEGCOUNT - 2)))
                match |= BoardMask[index - 2 * BOARDCOLNUM + 2];

            if (!(realState & INTBITAT(LEGCOUNT - 3)))
                match |= BoardMask[index + 2 * BOARDCOLNUM - 2];

            if (!(realState & INTBITAT(LEGCOUNT - 4)))
                match |= BoardMask[index + 2 * BOARDCOLNUM + 2];

            BishopMove[index][state] = match;
        }

#ifdef DEBUGBISHOP
        printf("printBishopMove: fromIndex:%2d (%2d,%2d)\n", index, row, col);

        char boardStr[INTBITAT(LEGCOUNT) * (BOARDROWNUM + 2) * 16];
        getBoardArrayStr(boardStr, BishopMove[index], INTBITAT(LEGCOUNT), INTBITAT(LEGCOUNT - 1), true, false);
        printf("%s\n", boardStr);
#endif
    }
}

static void initKninghtCanMove()
{
    for (int index = 0; index < BOARDLENGTH; ++index) {
        Seat fromSeat = Seats[index];
        int row = fromSeat.row, col = fromSeat.col;
        for (int state = 0; state < INTBITAT(LEGCOUNT); ++state) {
            int realState = state;
            if (row == 0)
                realState |= INTBITAT(LEGCOUNT - 1);
            else if (row == BOARDROWNUM - 1)
                realState |= INTBITAT(LEGCOUNT - 4);
            if (col == 0)
                realState |= INTBITAT(LEGCOUNT - 2);
            else if (col == BOARDCOLNUM - 1)
                realState |= INTBITAT(LEGCOUNT - 3);

            Board match = 0;
            if (!(realState & INTBITAT(LEGCOUNT - 1)) && row > 1) {
                if (col > 0)
                    match |= BoardMask[index - 2 * BOARDCOLNUM - 1];
                if (col < BOARDCOLNUM - 1)
                    match |= BoardMask[index - 2 * BOARDCOLNUM + 1];
            }
            if (!(realState & INTBITAT(LEGCOUNT - 2)) && col > 1) {
                if (row > 0)
                    match |= BoardMask[index - 2 - BOARDCOLNUM];
                if (row < BOARDROWNUM - 1)
                    match |= BoardMask[index - 2 + BOARDCOLNUM];
            }
            if (!(realState & INTBITAT(LEGCOUNT - 3)) && col < BOARDCOLNUM - 2) {
                if (row > 0)
                    match |= BoardMask[index + 2 - BOARDCOLNUM];
                if (row < BOARDROWNUM - 1)
                    match |= BoardMask[index + 2 + BOARDCOLNUM];
            }
            if (!(realState & INTBITAT(LEGCOUNT - 4)) && row < BOARDROWNUM - 2) {
                if (col > 0)
                    match |= BoardMask[index + 2 * BOARDCOLNUM - 1];
                if (col < BOARDCOLNUM - 1)
                    match |= BoardMask[index + 2 * BOARDCOLNUM + 1];
            }

            KnightMove[index][state] = match;
        }

#ifdef DEBUGKNIGHT
        printf("printKnightMove: fromIndex:%2d (%2d,%2d)\n", index, row, col);

        char boardStr[INTBITAT(LEGCOUNT) * (BOARDROWNUM + 2) * 16];
        getBoardArrayStr(boardStr, KnightMove[index], INTBITAT(LEGCOUNT), INTBITAT(LEGCOUNT - 1), true, false);
        printf("%s\n", boardStr);
#endif
    }
}

static int getMatch(int state, int rowColIndex, bool isCannon, bool isCol)
{
    int match = 0;
    for (int isHigh = 0; isHigh < 2; ++isHigh) {
        int direction = isHigh ? 1 : -1,
            endIndex = isHigh ? (isCol ? BOARDROWNUM : BOARDCOLNUM) - 1 : 0; // 每行列数或每列行数
        bool skip = false; // 炮是否已跳
        for (int i = direction * (rowColIndex + direction); i <= endIndex; ++i) {
            int index = direction * i;
            bool hasPiece = INTBITHAS(state, index);
            if (isCannon) {
                if (!skip) {
                    if (hasPiece)
                        skip = true;
                    else
                        match |= INTBITAT(index);
                } else if (hasPiece) {
                    match |= INTBITAT(index);
                    break;
                }
            } else {
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
    for (Kind kind = ROOK; kind <= CANNON; ++kind) {
        bool isCannon = kind == CANNON;
        for (int isCol = 0; isCol < 2; ++isCol) {
            int stateTotal = isCol ? COLSTATEMAX : ROWSTATEMAX,
                length = isCol ? BOARDROWNUM : BOARDCOLNUM;
            for (int rowCloIndex = 0; rowCloIndex < length; ++rowCloIndex) {
#ifdef DEBUGROOKCANNON1
                char temp[32], temp2[32];
                int count = 0;
                printf("printRookCannonCanMove: Format:[state][match] %s, %s: %s\n",
                    isCannon ? "Cannon" : "Rook",
                    isCol ? "Col" : "Row",
                    getBitStr(temp, INTBITAT(rowCloIndex), isCol));
#endif

                Board* moveMatchs = (isCannon
                        ? (isCol ? CannonColMove[rowCloIndex] : CannonRowMove[rowCloIndex])
                        : (isCol ? RookColMove[rowCloIndex] : RookRowMove[rowCloIndex]));
                for (int state = 0; state < stateTotal; ++state) {
                    // 本状态当前行或列位置无棋子
                    if (!INTBITHAS(state, rowCloIndex))
                        continue;

                    int match = getMatch(state, rowCloIndex, isCannon, isCol);
                    if (match == 0) {
                        // printf("match==0: %s %s", getBitStr(temp, state, isCol), getBitStr(temp2, match, isCol));
                        continue;
                    }

                    if (isCol) {
                        Board colMatch = 0;
                        for (int row = 0; row < BOARDROWNUM; ++row) {
                            if (match & INTBITAT(row))
                                colMatch |= BoardMask[ROWBASEOFFSET(row)]; // 每行的首列置位
                        }
                        moveMatchs[state] = colMatch;
                    } else
                        moveMatchs[state] = match;
#ifdef DEBUGROOKCANNON1
                    printf("%s %s", getBitStr(temp, state, isCol), getBitStr(temp2, match, isCol));
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
                    getBitStr(temp, INTBITAT(rowCloIndex), isCol));

                char boardStr[stateTotal * (BOARDROWNUM + 2) * 16];
                getBoardArrayStr(boardStr, moveMatchs, stateTotal, BOARDCOLNUM, false, false);
                printf("%s\n", boardStr);
#endif
            }
        }
    }
}

static void initPawnMove()
{
    for (int index = 0; index < BOARDLENGTH; ++index) {
        Seat fromSeat = Seats[index];
        int row = fromSeat.row, col = fromSeat.col;
        for (int isBottom = 0; isBottom < 2; ++isBottom) {

            if (!isValidPawn(row, col, isBottom))
                continue;

            Board match = 0;
            if ((isBottom == 0 && row > 4) || (isBottom == 1 && row < 5)) {
                if (col != 0)
                    match |= BoardMask[index - 1];
                if (col != BOARDCOLNUM - 1)
                    match |= BoardMask[index + 1];
            }
            if (isBottom == 0 && row != BOARDROWNUM - 1)
                match |= BoardMask[index + BOARDCOLNUM];
            if (isBottom == 1 && row != 0)
                match |= BoardMask[index - BOARDCOLNUM];

            PawnMove[index][isBottom] = match;
        }

#ifdef DEBUGPAWN
        printf("printPawnMove: fromIndex:%2d (%2d,%2d)\n", index, row, col);

        char boardStr[2 * (BOARDROWNUM + 2) * 16];
        getBoardArrayStr(boardStr, PawnMove[index], 2, 2, true, false);
        printf("%s\n", boardStr);
#endif
    }
}

Board getBishopMove(int fromIndex, Board allPieces)
{
    Seat fromSeat = Seats[fromIndex];
    int row = fromSeat.row, col = fromSeat.col;
    bool isNonTop = row != 0 && row != 5,
         isNonBottom = row != 4 && row != BOARDROWNUM - 1,
         isNonLeft = col > 0,
         isNonRight = col < BOARDCOLNUM - 1;
    int state = ((isNonTop && isNonLeft && !(allPieces & BoardMask[fromIndex - BOARDCOLNUM - 1]) ? 0 : INTBITAT(LEGCOUNT - 1))
        | (isNonTop && isNonRight && !(allPieces & BoardMask[fromIndex - BOARDCOLNUM + 1]) ? 0 : INTBITAT(LEGCOUNT - 2))
        | (isNonBottom && isNonLeft && !(allPieces & BoardMask[fromIndex + BOARDCOLNUM - 1]) ? 0 : INTBITAT(LEGCOUNT - 3))
        | (isNonBottom && isNonRight && !(allPieces & BoardMask[fromIndex + BOARDCOLNUM + 1]) ? 0 : INTBITAT(LEGCOUNT - 4)));

    return BishopMove[fromIndex][state];
}

Board getKnightMove(int fromIndex, Board allPieces)
{
    Seat fromSeat = Seats[fromIndex];
    int row = fromSeat.row, col = fromSeat.col;
    int state = ((row > 0 && !(allPieces & BoardMask[fromIndex - BOARDCOLNUM]) ? 0 : INTBITAT(LEGCOUNT - 1))
        | (col > 0 && !(allPieces & BoardMask[fromIndex - 1]) ? 0 : INTBITAT(LEGCOUNT - 2))
        | (col < BOARDCOLNUM - 1 && !(allPieces & BoardMask[fromIndex + 1]) ? 0 : INTBITAT(LEGCOUNT - 3))
        | (row < BOARDROWNUM - 1 && !(allPieces & BoardMask[fromIndex + BOARDCOLNUM]) ? 0 : INTBITAT(LEGCOUNT - 4)));

    return KnightMove[fromIndex][state];
}

Board getRookMove(int fromIndex, Board allPieces, Board rotatePieces)
{
    Seat fromSeat = Seats[fromIndex];
    int row = fromSeat.row, col = fromSeat.col,
        rowOffset = ROWBASEOFFSET(row);

    return ((RookRowMove[col][(allPieces >> rowOffset) & 0x1FF] << rowOffset)
        | (RookColMove[row][(rotatePieces >> COLBASEOFFSET(col)) & 0x3FF] << col)); // 每行首列置位全体移动数列
}

Board getCannonMove(int fromIndex, Board allPieces, Board rotatePieces)
{
    Seat fromSeat = Seats[fromIndex];
    int row = fromSeat.row, col = fromSeat.col,
        rowOffset = ROWBASEOFFSET(row);

    return ((CannonRowMove[col][(allPieces >> rowOffset) & 0x1FF] << rowOffset)
        | (CannonColMove[row][(rotatePieces >> COLBASEOFFSET(col)) & 0x3FF] << col)); // 每行首列置位全体移动数列
}

char* getBitStr(char* bitStr, int value, bool isCol)
{
    if (isCol)
        snprintf(bitStr, 64, BINARYPATTERN10, BYTEBINARY10(value));
    else
        snprintf(bitStr, 64, BINARYPATTERN9, BYTEBINARY9(value));

    return bitStr;
}

char* getBoardStr(char* boardStr, Board board)
{
    char temp[128], temp2[64];
    strcpy(boardStr, "   ABCDEFGHI\n");
    for (int row = 0; row < BOARDROWNUM; ++row) {
        snprintf(temp, 128, "%d: %s\n",
            row, getBitStr(temp2, (board >> ROWBASEOFFSET(row)) & 0x1FF, false));
        strcat(boardStr, temp);
    }

    return boardStr;
}

char* getBoardArrayStr(char* boardStr, const Board* boards, int length, int colNum, bool showZero, bool isCol)
{
    if (length < colNum)
        colNum = length;

    char temp[64],
        indexRowStr[colNum * 16],
        nullRowStr[colNum * 16];
    strcpy(nullRowStr, "  ");
    for (int col = 0; col < colNum; ++col)
        strcat(nullRowStr, isCol ? " ABCDEFGHIJ" : " ABCDEFGHI");
    strcat(nullRowStr, "\n");

    strcpy(boardStr, "");
    Board nonZeroBoards[length];
    if (!showZero) {
        int count = 0;
        for (int index = 0; index < length; ++index) {
            if (boards[index])
                nonZeroBoards[count++] = boards[index];
        }
        boards = nonZeroBoards;
        length = count;
    }
    for (int index = 0; index < length; index += colNum) {
        strcpy(indexRowStr, "   ");
        for (int col = 0; col < colNum; ++col) {
            snprintf(temp, 16, "%02d(%d,%d):  ", index + col, (index + col) / colNum, col);
            strcat(indexRowStr, temp);
        }
        strcat(indexRowStr, "\n");
        strcat(boardStr, indexRowStr);
        strcat(boardStr, nullRowStr);

        int totalRow = isCol ? BOARDCOLNUM : BOARDROWNUM,
            totalCol = !isCol ? BOARDCOLNUM : BOARDROWNUM,
            mode = !isCol ? 0x1FF : 0x3FF;
        for (int row = 0; row < totalRow; ++row) {
            snprintf(temp, 16, "%d: ", row);
            strcat(boardStr, temp);
            for (int col = 0; col < colNum && index + col < length; ++col) {
                int rowOrCol = (boards[index + col] >> (row * totalCol)) & mode;
                getBitStr(temp, rowOrCol, isCol);
                strcat(boardStr, temp);
            }
            strcat(boardStr, "\n");
        }
        // strcat(boardStr, "\n");
    }
    snprintf(temp, 32, "count: %d\n", length);
    strcat(boardStr, temp);

    return boardStr;
}

char* getMoveBoardsStr(char* moveStr, const MoveBoard* moveBoards, int count)
{
    strcpy(moveStr, "");
    char temp[1024],
        boardStr[512];
    for (int index = 0; index < count; ++index) {
        MoveBoard moveBoard = moveBoards[index];
        snprintf(temp, 1024, "kind: %d fromIndex:%2d movtTo:\n%s",
            moveBoard.kind, moveBoard.fromIndex,
            getBoardStr(boardStr, moveBoard.moveTo));

        strcat(moveStr, temp);
    }

    return moveStr;
}

void initData()
{
    initBaseData();

    initKingMove();
    initAdvisorMove();
    initBishopMove();
    initKninghtCanMove();
    initRookCannonCanMove();
    initPawnMove();
}
