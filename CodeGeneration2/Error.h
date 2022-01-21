#pragma once

#include "Global.h"
using namespace std;

// 以下为外部引用的变量和函数
extern ofstream errorFile;
extern int cnt_line;
extern int lastCountLine;

// 以下为内部定义的变量和函数
void error(char errorCode);