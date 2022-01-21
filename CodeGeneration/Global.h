#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#define MAIN "main"
#define PROGRAM "Program"
using namespace std;

// 词法分析相关定义
enum classCode {
	IDENFR,
	INTCON,
	CHARCON,
	STRCON,
	CONSTTK, 
	INTTK, 
	CHARTK, 
	VOIDTK, 
	MAINTK, 
	IFTK, 
	ELSETK, 
	SWITCHTK, 
	CASETK, 
	DEFAULTTK, 
	WHILETK, 
	FORTK, 
	SCANFTK, 
	PRINTFTK,
	RETURNTK, 
	PLUS, MINU, 
	MULT, 
	DIV, 
	LSS, 
	LEQ, 
	GRE, 
	GEQ, 
	EQL, 
	NEQ, 
	COLON, 
	ASSIGN, 
	SEMICN, 
	COMMA, 
	LPARENT, 
	RPARENT, 
	LBRACK, 
	RBRACK, 
	LBRACE, 
	RBRACE
};

// 符号表相关定义
struct sym {
	// 以下为通用参数
	string name;
	int kind;	// 符号的种类，0常量，1有初始值变量，2无初始值变量，3有返回值函数，4无返回值函数，5函数参数，6main函数
	int type;	// 符号的类型，1int，2int[]，3int[][]，4char，5char[]，6char[][]
	int dimension;	// 若为数组类型，记录其维度；若为函数类型，记录其参数数量
	int addr;

	// 以下为特殊参数
	int num;	// 记录数值型符号的值
	string str;	// 记录字符或字符串型符号的值
	int index;		// 若为数组类型，记录其行数
	int columns;	// 若为数组类型，记录其列数
};

// 中间代码相关定义
enum midOp {
	PLUSOP, //+
	MINUOP, //-
	MULTOP, //*
	DIVOP,  // /
	LSSOP,  //<
	LEQOP,  //<=
	GREOP,  //>
	GEQOP,  //>=
	EQLOP,  //==
	NEQOP,  //!=
	ASSIGNOP,  //=
	GOTO,  //无条件跳转
	BZ,    //不满足条件跳转
	BNZ,   //满足条件跳转
	PUSH,  //函数调用时参数传递
	CALL,  //函数调用
	RET,   //函数返回语句
	RETVALUE, //有返回值函数返回的结果
	SCAN,  //读
	PRINT, //写
	LABEL, //标号
	CONST, //常量
	ARRAY, //数组
	VAR,   //变量
	FUNC,  //函数定义
	PARAM, //函数参数
	GETARRAY,  //取数组的值  t = a[]
	PUTARRAY,  //给数组赋值  a[] = t
	EXIT,  //退出 main最后
};
struct midCode {
	string result;		// 结果
	string left;		// 左操作数
	midOp op;			// 运算符
	string right;		// 右操作数
	set<string> out;
};


// 代码生成相关定义
enum mipsOp {
	add,
	addi,
	sub,
	mult,
	divop,
	mflo,
	mfhi,
	sll,
	beq,
	bne,
	bgt, //扩展指令 相当于一条ALU类指令+一条branch指令
	bge, //扩展指令 相当于一条ALU类指令+一条branch指令
	blt, //扩展指令 相当于一条ALU类指令+一条branch指令
	ble, //扩展指令 相当于一条ALU类指令+一条branch指令
	j,
	jal,
	jr,
	lw,
	sw,
	syscall,
	li,
	la,
	moveop,
	dataSeg,  //.data
	textSeg,  //.text
	asciizSeg,  //.asciiz
	globlSeg,  //.globl
	label,  //产生标号
};
struct mipsCode {
	mipsOp op;		// 操作
	string result;	// 结果
	string left;	// 左操作数
	string right;	// 右操作数
	int imm;		// 立即数
};
