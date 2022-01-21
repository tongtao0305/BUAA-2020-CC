#pragma once

#include "Global.h"
using namespace std;

// 以下为外部引用的变量和函数
extern ofstream mipsCodeFile;
extern map<string, map<string, struct sym>> symList;
extern vector <struct midCode> midCodes;
extern vector <struct mipsCode> mipsCodes;

extern  map <string, int> functionParameterCount;	// 记录函数参数的数量
vector<string> stringList;			//保存所有的字符串

string curFuncName = "";			// 记录当前所在函数名称
extern int string2int(string s);
extern string int2string(int t);

// 以下为内部定义的变量和函数
void generateMipsCodes();

void loadValue(string symbolName, string regName, bool gene, int& va, bool& get);

void storeValue(string symbolName, string regName);	

void insertMipsCode(mipsOp op, string result, string left, string right, int imm);

void insertMipsCode(mipsOp op, string result, string left, string right);

void outputMipsCodes();

void outputMipsCode(struct mipsCode mc);