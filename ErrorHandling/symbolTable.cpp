#include <map>
#include <string>
#include <algorithm>
using namespace std;

// 错误处理函数
extern void error(char errorCode);

void initCurrentSymbol();		// 将当前的curSym初始化

void insertCurrentSymbol();

void insertCurrentLevel();

// 借用文法分析中的两个函数将字符串转化成小写
extern int isString(char c);		// 判断是否为字符串中的字符
extern void tolowercase(char s[]);	// 将字符串中的大写英文字符转化为小写

// 符号表
struct sym {
	// 以下为通用参数
	string name;
	int kind;	// 符号的种类，0为程序常量，1为程序变量，2为函数名，3为函数参数表
	int type;	// 符号的类型，0为int，1为char
	int dimension;	// 若为数组类型，记录其维度；若为函数类型，记录其参数数量
	
	// 以下为特殊参数
	int num;	// 记录数值型符号的值
	string str;	// 记录字符或字符串型符号的值
	int index;		// 若为数组类型，记录其行数
	int columns;	// 若为数组类型，记录其列数
};
extern map<string, map<string, struct sym>> symList;
extern struct sym curSym;					// 记录当前正在使用的符号
extern string curLevel;						// 记录当前所在的符号表层次名称
extern map<string, struct sym> curMap;		// 记录当前所在的符号表层次的map

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

// 该函数将当前的层次插入符号表
void insertCurrentLevel() {
	curMap.clear();
	symList[curLevel] = curMap;
}

// 该函数将当前的符号插入当前的层次
void insertCurrentSymbol() {
	//transform(curSym.name.begin(), curSym.name.end(), curSym.name.begin(), ::tolower);
	if (symList[curLevel].find(curSym.name) == symList[curLevel].end()) {
		symList[curLevel][curSym.name] = curSym;
	}
	else {
		error('b');
	}
}

