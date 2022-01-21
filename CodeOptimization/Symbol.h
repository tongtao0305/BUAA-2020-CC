#pragma once

#include "Global.h"
using namespace std;

// 以下为外部引用的变量和函数
// 借用文法分析中的两个函数将字符串转化成小写
extern int isString(char c);		// 判断是否为字符串中的字符
extern void tolowercase(char s[]);	// 将字符串中的大写英文字符转化为小写

// 错误处理函数
extern void error(char errorCode);


// 以下为内部定义的变量和函数
int globalAddr = 0;					// 记录全局变量在栈中的地址
int localAddr = 0;					// 记录局部变量在栈中的地址

string curLevel;					// 记录当前所在的符号表层次名称
struct sym curSym;					// 记录当前正在使用的符号
map<string, struct sym> curMap;		// 记录当前所在的符号表层次的map
map<string, map<string, struct sym>> symList;

void initCurrentSymbol();

void insertCurrentSymbol();

void insertCurrentLevel();

void insertSymbol(string name, int kind, int type);
