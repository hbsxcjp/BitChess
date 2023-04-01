#include "data.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

Seat Seats[BOARDLENGTH];

int Rotate[BOARDLENGTH];

Board BoardMask[BOARDLENGTH];

Board PiecePut[KINDNUM];

Board PieceMove[KINDNUM][BOARDLENGTH];

typedef bool IsValid(int frow, int fcol);

typedef int GetMoveTo(int toIndex[], int frow, int fcol);

bool isValid(int frow, int fcol) { return true; } // 所有位置均有效

static int getIndexFromSeat(Seat seat)
{
    return INDEXFROMROWCOL(seat.row, seat.col);
}

bool isValidKing(int row, int col)
{
    return (row < 3 || row > 6) && (col > 2 && col < 6);
}

bool isValidAdvisor(int row, int col)
{
    return (((row == 0 || row == 2 || row == 7 || row == 9) && (col == 3 || col == 5)) ||
            ((row == 1 || row == 8) && col == 4));
}

bool isValidBishop(int row, int col)
{
    return (((row == 0 || row == 4 || row == 5 || row == 9) && (col == 2 || col == 6)) ||
            ((row == 2 || row == 7) && (col == 0 || col == 4 || col == 8)));
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
        {
            select[0] = select[1] = false;
            if (frow == 0)
                select[4] = select[5] = false;
        }
        else if (frow > 7)
        {
            select[2] = select[3] = false;
            if (frow == 9)
                select[6] = select[7] = false;
        }
    }
    else if (fcol == 7) // 最右第二列
    {
        select[5] = select[7] = false;
        if (frow < 2)
        {
            select[0] = select[1] = false;
            if (frow == 0)
                select[4] = select[5] = false;
        }
        else if (frow > 7)
        {
            select[2] = select[3] = false;
            if (frow == 9)
                select[6] = select[7] = false;
        }
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

static int getRookCannonMoveTo(int toIndex[], int frow, int fcol)
{
    int count = 0;
    for (int row = frow - 1; row >= 0; --row)
        toIndex[count++] = getIndexFromSeat((Seat){row, fcol});

    for (int row = frow + 1; row <= 9; ++row)
        toIndex[count++] = getIndexFromSeat((Seat){row, fcol});

    for (int col = fcol - 1; col >= 0; --col)
        toIndex[count++] = getIndexFromSeat((Seat){frow, col});

    for (int col = fcol + 1; col <= 8; ++col)
        toIndex[count++] = getIndexFromSeat((Seat){frow, col});

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

void initData()
{
    IsValid *isValids[] = {
        isValidKing,
        isValidAdvisor,
        isValidBishop,
        isValid,
        isValid,
        isValid,
        isValid,
    };

    GetMoveTo *getMoveTos[] = {
        getKingMoveTo,
        getAdvisorMoveTo,
        getBishopMoveTo,
        getKnightMoveTo,
        getRookCannonMoveTo,
        getRookCannonMoveTo,
        getPawnMoveTo,
    };

    int index = 0;
    for (int row = 0; row < BOARDROWNUM; ++row)
    {
        for (int col = 0; col < BOARDCOLNUM; ++col)
        {
            Seats[index] = (Seat){row, col};
            BoardMask[index] = BOARDAT(index);
            Rotate[index] = col * BOARDROWNUM + row;

            ++index;
        }
    }

    for (Kind kind = KING; kind <= PAWN; ++kind)
    {
        int index = 0;
        for (int row = 0; row < BOARDROWNUM; ++row)
        {
            for (int col = 0; col < BOARDCOLNUM; ++col)
            {
                if (isValids[kind](row, col))
                {
                    // 棋子可放至位置的位棋盘
                    PiecePut[kind] |= BoardMask[index];

                    int toIndex[BOARDCOLNUM + BOARDROWNUM];
                    int count = getMoveTos[kind](toIndex, row, col);
                    for (int i = 0; i < count; ++i)
                        // 棋子在每一位置可移动至位置的位棋盘
                        PieceMove[kind][index] |= BoardMask[toIndex[i]];
                }
                else
                    PieceMove[kind][index] = 0;

                ++index;
            }
        }
    }
}

char *getRowColBit(char *bitStr, int value, bool isCol)
{
    if (isCol)
        snprintf(bitStr, 64, BINARYPATTERN10, BYTEBINARY10(value));
    else
        snprintf(bitStr, 64, BINARYPATTERN9, BYTEBINARY9(value));

    return bitStr;
}

char *getBoardStr(char *boardStr, const Board *boards, int length, int colNum, bool showZero, bool isCol)
{
    if (length < colNum)
        colNum = length;

    char temp[64],
        indexRowStr[colNum * 16],
        nullRowStr[colNum * 16];
    strcpy(nullRowStr, "  ");
    for (int col = 0; col < colNum; ++col)
        strcat(nullRowStr, isCol ? " 0123456789" : " ABCDEFGHI");
    strcat(nullRowStr, "\n");

    strcpy(boardStr, "");
    Board nonZeroBoards[length];
    if (!showZero)
    {
        int count = 0;
        for (int index = 0; index < length; ++index)
        {
            if (boards[index])
                nonZeroBoards[count++] = boards[index];
        }
        boards = nonZeroBoards;
        length = count;
    }
    for (int index = 0; index < length; index += colNum)
    {
        strcpy(indexRowStr, "   ");
        for (int col = 0; col < colNum; ++col)
        {
            snprintf(temp, 16, "%02d(%d,%d):  ", index + col, (index + col) / colNum, col);
            strcat(indexRowStr, temp);
        }
        strcat(indexRowStr, "\n");
        strcat(boardStr, indexRowStr);
        strcat(boardStr, nullRowStr);

        int totalRow = isCol ? BOARDCOLNUM : BOARDROWNUM,
            totalCol = !isCol ? BOARDCOLNUM : BOARDROWNUM,
            mode = !isCol ? 0x1FF : 0x3FF;
        for (int row = 0; row < totalRow; ++row)
        {
            if (isCol)
                snprintf(temp, 16, "%c: ", 'A' + row);
            else
                snprintf(temp, 16, "%d: ", row);
            strcat(boardStr, temp);
            for (int col = 0; col < colNum && index + col < length; ++col)
            {
                int rowOrCol = (boards[index + col] >> (row * totalCol)) & mode;
                getRowColBit(temp, rowOrCol, isCol);
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

void printData()
{
    char temp[64],
        boardStr[BOARDLENGTH * (BOARDROWNUM + 2) * 16];
    strcpy(boardStr, "Seats[90]:\n");
    for (int i = 0; i < BOARDLENGTH; ++i)
    {
        snprintf(temp, 64, "seat:(%d, %d), ", Seats[i].row, Seats[i].col);
        strcat(boardStr, temp);
        if (i % BOARDCOLNUM == BOARDCOLNUM - 1)
            strcat(boardStr, "\n");
    }

    strcat(boardStr, "Rotate[90]:\n");
    for (int i = 0; i < BOARDLENGTH; ++i)
    {
        snprintf(temp, 64, "%3d", Rotate[i]);
        strcat(boardStr, temp);
        if (i % BOARDROWNUM == BOARDROWNUM - 1)
            strcat(boardStr, "\n");
    }
    printf("%s\n", boardStr);

    getBoardStr(boardStr, BoardMask, BOARDLENGTH, BOARDCOLNUM, true, false);
    printf("testBoardMask:\n%s\n", boardStr);

    getBoardStr(boardStr, PiecePut, KINDNUM, KINDNUM, true, false);
    printf("testPiecePut:\n%s\n", boardStr);

    for (int kind = 0; kind < KINDNUM; ++kind)
    {
        getBoardStr(boardStr, PieceMove[kind], BOARDLENGTH, BOARDCOLNUM, true, false);
        printf("testPieceMove kind: %d\n%s\n", kind, boardStr);
    }
}