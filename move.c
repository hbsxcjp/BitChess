#include "move.h"
#include "board.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DEBUGFILTERKILLEDMOVE

Move* newMove()
{
    Move* move = (Move*)malloc(sizeof(Move));
    move->color = NONCOLOR;
    move->kind = NONKIND;
    move->fromIndex = 0;
    move->toIndex = 0;

    move->front = move->next = move->other = NULL;

    move->remark = NULL;

    return move;
}

Move* addNext(Move* move)
{
    Move* nextMove = newMove();
    move->next = nextMove;
    nextMove->front = move;

    return nextMove;
}

Move* addOther(Move* move)
{
    Move* otherMove = newMove();
    move->other = otherMove;
    otherMove->front = move;

    return otherMove;
}

void delMove(Move* move)
{
    if (move == NULL)
        return;

    Move* next = move->next;
    Move* other = move->other;
    free(move->remark);
    free(move);

    delMove(next);
    delMove(other);
}

wchar_t* getICCS(wchar_t* IccsStr, const Move* move)
{
    Coord fcoord = Coords[move->fromIndex], tcoord = Coords[move->toIndex];
    swprintf(IccsStr, 8, L"%lc%d%c%d", IccsChar[fcoord.col], fcoord.row, IccsChar[tcoord.col], tcoord.row);

    return IccsStr;
}

wchar_t* getZH(wchar_t* ZHStr, const Move* move)
{
    ZHStr[0] = L'\x0';
    return ZHStr;
}

void setRemark(Move* move, const wchar_t* remark)
{
    if (remark == NULL || wcslen(remark) == 0)
        return;

    move->remark = (wchar_t*)malloc((wcslen(remark) + 1) * sizeof(remark[0]));
    wcscpy(move->remark, remark);
}

static wchar_t* getRemarkStr(wchar_t* remark, size_t n, const Move* move)
{
    if (move->remark == NULL)
        remark[0] = L'\x0';
    else
        swprintf(remark, n, L" {%ls} ", move->remark);

    return remark;
}

static wchar_t* getMovString(wchar_t* movStr, size_t n, const Move* move, int* nextNo, bool isPGN_ZH, bool isOther)
{
    wchar_t boutStr[6], tempStr[REMARKSIZE];
    swprintf(boutStr, 6, L"%d. ", (*nextNo + 1) / 2);
    bool isEven = *nextNo % 2 == 0;
    if (isOther) {
        swprintf(tempStr, REMARKSIZE, L"(%ls%ls", boutStr, isEven ? L"... " : L"");
        wcscat(movStr, tempStr);
    } else
        wcscat(movStr, isEven ? L" " : boutStr);

    wcscat(movStr, isPGN_ZH ? getZH(tempStr, move) : getICCS(tempStr, move));
    wcscat(movStr, L" ");
    wcscat(movStr, getRemarkStr(tempStr, REMARKSIZE, move));

    if (move->other != NULL) {
        getMovString(movStr, n, move->other, nextNo, isPGN_ZH, true);
        wcscat(movStr, L")");
    }

    if (move->next != NULL) {
        ++nextNo;
        getMovString(movStr, n, move->next, nextNo, isPGN_ZH, false);
    }

    return movStr;
}

wchar_t* getMovStringIccsZh(wchar_t* movStr, size_t n, const Move* rootMove, RecFormat fmt)
{
    int nextNo = 0;
    getRemarkStr(movStr, n, rootMove);
    getMovString(movStr, n, rootMove, &nextNo, fmt == PGN_ZH, false);

    return movStr;
}

Kind doMove(Board* board, Color color, Kind kind, int fromIndex, int toIndex)
{
    BitBoard toBoard = BoardMask[toIndex],
             moveBoard = BoardMask[fromIndex] | toBoard,
             rotateMoveBoard = BoardMask[Rotate[fromIndex]] | BoardMask[Rotate[toIndex]];

    // 清除原位置，置位新位置
    board->pieces[color][kind] ^= moveBoard;
    board->colorPieces[color] ^= moveBoard;
    board->allPieces ^= moveBoard;
    board->rotatePieces ^= rotateMoveBoard;

    Color toColor = !color;
    Kind thisToKind = NONKIND;
    // 新位置有对方棋子时
    if (board->colorPieces[toColor] & toBoard) {
        for (Kind toKind = KING; toKind <= PAWN; ++toKind) {
            if (board->pieces[toColor][toKind] & toBoard) {
                thisToKind = toKind;
                board->pieces[toColor][toKind] ^= toBoard;
                board->colorPieces[toColor] ^= toBoard;
                break;
            }
        }
        // assert(thisToKind != NONKIND);
    }

    return thisToKind;
}

void undoMove(Board* board, Color color, Kind kind, int fromIndex, int toIndex, Kind eatKind)
{
    BitBoard fromBoard = BoardMask[fromIndex],
             moveBoard = fromBoard | BoardMask[toIndex],
             rotateMoveBoard = BoardMask[Rotate[fromIndex]] | BoardMask[Rotate[toIndex]];

    // 清除原位置，置位新位置
    board->pieces[color][kind] ^= moveBoard;
    board->colorPieces[color] ^= moveBoard;
    board->allPieces ^= moveBoard;
    board->rotatePieces ^= rotateMoveBoard;

    if (eatKind != NONKIND) {
        Color toColor = !color;
        board->pieces[toColor][eatKind] ^= fromBoard;
        board->colorPieces[toColor] ^= fromBoard;
    }
}

static BitBoard getMove(Board* board, Color color, Kind kind, int index)
{
    BitBoard move;
    switch (kind) {
    case KING:
        move = KingMove[index];
        break;
    case ADVISOR:
        move = AdvisorMove[index];
        break;
    case BISHOP:
        move = getBishopMove(index, board->allPieces);
        break;
    case KNIGHT:
        move = getKnightMove(index, board->allPieces);
        break;
    case ROOK:
        move = getRookMove(index, board->allPieces, board->rotatePieces);
        break;
    case CANNON:
        move = getCannonMove(index, board->allPieces, board->rotatePieces);
        break;
    default:
        move = PawnMove[index][board->bottomColor == color];
        break;
    }

    // 去掉同色棋子
    return move ^ (move & board->colorPieces[color]);
}

static void addMove(Board* board, Color color, Kind kind, int index, void* colorMove, void* arg2)
{
    *(BitBoard*)colorMove |= getMove(board, color, kind, index);
}

// 获取某种颜色棋子可移动至的全部位置
static BitBoard getColorMove(Board* board, Color color)
{
    BitBoard colorMove = 0;
    traverseColorPieces(board, color, addMove, &colorMove, NULL);
    return colorMove;
}

static void filterKilledMove(Board* board, Color color, Kind kind, int toIndex, void* pfromIndex, void* pmove)
{
#ifdef DEBUGFILTERKILLEDMOVE
    Board tempBoard = *board;
#endif
    int fromIndex = *(int*)pfromIndex;
    Kind eatKind = doMove(board, color, kind, fromIndex, toIndex);
    // 排除被杀将的位置
    if (getColorMove(board, !color) & board->pieces[color][KING])
        *(BitBoard*)pmove ^= BoardMask[toIndex];

    undoMove(board, color, kind, toIndex, fromIndex, eatKind);

#ifdef DEBUGFILTERKILLEDMOVE
    char boardStr[4 * 4096];
    if (!isEqual(tempBoard, *board)) {
        printf("testGetMoveTo:\ntempChess:\n%s", getBoardStr(boardStr, &tempBoard));
        printf("*board:\n%s\n", getBoardStr(boardStr, board));
    }
    assert(isEqual(tempBoard, *board));
#endif
}

static void getCanMove(Board* board, Color color, Kind kind, int fromIndex, void* bitMoves, void* count)
{
    BitBoard move = getMove(board, color, kind, fromIndex);
    traverseColorKindPieces(board, color, kind, getNonZeroIndex(board, color), move,
        filterKilledMove, &fromIndex, &move);

    ((BitMove*)bitMoves)[(*(int*)count)++] = (BitMove) { color, kind, fromIndex, move };
}

int getBoardCanMove(BitMove bitMoves[], Board* board, Color color)
{
    int count = 0;
    traverseColorPieces(board, color, getCanMove, bitMoves, &count);

    return count;
}

static void setRowColStr(Board* board, Color color, Kind kind, int index, void* moveArrayStr, void* rowColStr)
{
    Coord coord = Coords[index];
    snprintf((char*)rowColStr, 64, "(%02d,%02d) ", coord.row, coord.col);
    strcat((char*)moveArrayStr, (char*)rowColStr);
}

char* getMoveArrayStr(char* moveArrayStr, const BitMove* bitMoves, int length, int colNum)
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
            BitMove move = bitMoves[index + col];
            snprintf(temp, 64, "%c %02d      ", Chars[move.color][move.kind], move.index);
            strcat(moveArrayStr, temp);

            getBitBoardStr(boardStr[col], move.moveTo, false);
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
        BitMove move = bitMoves[index];
        Coord coord = Coords[move.index];
        snprintf(temp, 64, "[%c] from:(%02d,%02d) to:", Chars[move.color][move.kind], coord.row, coord.col);
        strcat(moveArrayStr, temp);

        traverseColorKindPieces(NULL, NONCOLOR, NONKIND, getLowNonZeroIndex, move.moveTo,
            setRowColStr, moveArrayStr, temp);

        strcat(moveArrayStr, "\n");
    }

    return moveArrayStr;
}