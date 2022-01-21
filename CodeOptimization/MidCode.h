#pragma once

#include "Global.h"
using namespace std;

// 以下为外部引用的变量和函数
extern string curLevel;
extern ofstream midCodeFile;
extern vector <struct midCode> midCodes;
extern vector<midCode> midCodesForProgram;
extern map<string, map<string, struct sym>> symList;

// 以下为内部定义的变量和函数
// 新增函数
void insertMidCode(midOp op, string result, string left, string right);

void insertMidCode(midOp op, string result, string left, string right, string backup);

void outputMidCodes();

void outputMidCode(struct midCode m);