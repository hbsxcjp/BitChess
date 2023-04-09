#ifndef MOVE_H
#define MOVE_H

#include "data.h"

// 新建一个move
Move* newMove();

// 加入下着
Move* addNext(Move* move);

// 加入变着
Move* addOther(Move* move);

// 删除move的所有下着move、变着move及自身
void delMove(Move* move);

// 取得ICCS字符串
wchar_t* getICCS(wchar_t* IccsStr, const Move* move);

// 取得Zh字符串
wchar_t* getZH(wchar_t* ZHStr, const Move* move);

// 设置remark
void setRemark(Move* move, const wchar_t* remark);

// 取得表示move的字符串
wchar_t* getMovStringIccsZh(wchar_t* MovStr, size_t n, const Move* move, RecFormat fmt);

// 执行一步着法
Kind doMove(Board* board, Color color, Kind kind, int fromIndex, int toIndex);

// 回退一步着法
void undoMove(Board* board, Color color, Kind kind, int fromIndex, int toIndex, Kind eatKind);

// 获取某个局面下某一方的全部着法（位表示）
int getBoardCanMove(BitMove bitMoves[], Board* board, Color color);

// 回退一步着法
char* getMoveArrayStr(char* moveArrayStr, const BitMove* bitMoves, int length, int colNum);

#endif /* MOVE_H */
