#include "data.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef bool IsValid(int frow, int fcol);

typedef int GetMoveTo(int toIndex[], int frow, int fcol);

bool isValid(int frow, int fcol) { return true; } // 所有位置均有效

static int getIndexFromSeat(Seat seat)
{
    return seat.row * BOARDCOLNUM + seat.col;
}

static char *getPieceMoveStr(char *moveStr, IsValid *isValid, GetMoveTo *getMoveTo)
{
    strcpy(moveStr, "{");
    for (int row = 0; row < BOARDROWNUM; ++row)
    {
        for (int col = 0; col < BOARDCOLNUM; ++col)
        {
            if (isValid(row, col))
            {
                int toIndex[BOARDCOLNUM + BOARDROWNUM] = {};
                int count = getMoveTo(toIndex, row, col);
                char oneStr[64] = {}, allStr[count * 64];
                strcpy(allStr, "");
                for (int i = 0; i < count; ++i)
                {
                    snprintf(oneStr, 64, "BOARDAT(%d)|", toIndex[i]);
                    strcat(allStr, oneStr);
                }
                allStr[strlen(allStr) - 1] = ','; // 最后的'|'替换成‘,’
                strcat(moveStr, allStr);
                strcat(moveStr, "\n");
            }
            else
                strcat(moveStr, "0,\n");
        }
    }
    moveStr[strlen(moveStr) - 2] = '\x0'; // 去掉最后的",\n"
    strcat(moveStr, "},\n");

    return moveStr;
}

static bool isValidKing(int frow, int fcol)
{
    return (frow < 3 || frow > 6) && (fcol > 2 && fcol < 6);
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

static bool isValidAdvisor(int frow, int fcol)
{
    return (((frow == 0 || frow == 2 || frow == 7 || frow == 9) && (fcol == 3 || fcol == 5)) || ((frow == 1 || frow == 8) && fcol == 4));
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

static bool isValidBishop(int frow, int fcol)
{
    return (((frow == 0 || frow == 4 || frow == 5 || frow == 9) && (fcol == 2 || fcol == 6)) || ((frow == 2 || frow == 7) && (fcol == 0 || fcol == 4 || fcol == 8)));
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

// 初始化棋子可放至位置的位棋盘
static void initPiecePutStr(char *putStr)
{
    IsValid *isValids[] = {
        isValidKing,
        isValidAdvisor,
        isValidBishop,
    };

    strcpy(putStr, "const Board PiecePut[KINDNUM] = {");
    for (int i = 0; i < sizeof(isValids) / sizeof(isValids[0]); ++i)
    {
        char oneStr[64] = {}, allStr[4096];
        snprintf(oneStr, 64, "\n// Piece Kind: %d\n", i);
        strcpy(allStr, oneStr);
        for (int row = 0; row < BOARDROWNUM; ++row)
        {
            for (int col = 0; col < BOARDCOLNUM; ++col)
            {
                if (isValids[i](row, col))
                {
                    snprintf(oneStr, 64, "BOARDAT(%d)|", getIndexFromSeat((Seat){row, col}));
                    strcat(allStr, oneStr);
                }
            }
        }
        allStr[strlen(allStr) - 1] = ','; // 最后的'|'替换成‘,’
        strcat(putStr, allStr);
        strcat(putStr, "\n");
    }

    strcat(putStr, "\n// Piece Kind: 3\n(Board)(-1),\n\n// Piece Kind: 4\n(Board)(-1),"
                   "\n\n// Piece Kind: 5\n(Board)(-1),\n\n// Piece Kind: 6\n(Board)(-1)\n};\n");
}

// 初始化棋子在每一位置可移动至位置的位棋盘
static void initPieceMoveStr(char *aLLKindMoveStr)
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

    strcpy(aLLKindMoveStr, "const Board PieceMove[KINDNUM][BOARDLENGTH] = {");
    for (int i = 0; i < KINDNUM; ++i) //
    {
        char kindMoveStr[BOARDLENGTH * (BOARDCOLNUM + BOARDROWNUM) * 64];
        snprintf(kindMoveStr, 64, "\n// Piece Kind: %d\n", i);
        strcat(aLLKindMoveStr, kindMoveStr);
        getPieceMoveStr(kindMoveStr, isValids[i], getMoveTos[i]);
        strcat(aLLKindMoveStr, kindMoveStr);
    }

    strcat(aLLKindMoveStr, "};\n");
}

// 初始化位置
static void getSeats(char *seatStr)
{
    strcpy(seatStr, "const Seat Seats[BOARDLENGTH] = {\n");
    for (int i = 0; i < BOARDLENGTH; ++i)
    {
        char oneStr[64];
        snprintf(oneStr, 64, "(Seat){%d, %d},\n", i / BOARDCOLNUM, i % BOARDCOLNUM);
        strcat(seatStr, oneStr);
    }

    strcat(seatStr, "};\n");
}

// 初始化单一位置位棋盘
static void initBoardMaskStr(char *maskStr)
{
    strcpy(maskStr, "const Board BoardMask[BOARDLENGTH] = {\n");
    for (int i = 0; i < BOARDLENGTH; ++i)
    {
        char oneStr[64];
        snprintf(oneStr, 64, "BOARDAT(%d),\n", i);
        strcat(maskStr, oneStr);
    }

    strcat(maskStr, "};\n");
}

void genDataFile()
{
    FILE *file = fopen("data.c", "w");
    if (file == NULL)
        return;

    char seatsStr[BOARDLENGTH * 64];
    getSeats(seatsStr);

    char maskStr[BOARDLENGTH * 64];
    initBoardMaskStr(maskStr);

    char putStr[4096];
    initPiecePutStr(putStr);

    char aLLKindMoveStr[KINDNUM * BOARDLENGTH * (BOARDCOLNUM + BOARDROWNUM) * 64];
    initPieceMoveStr(aLLKindMoveStr);

    fprintf(file, "#include \"data.h\"\n\n%s\n%s\n%s\n%s\n", seatsStr, maskStr, putStr, aLLKindMoveStr);

    fclose(file);
}

char *getBoardStr(char *boardStr, const Board *boards, int length, int colNum)
{
    if (length < colNum)
        colNum = length;

    char temp[16],
        indexRowStr[colNum * 16],
        nullRowStr[colNum * 16];
    strcpy(nullRowStr, "  ");
    for (int col = 0; col < colNum; ++col)
        strcat(nullRowStr, " ABCDEFGHI");
    strcat(nullRowStr, "\n");

    strcpy(boardStr, "");
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

        for (int row = 0; row < BOARDROWNUM; ++row)
        {
            snprintf(temp, 16, "%d: ", row);
            strcat(boardStr, temp);
            for (int col = 0; col < colNum; ++col)
            {
                int rowInt = (boards[index + col] >> (row * BOARDCOLNUM)) & 0x1FF;
                snprintf(temp, 16, BINARYPATTERN9, BYTEBINARY9(rowInt));
                strcat(boardStr, temp);
            }
            strcat(boardStr, "\n");
        }
        // strcat(boardStr, "\n");
    }

    return boardStr;
}

static void printData()
{
    printf("testSeats:\n");
    for (int i = 0; i < BOARDLENGTH; ++i)
    {
        printf("seat:(%d, %d), ", Seats[i].row, Seats[i].col);
        if (i % BOARDCOLNUM == BOARDCOLNUM - 1)
            printf("\n");
    }
    printf("\n");

    char boardStr[BOARDLENGTH * (BOARDROWNUM + 2) * 16];
    getBoardStr(boardStr, BoardMask, BOARDLENGTH, BOARDCOLNUM);
    printf("testBoardMask:\n%s\n", boardStr);

    getBoardStr(boardStr, PiecePut, KINDNUM, KINDNUM);
    printf("testPiecePut:\n%s\n", boardStr);

    for (int kind = 0; kind < KINDNUM; ++kind)
    {
        getBoardStr(boardStr, PieceMove[kind], BOARDLENGTH, BOARDCOLNUM);
        printf("testPieceMove kind: %d\n%s\n", kind, boardStr);
    }
}

// 运行genDataFile()前，如果data.c为空，则应将以下注释去掉
// const Seat Seats[BOARDLENGTH] = {};
// const Board BoardMask[BOARDLENGTH] = {};
// const Board PiecePut[KINDNUM] = {};
// const Board PieceMove[KINDNUM][BOARDLENGTH] = {};
void testGenData()
{
    // genDataFile();

    printData();
}