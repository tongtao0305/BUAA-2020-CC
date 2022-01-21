#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
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
	int length;		// 记录函数中参数+变量+中间变量的数量
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
	GETARRAY2,  //取数组的值  t = a[][]
	PUTARRAY,  //给数组赋值  a[] = t
	PUTARRAY2,  //给数组赋值  a[][] = t
	EXIT,  //退出 main最后
	JMAIN,	// 跳转到main
	EXITCASE,		// 跳出case语句
	ENTERSWITCH,
	EXITSWITCH,
	INLINERET,  //函数内联之后的返回语句
	INLINEEND   //内联的结尾
};
struct midCode {
	string result;		// 结果
	string left;		// 左操作数
	midOp op;			// 运算符
	string right;		// 右操作数
	string backup;		// 备用
};


// 代码生成相关定义
enum mipsOp {
	add,
	addi,
	sub,
	mult,
	mul,
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
	blez,  //一条branch
	bgtz,  //一条branch
	bgez,  //一条branch
	bltz,  //一条branch
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


class Block {
public:
	int start;
	int end;
	int nextBlock1;
	int nextBlock2;
	vector<midCode> midCodeVector;
	vector<string> use;
	vector<string> def;
	vector<string> in;
	vector<string> out;

	Block(int s, int e, int n1, int n2) : start(s), end(e), nextBlock1(n1), nextBlock2(n2) {
		use = vector<string>();
		def = vector<string>();
		in = vector<string>();
		out = vector<string>();
	}

	void setnextBlock1(int n1) {
		nextBlock1 = n1;
	}

	void setnextBlock2(int n2) {
		nextBlock2 = n2;
	}

	void insert(midCode mc) {
		midCodeVector.push_back(mc);
	}

	void useInsert(string name) {
		use.push_back(name);
	}

	void defInsert(string name) {
		def.push_back(name);
	}

	void inInsert(string name) {
		in.push_back(name);
	}

	void outInsert(string name) {
		out.push_back(name);
	}

	void output() {
		cout << start << " " << end << " " << nextBlock1 << " " << nextBlock2 << "\n";
		cout << "use: \n";
		for (int i = 0; i < use.size(); i++) {
			cout << use[i] << " ";
		}
		cout << "\n";
		cout << "def: \n";
		for (int i = 0; i < def.size(); i++) {
			cout << def[i] << " ";
		}
		cout << "\n";
		cout << "in: \n";
		for (int i = 0; i < in.size(); i++) {
			cout << in[i] << " ";
		}
		cout << "\n";
		cout << "out: \n";
		for (int i = 0; i < out.size(); i++) {
			cout << out[i] << " ";
		}
		cout << "\n";
		for (int i = 0; i < midCodeVector.size(); i++) {
			cout << "(" << start + i << ") ";
			midCode mc = midCodeVector[i];
			switch (mc.op) {
			case PLUSOP:
				cout << mc.result << " = " << mc.left << " + " << mc.right << "\n";
				break;
			case MINUOP:
				cout << mc.result << " = " << mc.left << " - " << mc.right << "\n";
				break;
			case MULTOP:
				cout << mc.result << " = " << mc.left << " * " << mc.right << "\n";
				break;
			case DIVOP:
				cout << mc.result << " = " << mc.left << " / " << mc.right << "\n";
				break;
			case LSSOP:
				cout << mc.result << " = (" << mc.left << " < " << mc.right << ")\n";
				break;
			case LEQOP:
				cout << mc.result << " = (" << mc.left << " <= " << mc.right << ")\n";
				break;
			case GREOP:
				cout << mc.result << " = (" << mc.left << " > " << mc.right << ")\n";
				break;
			case GEQOP:
				cout << mc.result << " = (" << mc.left << " >= " << mc.right << ")\n";
				break;
			case EQLOP:
				cout << mc.result << " = (" << mc.left << " == " << mc.right << ")\n";
				break;
			case NEQOP:
				cout << mc.result << " = (" << mc.left << " != " << mc.right << ")\n";
				break;
			case ASSIGNOP:
				cout << mc.result << " = " << mc.left << "\n";
				break;
			case GOTO:
				cout << "GOTO " << mc.result << "\n";
				break;
			case BZ:
				cout << "BZ " << mc.result << "(" << mc.left << "=0)" << "\n";
				break;
			case BNZ:
				cout << "BNZ " << mc.result << "(" << mc.left << "=1)" << "\n";
				break;
			case PUSH:
				cout << "PUSH " << mc.result << "\n";
				break;
			case CALL:
				cout << "CALL " << mc.result << "\n";
				break;
			case RET:
				cout << "RET " << mc.result << "\n";
				break;
			case INLINERET:
				cout << "INLINERET " << mc.result << "\n";
				break;
			case RETVALUE:
				cout << "RETVALUE " << mc.result << " = " << mc.left << "\n";
				break;
			case SCAN:
				cout << "SCAN " << mc.result << "\n";
				break;
			case PRINT:
				cout << "PRINT " << mc.result << " " << mc.left << "\n";
				break;
			case LABEL:
				cout << mc.result << ": \n";
				break;
			case CONST:
				cout << "CONST " << mc.result << " " << mc.left << " = " << mc.right << endl;
				break;
			case ARRAY:
				cout << "ARRAY " << mc.result << " " << mc.left << "[" << mc.right << "]" << endl;
				break;
			case VAR:
				cout << "VAR " << mc.result << " " << mc.left << endl;
				break;
			case FUNC:
				cout << "FUNC " << mc.result << " " << mc.left << "()" << endl;
				break;
			case PARAM:
				cout << "PARA " << mc.result << " " << mc.left << endl;
				break;
			case GETARRAY:
				cout << mc.result << " = " << mc.left << "[" << mc.right << "]\n";
				break;
			case PUTARRAY:
				cout << mc.result << "[" << mc.left << "]" << " = " << mc.right << "\n";
				break;
			case EXIT:
				cout << "EXIT\n";
				break;
			default:
				break;
			}
		}
	}
};