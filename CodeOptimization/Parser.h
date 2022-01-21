#pragma once

#include "Global.h"
using namespace std;

// 以下为外部引用的变量和函数
extern ofstream ofile;			// 输出文件
extern int isError;
extern int isDebug;

// 词法分析阶段
extern int symbol;				// 记录当前Token的类型
extern char TOKEN[256];			// 记录当前Token的字符串
extern int lastSymbol;			// 记录上一个Token的类型
extern char lastTOKEN[256];		// 记录上一个Token的字符串
extern int lastlastSymbol;		// 记录上上个Token的类型
extern char lastlastTOKEN[256];	// 记录上上个Token的字符串
extern int cnt_line;			// 记录当前缓冲区在源文件中的行数，以便进行错误输出

extern void getsym();
extern void getsym(bool isOutput);
extern void trysym();
extern void ungetsym();
extern void outputsym();

// 错误处理阶段
extern void error(char errorCode);

// 符号表
extern int localAddr;
extern string curLevel;										// 记录当前所在的符号表层次名称
extern struct sym curSym;									// 记录当前正在使用的符号
extern map<string, map<string, struct sym>> symList;		// 符号表

extern void initCurrentSymbol();
extern void insertCurrentSymbol();
extern void insertCurrentLevel();
extern void insertSymbol(string name, int kind, int type);
extern void insertSymbol(string name, struct sym tmpSymbol);

// 中间代码生成阶段
extern int nameId;
extern vector<string> stringList;
vector<midCode> midCodesForProgram;							// 全局变量的初始化
map<string, vector<midCode> > funcMidCodeTable;				//每个函数单独的中间代码	
map<string, bool> funcInlineAble; //函数是否可以内联		
vector<string> funcNameList; //函数名字
extern vector <struct midCode> midCodes;	// 中间代码

extern string genTmp();				// 获取临时变量符号
extern string genSTmp();
extern string genName();
extern string genLabel();			// 获取汇编代码标签
extern string genLabel(string app);
extern int string2int(string s);	// 字符串转化为整型
extern string int2string(int t);	// 整型转化为字符串
extern void insertMidCode(midOp op, string result, string left, string right);
extern void insertMidCode(midOp op, string result, string left, string right, string backup);

// 以下为内部定义的变量和函数
int returnedType;								// 记录函数已经返回值的类型，0为无返回值，1为整型，2为字符，3为return;
map <string, int> functionReturnType;			// 记录函数返回值类型，0为无返回值，1为整型，2为字符
map <string, int> functionParameterCount;		// 记录有返回值函数的参数数量
map <string, int[100]> functionParameterType;	// 记录有返回值函数的参数类型

// 递归调用子函数
void getAdditionOperator(int& additionOperator);				// <加法运算符>
void getMultiplicationOperator(int& multiplicationOperator);	// <乘法运算符>
void getRelationalOperator(int& tmpRelationalOperator);			// <关系运算符>
void getChar(string& c);										// <字符>
void getString(string& s);										//*<字符串>

void getProgram();									//*<程序>
void getConstantDeclaration();						//*<常量说明>
void getConstantDefination();						//*<常量定义>
void getUnsignedInteger(int& tmpUnsignedInteger);	//*<无符号整数>				
void getInteger(int& tmpInteger);					//*<整数>
			
void getIdentifier(string& identifier);				// <标识符>
void getConstant(int& tmpConstType, int& constInteger, string& constChar);		//*<常量>

void getVariableDeclaration();															//*<变量说明>
void getVariableDeclarationForProgram(int tmpTypeIdentifier);
void getVariableDefination();															//*<变量定义>
void getVariableDefinationForProgram(int tmpTypeIdentifier);
void getVariableDefinationWithoutInitialization(int tmpTypeIdentifier);					//*<变量定义无初始化>
void getVariableDefinationWithInitialization(int tmpTypeIdentifier, int dimension);		//*<变量定义及初始化>	
void getTypeIdentifier(int& typeIdentifier);											// <类型标识符>

void getFunctionDefinationWithReturnValueForProgram(int returnType);		//*<有返回值函数定义>
void getFunctionDefinationWithoutReturnValueForProgram(int returnType);		//*<无返回值函数定义>
void getCompoundStatements();												//*<复合语句>
void getParameterList();													//*<参数表>
void getMainForProgram();													//*<主函数>
	
void getExpression(int& type, string& ansTmp);			// 表达式	
void getTerm(int& type, string& ansTmp);				// 项	
void getFactor(int& type, string& ansTmp);				// 因子

void getStatement();
void getAssignmentStatement();
void getConditionalStatement();
void getCondition(string& tmpCondition);
void getLoopStatement();
void getStep(int& tmpStep);
void getSwitch();
void getCaseTable(int tmpExpressionType, string tmpExpression, string endLabel);
void getCase(int tmpExpressionType, string tmpExpression, string endLabel);
void getDefault(string endLabel);
void getFunctionCallStatementWithReturnValue(int& type);
void getFunctionCallStatementWithoutReturnValue();
void getValueParameterTable(string tmpFunctionCall);
void getStatementList();
void getScanf();
void getPrintf();
void getReturn();

// 检查symbol类型
void checkSymbol(int symType, char errorType);
void checkSymbolandGetSym(int symType, char errorType);

// 检查Identifier
void checkUndefinedIdentifier(string tmpIdentifier);

// 处理内联函数
void checkBeforeFunc();
void fullNameMap(map<string, string>& nameMap, vector<midCode> ve, string funcName);
void dealInlineFunc(string name, int& begin, int& end);