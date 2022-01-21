#pragma once

#include "Global.h"
using namespace std;

// 以下为外部引用的变量和函数
extern ofstream ofile;			// 输出文件

// 词法分析阶段
extern int symbol;				// 记录当前Token的类型
extern char TOKEN[256];			// 记录当前Token的字符串
extern int lastSymbol;			// 记录上一个Token的类型
extern char lastTOKEN[256];		// 记录上一个Token的字符串
extern int lastlastSymbol;		// 记录上上个Token的类型
extern char lastlastTOKEN[256];	// 记录上上个Token的字符串
extern int cnt_line;			// 记录当前缓冲区在源文件中的行数，以便进行错误输出

extern void getsym();
extern void trysym();
extern void ungetsym();
extern void outputsym();
extern void outputlastsym();
extern void outputlastlastsym();

// 错误处理阶段
extern void error(char errorCode);

// 符号表
extern string curLevel;										// 记录当前所在的符号表层次名称
extern struct sym curSym;									// 记录当前正在使用的符号
extern map<string, map<string, struct sym>> symList;		// 符号表

extern void initCurrentSymbol();
extern void insertCurrentSymbol();
extern void insertCurrentLevel();
extern void insertSymbol(string name, int kind, int type);

// 中间代码生成阶段
extern vector<string> stringList;

extern string genTmp();				// 获取临时变量符号
extern int string2int(string s);	// 字符串转化为整型
extern string int2string(int t);	// 整型转化为字符串
extern void insertMidCode(string result, string left, midOp op, string right);


// 以下为内部定义的变量和函数
int dimension = 0;						// 变量定义的数组维数
int indexCount = 0;						// 无初始化变量的数组的行数
int columnsCount = 0;					// 有初始化变量的数组的列数

int lastTypeIdentifier;						// 记录上一个类型标识符，0代表整数，1代表字符
string lastIdentifier;						// 记录上一个标识符
set<string> functionWithReturnValue;		// 记录有返回值函数
set<string> functionWithoutReturnValue;		// 记录无返回值函数
map<string, int> functionReturnType;		// 记录函数返回值类型，0为无返回值，1为整型，2为字符

// 错误处理相关变量
int lastOperator;			// 保存上一个整数的符号，-1表示为负数
int lastInteger;			// 保存上一个整数
int lastUnsignedInteger;	// 保存上一个无符号整数
string lastChar;			// 保存上一个字符
int constType;				// 记录上一个常数的类型，0代表整数，1代表字符
int lastExpressionType;		// 记录上一个表达式的类型，1代表整数，2代表字符
int lastConstantType;		// 记录上一个常量的类型，0代表整数，1代表字符
int lastVariableType;		// 记录上一个变量定义的类型，0代表整数，1代表字符

int hasReturn;			// 记录当前函数是否有返回值，0为无返回值，1为整型，2为字符
int hasReturned;		// 记录当前函数是否已经返回，0为无返回值，1为整型，2为字符
int returnType;			// 记录需要的返回值类型，0为无返回值，1为整型，2为字符
int returnedType;		// 记录已经返回值的类型，0为无返回值，1为整型，2为字符，3为return;

string lastFunctionCall;	// 记录上一个函数调用
int parameterCount;			// 记录函数参数表的数量
int valueParameterCount;	// 记录函数值参数表的数量
map <string, int> functionParameterCount;			// 记录有返回值函数的参数数量
map <string, int[100]> functionParameterType;		// 记录有返回值函数的参数类型

// 递归调用子函数
void getAdditionOperator();			// <加法运算符>
void getMultiplicationOperator();	// <乘法运算符>
void getRelationalOperator();		// <关系运算符>
void getLetter();					// <字母>
void getDigit();					// <数字>
void getChar();						// <字符>
void getString();					//*<字符串>

void getProgram();					//*<程序>
void getConstantDeclaration();		//*<常量说明>
void getConstantDefination();		//*<常量定义>
void getUnsignedInteger();			//*<无符号整数>
void getUnsignedInteger(int& unsignedInteger);
void getInteger();					//*<整数>
void getInteger(int& integer);

void getIdentifier();				// <标识符>
void getIdentifier(string& identifier);
void getDeclarationHeader();		//*<声明头部>
void getConstant();					//*<常量>
void getVariableDeclaration();		//*<变量说明>
void getVariableDefination();		//*<变量定义>
void getVariableDefinationWithoutInitialization();	//*<变量定义无初始化>
void getVariableDefinationWithInitialization();		//*<变量定义及初始化>
void getTypeIdentifier();			// <类型标识符>
void getTypeIdentifier(int& typeIdentifier);

void getFunctionDefinationWithReturnValue();	//*<有返回值函数定义>
void getFunctionDefinationWithoutReturnValue();	//*<无返回值函数定义>
void getCompoundStatements();					//*<复合语句>
void getParameterList();						//*<参数表>
void getMain();									//*<主函数>

void getExpression();	// 表达式
void getExpression(int& type, string& ansTmp);
void getTerm();			// 项
void getTerm(int& type, string& ansTmp);
void getFactor();		// 因子
void getFactor(int& type, string& ansTmp);

void getStatement();
void getAssignmentStatement();
void getConditionalStatement();
void getCondition();
void getLoopStatement();
void getStep();
void getSwitch();
void getCaseTable();
void getCase();
void getDefault();
void getFunctionCallStatementWithReturnValue();
void getFunctionCallStatementWithoutReturnValue();
void getValueParameterTable();
void getStatementList();
void getScanf();
void getPrintf();
void getReturn();

// program专属函数
void getVariableDeclarationForProgram();
void getVariableDefinationForProgram();
void getMainForProgram();
void getFunctionDefinationWithReturnValueForProgram();
void getFunctionDefinationWithoutReturnValueForProgram();

// 检查symbol类型
void checkSymbol(int symType, char errorType);
void checkSymbolandGetSym(int symType, char errorType);
