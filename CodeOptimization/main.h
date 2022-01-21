#pragma once

#include "Global.h"

ifstream ifile;				// 源代码 输入文件
ofstream ofile;				// 语法分析 输出文件
ofstream errorFile;			// 错误处理 输出文件
ofstream midCodeFile;		// 中间代码 输出文件
ofstream mipsCodeFile;		// mips代码 输出文件

int labelId = 0;	// 标号的id
int tmpVarId = 0;	// 中间变量的id
int nameId = 0;

// 文件处理和初始化
void openFiles();
void closeFiles();
void init_CODEN();
void init_CODES();

// 词法分析阶段
string CODEN[40];			// 保存单词类别编码
string CODES[40];			// 保存单词对应字符串
extern void getsym();

// 语法分析阶段
extern void getProgram();

// 中间代码生成阶段
vector <struct midCode> midCodes;		// 保存了所有中间代码
extern void outputMidCodes();			// 输出中间代码

// 目标代码生成阶段
vector <struct mipsCode> mipsCodes;		// 保存了所有mips代码
extern void generateMipsCodes();		// 从中间代码生成mips代码
extern void outputMipsCodes();			// 输出mips代码

// 代码优化阶段
extern void splitBlock();
extern void showFuncBlock();
extern vector<midCode> midCodesForProgram;
extern map<string, bool> funcInlineAble;
void outputMidCode(struct midCode mc);