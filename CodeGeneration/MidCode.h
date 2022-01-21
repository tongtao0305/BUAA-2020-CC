#pragma once

#include "Global.h"
using namespace std;

// 以下为外部引用的变量和函数
extern ofstream midCodeFile;
extern vector <struct midCode> midCodes;
extern map<string, map<string, struct sym>> symList;

// 以下为内部定义的变量和函数
// 新增函数
void insertMidCode(string result, string left, midOp op, string right);

void outputMidCodes();

void outputMidCode(struct midCode m);