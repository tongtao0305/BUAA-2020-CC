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

extern map<string, vector<midCode>> funcMidCodeTable;	//每个函数单独的中间代码
extern map<string, vector<Block>> funcBlockTable;		//每个函数的基本块列表
extern vector<string> funcNameList;
extern vector<midCode> midCodesForProgram;

// 以下为内部定义的变量和函数
extern int isDebug;
int tRegBusy[10] = { 0, };		// 有t3-t9共7个临时寄存器供分配 用于记录临时寄存器是否被占用
string tRegContent[10];			// 记录每一个临时寄存器分配给了哪一个中间变量 #T0,#T1...
int sRegBusy[10] = { 0, };		// 有s0-s7共8个全局寄存器供分配 用于记录全局寄存器是否被占用
string sRegContent[10];			// 记录每一个全局寄存器分配给了哪一个局部变量

void generateMipsCodes();

void loadValue(string& symbolName, string& regName, bool gene, int& va, bool& get, bool assign=true);

void storeValue(string& symbolName, string& regName);

void insertMipsCode(mipsOp op, string result, string left, string right, int imm);

void insertMipsCode(mipsOp op, string result, string left, string right);

void outputMipsCodes();

void outputMipsCode(struct mipsCode mc);