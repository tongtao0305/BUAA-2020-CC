#include "Symbol.h"

// 初始化当前的符号
void initCurrentSymbol() {
	curSym.name = "";
	curSym.kind = -1;
	curSym.type = -1;
	curSym.dimension = 0;

	curSym.num = -1;
	curSym.str = "";
	curSym.index = -1;
	curSym.columns = -1;
}

// 该函数将当前的符号插入当前的层次
void insertCurrentSymbol() {
	if (symList[curLevel].find(curSym.name) == symList[curLevel].end()) {
		symList[curLevel][curSym.name] = curSym;
		symList[curLevel][curSym.name].addr = curLevel == PROGRAM ? globalAddr : localAddr;	

		if (curSym.type == 1 || curSym.type == 4) {
			// 非数组
			if (curLevel == PROGRAM) {
				globalAddr++;
			}
			else {
				localAddr++;
			}
		}
		else if (curSym.type == 2 || curSym.type == 5) {
			// 一维数组
			if (curLevel == PROGRAM) {
				globalAddr = globalAddr + curSym.index;
			}
			else {
				localAddr = localAddr + curSym.index;
			}
		}
		else if (curSym.type == 3 || curSym.type == 6) {
			// 二维数组
			if (curLevel == PROGRAM) {
				globalAddr = globalAddr + curSym.index * curSym.columns;
			}
			else {
				localAddr = localAddr + curSym.index * curSym.columns;
			}
		}
	}
	else {
		error('b');
	}
}

// 该函数将当前的层次插入符号表
void insertCurrentLevel() {
	symList[curLevel] = curMap;
	curMap.clear();
	localAddr = 0;
}

void insertSymbol(string name, int kind, int type) {
	struct sym tmp;
	tmp.name = name;
	tmp.kind = kind;
	tmp.type = type;
	if (symList[curLevel].find(tmp.name) == symList[curLevel].end()) {
		symList[curLevel][tmp.name] = tmp;
	}
	symList[curLevel][name].addr = curLevel == PROGRAM ? globalAddr++ : localAddr++;
}

void insertSymbol(string name, struct sym tmpSymbol) {
	if (symList[curLevel].find(name) == symList[curLevel].end()) {
		symList[curLevel][name] = tmpSymbol;
	}
	symList[curLevel][name].addr = curLevel == PROGRAM ? globalAddr++ : localAddr++;
}