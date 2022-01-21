#pragma once

#include "Global.h"
using namespace std;

extern int isDebug;
extern map<string, map<string, struct sym>> symList;
extern vector <struct midCode> midCodes;
extern map<string, vector<midCode> > funcMidCodeTable; //每个函数单独的中间代码	

map<string, vector<Block>> funcBlockTable;

void splitBlock();

void showFuncBlock();

void calUseDef(Block& bl, string funcName);

void calInOut();