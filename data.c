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

const char Chars[COLORNUM][KINDNUM] = { "KABNRCP", "kabnrcp" };

Coord Coords[BOARDLENGTH];
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

GetIndexFunc getNonZeroIndex(ChessPosition* chess, Color color)
{
    return color == chess->bottomColor ? getHighNonZeroIndex : getLowNonZeroIndex;
}

int getBoardNonZeroIndexs(int indexs[], Board board)
{
    int count = 0;
    while (board) {
        int index = getLowNonZeroIndex(board);
        indexs[count++] = index;

        board ^= BOARDAT(index);
    }

    return count;
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
            Coords[index] = (Coord) { row, col };
            Rotate[index] = col * BOARDROWNUM + row;
            BoardMask[index] = BOARDAT(index);

            ++index;
        }
    }

#ifdef DEBUGBASEDATA
    char temp[64],
        boardStr[BOARDLENGTH * (BOARDROWNUM + 2) * 16];
    strcpy(boardStr, "Coords[90]:\n");
    for (int i = 0; i < BOARDLENGTH; ++i) {
        snprintf(temp, 64, "coord:(%d, %d), ", Coords[i].row, Coords[i].col);
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
        Coord fromSeat = Coords[index];
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
        Coord fromSeat = Coords[index];
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
        Coord fromSeat = Coords[index];
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
        Coord fromSeat = Coords[index];
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

static int getMatch(int state, int rowColIndex, bool isCannon, bool isRotate)
{
    int match = 0;
    for (int isHigh = 0; isHigh < 2; ++isHigh) {
        int direction = isHigh ? 1 : -1,
            endIndex = isHigh ? (isRotate ? BOARDROWNUM : BOARDCOLNUM) - 1 : 0; // 每行列数或每列行数
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
        for (int isRotate = 0; isRotate < 2; ++isRotate) {
            int stateTotal = isRotate ? COLSTATEMAX : ROWSTATEMAX,
                length = isRotate ? BOARDROWNUM : BOARDCOLNUM;
            for (int rowCloIndex = 0; rowCloIndex < length; ++rowCloIndex) {
#ifdef DEBUGROOKCANNON1
                char temp[32], temp2[32];
                int count = 0;
                printf("printRookCannonCanMove: Format:[state][match] %s, %s: %s\n",
                    isCannon ? "Cannon" : "Rook",
                    isRotate ? "Col" : "Row",
                    getBitStr(temp, INTBITAT(rowCloIndex), isRotate));
#endif

                Board* moveMatchs = (isCannon
                        ? (isRotate ? CannonColMove[rowCloIndex] : CannonRowMove[rowCloIndex])
                        : (isRotate ? RookColMove[rowCloIndex] : RookRowMove[rowCloIndex]));
                for (int state = 0; state < stateTotal; ++state) {
                    // 本状态当前行或列位置无棋子
                    if (!INTBITHAS(state, rowCloIndex))
                        continue;

                    int match = getMatch(state, rowCloIndex, isCannon, isRotate);
                    if (match == 0) {
                        // printf("match==0: %s %s", getBitStr(temp, state, isRotate), getBitStr(temp2, match, isRotate));
                        continue;
                    }

                    if (isRotate) {
                        Board colMatch = 0;
                        for (int row = 0; row < BOARDROWNUM; ++row) {
                            if (match & INTBITAT(row))
                                colMatch |= BoardMask[ROWBASEOFFSET(row)]; // 每行的首列置位
                        }
                        moveMatchs[state] = colMatch;
                    } else
                        moveMatchs[state] = match;
#ifdef DEBUGROOKCANNON1
                    printf("%s %s", getBitStr(temp, state, isRotate), getBitStr(temp2, match, isRotate));
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
                    isRotate ? "Col" : "Row",
                    getBitStr(temp, INTBITAT(rowCloIndex), isRotate));

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
        Coord fromSeat = Coords[index];
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
    Coord fromSeat = Coords[fromIndex];
    int row = fromSeat.row, col = fromSeat.col;
    bool isTop = row == 0 || row == 5,
         isBottom = row == 4 || row == BOARDROWNUM - 1,
         isLeft = col == 0,
         isRight = col == BOARDCOLNUM - 1;
    int state = ((isTop || isLeft || (allPieces & BoardMask[fromIndex - BOARDCOLNUM - 1]) ? INTBITAT(LEGCOUNT - 1) : 0)
        | (isTop || isRight || (allPieces & BoardMask[fromIndex - BOARDCOLNUM + 1]) ? INTBITAT(LEGCOUNT - 2) : 0)
        | (isBottom || isLeft || (allPieces & BoardMask[fromIndex + BOARDCOLNUM - 1]) ? INTBITAT(LEGCOUNT - 3) : 0)
        | (isBottom || isRight || (allPieces & BoardMask[fromIndex + BOARDCOLNUM + 1]) ? INTBITAT(LEGCOUNT - 4) : 0));

    return BishopMove[fromIndex][state];
}

Board getKnightMove(int fromIndex, Board allPieces)
{
    Coord fromSeat = Coords[fromIndex];
    int row = fromSeat.row, col = fromSeat.col;
    int state = ((row == 0 || (allPieces & BoardMask[fromIndex - BOARDCOLNUM]) ? INTBITAT(LEGCOUNT - 1) : 0)
        | (col == 0 || (allPieces & BoardMask[fromIndex - 1]) ? INTBITAT(LEGCOUNT - 2) : 0)
        | (col == BOARDCOLNUM - 1 || (allPieces & BoardMask[fromIndex + 1]) ? INTBITAT(LEGCOUNT - 3) : 0)
        | (row == BOARDROWNUM - 1 || (allPieces & BoardMask[fromIndex + BOARDCOLNUM]) ? INTBITAT(LEGCOUNT - 4) : 0));

    return KnightMove[fromIndex][state];
}

Board getRookMove(int fromIndex, Board allPieces, Board rotatePieces)
{
    Coord fromSeat = Coords[fromIndex];
    int row = fromSeat.row, col = fromSeat.col,
        rowOffset = ROWBASEOFFSET(row);

    return ((RookRowMove[col][(allPieces >> rowOffset) & 0x1FF] << rowOffset)
        | (RookColMove[row][(rotatePieces >> COLBASEOFFSET(col)) & 0x3FF] << col)); // 每行首列置位全体移动数列
}

Board getCannonMove(int fromIndex, Board allPieces, Board rotatePieces)
{
    Coord fromSeat = Coords[fromIndex];
    int row = fromSeat.row, col = fromSeat.col,
        rowOffset = ROWBASEOFFSET(row);

    return ((CannonRowMove[col][(allPieces >> rowOffset) & 0x1FF] << rowOffset)
        | (CannonColMove[row][(rotatePieces >> COLBASEOFFSET(col)) & 0x3FF] << col)); // 每行首列置位全体移动数列
}

void turnColorKindPieces(ChessPosition* chess, Color color, Kind kind, Board turnBoard, Board rotateTurnBoard)
{
    chess->pieces[color][kind] ^= turnBoard;

    chess->calPieces[color] ^= turnBoard;
    chess->calPieces[ALLCOLOR] ^= turnBoard;
    chess->calPieces[ROTATE] ^= rotateTurnBoard;
}

void traverseColorKindPieces(ChessPosition* chess, Color color, Kind kind, GetIndexFunc getIndexFunc, Board board,
    void func(ChessPosition* chess, Color color, Kind kind, int index, void* arg1, void* arg2),
    void* arg1, void* arg2)
{
    while (board) {
        int index = getIndexFunc(board);
        // 执行针对遍历元素的操作函数
        func(chess, color, kind, index, arg1, arg2);

        board ^= BoardMask[index];
    }
}

void traverseColorPieces(ChessPosition* chess, Color color,
    void func(ChessPosition* chess, Color color, Kind kind, int index, void* arg1, void* arg2),
    void* arg1, void* arg2)
{
    GetIndexFunc getIndexFunc = getNonZeroIndex(chess, color);
    for (Kind kind = KING; kind <= PAWN; ++kind)
        traverseColorKindPieces(chess, color, kind, getIndexFunc, chess->pieces[color][kind],
            func, arg1, arg2);
}

void traverseAllColorPieces(ChessPosition* chess,
    void func(ChessPosition* chess, Color color, Kind kind, int index, void* arg1, void* arg2),
    void* arg1, void* arg2)
{
    for (Color color = RED; color <= BLACK; ++color)
        traverseColorPieces(chess, color, func, arg1, arg2);
}

bool isEqual(ChessPosition achess, ChessPosition bchess)
{
    if (achess.player != bchess.player)
        return false;

    for (Color color = RED; color <= BLACK; color++) {
        for (Kind kind = KING; kind < NONKIND; kind++)
            if (achess.pieces[color][kind] != bchess.pieces[color][kind])
                return false;
    }

    if (achess.bottomColor != bchess.bottomColor)
        return false;

    for (Color color = RED; color < NONCOLOR; color++) {
        if (achess.calPieces[color] != bchess.calPieces[color])
            return false;
    }

    return true;
}

static char* getBitStr(char* bitStr, int value, bool isRotate)
{
    if (isRotate)
        snprintf(bitStr, 64, BINARYPATTERN10, BYTEBINARY10(value));
    else
        snprintf(bitStr, 64, BINARYPATTERN9, BYTEBINARY9(value));

    return bitStr;
}

static void getBoardStr(char boardStr[][16], Board board, bool isRotate)
{
    int rowNum = isRotate ? BOARDCOLNUM : BOARDROWNUM,
        mode = isRotate ? 0x3FF : 0x1FF;
    for (int row = 0; row < rowNum; ++row)
        getBitStr(boardStr[row], (board >> (isRotate ? COLBASEOFFSET(row) : ROWBASEOFFSET(row))) & mode, isRotate);
}

char* getBoardArrayStr(char* boardArrayStr, const Board* boards, int length, int colNum, bool showZero, bool isRotate)
{
    if (length < colNum)
        colNum = length;

    char temp[64],
        indexRowStr[colNum * 16],
        nullRowStr[colNum * 16];
    strcpy(nullRowStr, "   ");
    for (int col = 0; col < colNum; ++col)
        strcat(nullRowStr, isRotate ? "ABCDEFGHIJ " : "ABCDEFGHI ");
    strcat(nullRowStr, "\n");

    strcpy(boardArrayStr, "");
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
        char boardStr[colNum][BOARDROWNUM][16];
        strcpy(indexRowStr, "   ");
        for (int col = 0; col < colNum && index + col < length; ++col) {
            snprintf(temp, 16, "%02d(%d,%d):  ", index + col, (index + col) / colNum, col);
            strcat(indexRowStr, temp);

            getBoardStr(boardStr[col], boards[index + col], isRotate);
        }
        strcat(indexRowStr, "\n");
        strcat(boardArrayStr, indexRowStr);
        strcat(boardArrayStr, nullRowStr);

        int totalRow = isRotate ? BOARDCOLNUM : BOARDROWNUM;
        for (int row = 0; row < totalRow; ++row) {
            snprintf(temp, 16, "%d: ", row);
            strcat(boardArrayStr, temp);
            for (int col = 0; col < colNum && index + col < length; ++col)
                strcat(boardArrayStr, boardStr[col][row]);

            strcat(boardArrayStr, "\n");
        }
    }
    snprintf(temp, 32, "boardCount: %d\n", length);
    strcat(boardArrayStr, temp);

    return boardArrayStr;
}

char* getMoveArrayStr(char* moveArrayStr, const Move* moves, int length, int colNum)
{
    if (length < colNum)
        colNum = length;

    char temp[64],
        nullRowStr[colNum * 16];
    strcpy(nullRowStr, "   ");
    for (int col = 0; col < colNum; ++col)
        strcat(nullRowStr, "ABCDEFGHI ");
    strcat(nullRowStr, "\n");

    strcpy(moveArrayStr, "");
    for (int index = 0; index < length; index += colNum) {
        char boardStr[colNum][BOARDROWNUM][16];
        strcat(moveArrayStr, "   ");
        for (int col = 0; col < colNum && index + col < length; ++col) {
            Move move = moves[index + col];
            snprintf(temp, 64, "%c %02d      ", Chars[move.color][move.kind], move.index);
            strcat(moveArrayStr, temp);

            getBoardStr(boardStr[col], move.moveTo, false);
        }
        strcat(moveArrayStr, "\n");
        strcat(moveArrayStr, nullRowStr);

        for (int row = 0; row < BOARDROWNUM; ++row) {
            snprintf(temp, 16, "%d: ", row);
            strcat(moveArrayStr, temp);
            for (int col = 0; col < colNum && index + col < length; ++col)
                strcat(moveArrayStr, boardStr[col][row]);

            strcat(moveArrayStr, "\n");
        }
    }

    for (int index = 0; index < length; ++index) {
        Move move = moves[index];
        Coord coord = Coords[move.index];
        snprintf(temp, 64, "[%c] from:(%02d,%02d) to:", Chars[move.color][move.kind], coord.row, coord.col);
        strcat(moveArrayStr, temp);

        int indexs[BOARDROWNUM + BOARDCOLNUM];
        int count = getBoardNonZeroIndexs(indexs, move.moveTo);
        for (int i = 0; i < count; ++i) {
            Coord coord = Coords[indexs[i]];
            snprintf(temp, 64, "(%02d,%02d) ", coord.row, coord.col);
            strcat(moveArrayStr, temp);
        }

        strcat(moveArrayStr, "\n");
    }

    return moveArrayStr;
}

static void setBoardNames(ChessPosition* chess, Color color, Kind kind, int index, void* boardStr, void* arg2)
{
    Coord coord = Coords[index];
    ((char*)boardStr)[coord.row * (BOARDCOLNUM + 1) + coord.col] = Chars[color][kind];
}

char* getChessPositionStr(char* chessStr, ChessPosition* chess)
{
    static const char* BoardStr = "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n"
                                  "---------\n";

    const char* colorStrs[] = { "RED", "BLACK" };
    char temp[KINDNUM * (BOARDROWNUM + 2) * 16];
    snprintf(temp, 128, "player: %s\n", colorStrs[chess->player]);
    strcpy(chessStr, temp);
    for (Color color = RED; color <= BLACK; ++color) {
        snprintf(temp, 32, "chess->pieces[%s][Kind]:\n", colorStrs[!color]);
        strcat(chessStr, temp);

        getBoardArrayStr(temp, chess->pieces[!color], KINDNUM, KINDNUM, true, false);
        strcat(chessStr, temp);
    }

    snprintf(temp, 128, "calPieces[Color]: \nRED:         BLACK:    ALLCOLOR:\n");
    strcat(chessStr, temp);
    getBoardArrayStr(temp, chess->calPieces, ALLCOLOR + 1, KINDNUM, true, false);
    strcat(chessStr, temp);

    strcat(chessStr, "ROTATE: \n");
    getBoardArrayStr(temp, &chess->calPieces[ROTATE], 1, KINDNUM, true, true);
    strcat(chessStr, temp);

    strcat(chessStr, "chessBoardStr:\n");
    strcpy(temp, BoardStr);
    traverseAllColorPieces(chess, setBoardNames, temp, NULL);
    strcat(chessStr, temp);

    return chessStr;
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
