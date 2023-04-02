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

#define LEGCOUNT 4
#define ROWSTATEMAX (1 << BOARDCOLNUM)
#define COLSTATEMAX (1 << BOARDROWNUM)
#define ROWBASEOFFSET(row) ((row)*BOARDCOLNUM)
#define COLBASEOFFSET(col) ((col)*BOARDROWNUM)
#define INDEXFROMROWCOL(row, col) ((row)*BOARDCOLNUM + (col))

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

    getBoardStr(boardStr, BoardMask, BOARDLENGTH, BOARDCOLNUM, true, false);
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
    getBoardStr(boardStr, KingMove, BOARDLENGTH, 9, false, false);
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
            match = (BoardMask[INDEXFROMROWCOL(row - 1, col - 1)]
                | BoardMask[INDEXFROMROWCOL(row - 1, col + 1)]
                | BoardMask[INDEXFROMROWCOL(row + 1, col - 1)]
                | BoardMask[INDEXFROMROWCOL(row + 1, col + 1)]);
        else
            match = BoardMask[INDEXFROMROWCOL(row + (row == 0 || row == 7 ? 1 : -1), 4)];

        AdvisorMove[index] = match;
    }
#ifdef DEBUGADVISOR
    printf("printAdvisorMove:\n");

    char boardStr[BOARDLENGTH * (BOARDROWNUM + 2) * 16];
    getBoardStr(boardStr, AdvisorMove, BOARDLENGTH, 5, false, false);
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
        getBoardStr(boardStr, BishopMove[index], INTBITAT(LEGCOUNT), INTBITAT(LEGCOUNT - 1), true, false);
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
        getBoardStr(boardStr, KnightMove[index], INTBITAT(LEGCOUNT), INTBITAT(LEGCOUNT - 1), true, false);
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
                    getIntBitStr(temp, INTBITAT(rowCloIndex), isCol));
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
                        // printf("match==0: %s %s", getIntBitStr(temp, state, isCol), getIntBitStr(temp2, match, isCol));
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
                    printf("%s %s", getIntBitStr(temp, state, isCol), getIntBitStr(temp2, match, isCol));
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
                    getIntBitStr(temp, INTBITAT(rowCloIndex), isCol));

                char boardStr[stateTotal * (BOARDROWNUM + 2) * 16];
                getBoardStr(boardStr, moveMatchs, stateTotal, BOARDCOLNUM, false, false);
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
        getBoardStr(boardStr, PawnMove[index], 2, 2, true, false);
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
    int state = (((isNonTop && isNonLeft ? allPieces & BoardMask[fromIndex - BOARDCOLNUM - 1] : 0x1) << (LEGCOUNT - 1))
        | ((isNonTop && isNonRight ? allPieces & BoardMask[fromIndex - BOARDCOLNUM + 1] : 0x1) << (LEGCOUNT - 2))
        | ((isNonBottom && isNonLeft ? allPieces & BoardMask[fromIndex + BOARDCOLNUM - 1] : 0x1) << (LEGCOUNT - 3))
        | ((isNonBottom && isNonRight ? allPieces & BoardMask[fromIndex + BOARDCOLNUM + 1] : 0x1) << (LEGCOUNT - 4)));

    return BishopMove[fromIndex][state];
}

Board getKnightMove(int fromIndex, Board allPieces)
{
    Seat fromSeat = Seats[fromIndex];
    int row = fromSeat.row, col = fromSeat.col;
    int state = (((row > 0 ? allPieces & BoardMask[fromIndex - BOARDCOLNUM] : 0x1) << (LEGCOUNT - 1))
        | ((col > 0 ? allPieces & BoardMask[fromIndex - 1] : 0x1) << (LEGCOUNT - 2))
        | ((col < BOARDCOLNUM - 1 ? allPieces & BoardMask[fromIndex + 1] : 0x1) << (LEGCOUNT - 3))
        | ((row < BOARDROWNUM - 1 ? allPieces & BoardMask[fromIndex + BOARDCOLNUM] : 0x1) << (LEGCOUNT - 4)));

    return KnightMove[fromIndex][state];
}

Board getRookCannonMove(bool isCannon, int fromIndex, Board allPieces, Board rotatePieces)
{
    Seat fromSeat = Seats[fromIndex];
    int row = fromSeat.row, col = fromSeat.col,
        rowOffset = ROWBASEOFFSET(row);
    Board(*rowCanMove)[ROWSTATEMAX] = isCannon ? CannonRowMove : RookRowMove;
    Board(*colCanMove)[COLSTATEMAX] = isCannon ? CannonColMove : RookColMove;

    return ((rowCanMove[col][(allPieces >> rowOffset) & 0x1FF] << rowOffset)
        | (colCanMove[row][(rotatePieces >> COLBASEOFFSET(col)) & 0x3FF] << col)); // 每行首列置位全体移动数列
}

char* getIntBitStr(char* bitStr, int value, bool isCol)
{
#define BINARYPATTERN9 "%c%c%c%c%c%c%c%c%c "
#define BINARYPATTERN10 "%c%c%c%c%c%c%c%c%c%c "
#define BYTEBINARY9(i)            \
    (((i)&0x01) ? '1' : '-'),     \
        (((i)&0x02) ? '1' : '-'), \
        (((i)&0x04) ? '1' : '-'), \
        (((i)&0x08) ? '1' : '-'), \
        (((i)&0x10) ? '1' : '-'), \
        (((i)&0x20) ? '1' : '-'), \
        (((i)&0x40) ? '1' : '-'), \
        (((i)&0x80) ? '1' : '-'), \
        (((i)&0x100) ? '1' : '-')
#define BYTEBINARY10(i)            \
    (((i)&0x01) ? '1' : '-'),      \
        (((i)&0x02) ? '1' : '-'),  \
        (((i)&0x04) ? '1' : '-'),  \
        (((i)&0x08) ? '1' : '-'),  \
        (((i)&0x10) ? '1' : '-'),  \
        (((i)&0x20) ? '1' : '-'),  \
        (((i)&0x40) ? '1' : '-'),  \
        (((i)&0x80) ? '1' : '-'),  \
        (((i)&0x100) ? '1' : '-'), \
        (((i)&0x200) ? '1' : '-')

    if (isCol)
        snprintf(bitStr, 64, BINARYPATTERN10, BYTEBINARY10(value));
    else
        snprintf(bitStr, 64, BINARYPATTERN9, BYTEBINARY9(value));

    return bitStr;
}

char* getBoardStr(char* boardStr, const Board* boards, int length, int colNum, bool showZero, bool isCol)
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
                getIntBitStr(temp, rowOrCol, isCol);
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
