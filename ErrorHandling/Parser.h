#pragma once

enum classnum {
	IDENFR, INTCON, CHARCON, STRCON, CONSTTK, INTTK, CHARTK, VOIDTK, MAINTK, IFTK, ELSETK, SWITCHTK, CASETK, DEFAULTTK, WHILETK, FORTK, SCANFTK, PRINTFTK,
	RETURNTK, PLUS, MINU, MULT, DIV, LSS, LEQ, GRE, GEQ, EQL, NEQ, COLON, ASSIGN, SEMICN, COMMA, LPARENT, RPARENT, LBRACK, RBRACK, LBRACE, RBRACE
};

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
void getInteger();					//*<整数>

void getIdentifier();				// <标识符>
void getDeclarationHeader();		//*<声明头部>
void getConstant();					//*<常量>
void getVariableDeclaration();		//*<变量说明>
void getVariableDefination();		//*<变量定义>
void getVariableDefinationWithoutInitialization();	//*<变量定义无初始化>
void getVariableDefinationWithInitialization();		//*<变量定义及初始化>
void getTypeIdentifier();			// <类型标识符>

void getFunctionDefinationWithReturnValue();	//*<有返回值函数定义>
void getFunctionDefinationWithoutReturnValue();	//*<无返回值函数定义>
void getCompoundStatements();					//*<复合语句>
void getParameterList();						//*<参数表>
void getMain();									//*<主函数>

void getExpression();	// 表达式
void getTerm();			// 项
void getFactor();		// 因子

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