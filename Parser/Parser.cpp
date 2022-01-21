#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
using namespace std;

extern void getsym();
extern void trysym();
extern void ungetsym();
extern void outputsym();
extern void outputlastsym();
extern void outputlastlastsym();
extern void error();

extern ofstream ofile;
extern int symbol;				// 保存当前所识别单词的类型
extern char TOKEN[256];			// 存放单词字符串

enum classnum {
	IDENFR, INTCON, CHARCON, STRCON, CONSTTK, INTTK, CHARTK, VOIDTK, MAINTK, IFTK, ELSETK, SWITCHTK, CASETK, DEFAULTTK, WHILETK, FORTK, SCANFTK, PRINTFTK,
	RETURNTK, PLUS, MINU, MULT, DIV, LSS, LEQ, GRE, GEQ, EQL, NEQ, COLON, ASSIGN, SEMICN, COMMA, LPARENT, RPARENT, LBRACK, RBRACK, LBRACE, RBRACE
};

int dimension = 0;					// 变量定义的数组维数

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

// 保存上一个标识符，保存到函数列表中
string lastIdentifier;
set<string> functionWithReturnValue;
set<string> functionWithoutReturnValue;

void getAdditionOperator() {
	if (!(symbol == PLUS || symbol == MINU)) {
		error();
	}
	getsym();
}

void getMultiplicationOperator() {
	if (!(symbol == MULT || symbol == DIV)) {
		error();
	}
	getsym();
}

void getRelationalOperator() {
	if (!(symbol == LSS || symbol == LEQ || symbol == GRE || symbol == GEQ || symbol == EQL || symbol == NEQ)) {
		error();
	}
	getsym();
}

void getLetter() {
	return;
}

void getDigit() {
	return;
}

void getChar() {
	if (symbol != CHARCON) {
		error();
	}
	getsym();
}

void getString() {
	if (symbol != STRCON) {
		error();
	}
	getsym();
	ofile << "<字符串>" << endl;
}

void getProgram() {
	int hasVariable = 0;	// 判断是否有变量说明
	
	// 常量说明
	if (symbol == CONSTTK) {
		getConstantDeclaration();
	}

	// 变量说明
	while (symbol == INTTK || symbol == CHARTK) {
		trysym();
		trysym();
		if (symbol != LPARENT) {
			hasVariable++;
			ungetsym();
			outputlastsym();
			getVariableDeclarationForProgram();
		}
		else {	
			ungetsym();
			ungetsym();
			break;
		}
	}
	if (hasVariable > 0) {
		ofile << "<变量说明>" << endl;
	}

	int hasReturn = 0;	// 判断是否为有返回值函数
	// 有返回值函数定义和无返回值函数定义
	while (symbol == INTTK || symbol == CHARTK || symbol == VOIDTK) {
		if (symbol == INTTK || symbol == CHARTK) {
			hasReturn = 1;
		}
		else {
			hasReturn = 0;
		}
		
		getsym();
		if (symbol == MAINTK) {
			break;
		}

		if (hasReturn == 1) {
			getFunctionDefinationWithReturnValueForProgram();
		}
		else if (hasReturn == 0) {
			getFunctionDefinationWithoutReturnValueForProgram();
		}
	}

	// 主函数
	getMainForProgram();

	ofile << "<程序>" << endl;
}

void getConstantDeclaration() {
	while (symbol == CONSTTK) {
		getsym();
		getConstantDefination();
		if (symbol != SEMICN) {
			error();
		}
		getsym();
	}

	ofile << "<常量说明>" << endl;
}

void getConstantDefination() {
	if (symbol == INTTK) {
		do {
			getsym();	// 标识符
			if (symbol != IDENFR) {
				error();
			}

			getsym();	// =
			if (symbol != ASSIGN) {
				error();
			}

			getsym();	// 整数
			getInteger();
		} while (symbol == COMMA);
	}
	else if (symbol == CHARTK) {
		do {
			getsym();	// 标识符
			if (symbol != IDENFR) {
				error();
			}

			getsym();	// =
			if (symbol != ASSIGN) {
				error();
			}

			getsym();	// 字符
			if (symbol != CHARCON) {
				error();
			}

			getsym();	// , or else
		} while (symbol == COMMA);
	}

	ofile << "<常量定义>" << endl;
}

void getUnsignedInteger() {
	if (symbol != INTCON) {
		error();
	}
	getsym();
	ofile << "<无符号整数>" << endl;
}

void getInteger() {
	// [+|-]
	if (symbol == PLUS || symbol == MINU) {
		getAdditionOperator();
	}
	getUnsignedInteger();
	ofile << "<整数>" << endl;
}

void getIdentifier() {
	if (symbol != IDENFR) {
		error();
	}
	lastIdentifier = TOKEN;
	getsym();
}

void getDeclarationHeader() {
	if (symbol != INTTK && symbol != CHARTK) {
		error();
	}
	getIdentifier();
	ofile << "<声明头部>" << endl;
}

void getConstant() {
	// 整数或者字符
	if (symbol == INTCON || symbol == PLUS || symbol == MINU) {
		getInteger();
	}
	else if (symbol == CHARCON) {
		getChar();
	}
	else {
		error();
	}
	ofile << "<常量>" << endl;
}

void getVariableDeclaration() {
	while (symbol == INTTK || symbol == CHARTK) {
		getVariableDefination();
		if (symbol != SEMICN) {
			error();
		}
		getsym();
	}
	ofile << "<变量说明>" << endl;
}

void getVariableDeclarationForProgram() {
	getVariableDefinationForProgram();
	if (symbol != SEMICN) {
		error();
	}
	getsym();
}

void getVariableDefination() {
	dimension = 0;	// 初始化数组维数

	// 变量定义的共同部分
	getTypeIdentifier();
	getIdentifier();
	
	if (symbol == LBRACK) {
		getsym();
		getUnsignedInteger();
		if (symbol != RBRACK) {
			error();
		}
		dimension++;
		getsym();
	}

	if (symbol == LBRACK) {
		getsym();
		getUnsignedInteger();
		if (symbol != RBRACK) {
			error();
		}
		dimension++;
		getsym();
	}

	// 判断是否进行初始化
	if (symbol == ASSIGN) {
		getVariableDefinationWithInitialization();
	}
	else {
		getVariableDefinationWithoutInitialization();
	}

	ofile << "<变量定义>" << endl;
}

void getVariableDefinationForProgram() {
	dimension = 0;	// 初始化数组维数

	// 变量定义的共同部分
	getIdentifier();
	if (symbol == LBRACK) {
		getsym();
		getUnsignedInteger();
		if (symbol != RBRACK) {
			error();
		}
		dimension++;
		getsym();
		if (symbol == LBRACK) {
			getsym();
			getUnsignedInteger();
			if (symbol != RBRACK) {
				error();
			}
			dimension++;
			getsym();
		}
	}

	// 判断是否进行初始化
	if (symbol == ASSIGN) {
		getVariableDefinationWithInitialization();
	}
	else {
		getVariableDefinationWithoutInitialization();
	}

	ofile << "<变量定义>" << endl;
}

void getVariableDefinationWithoutInitialization() {
	while (symbol == COMMA) {
		getsym();
		getIdentifier();
		if (symbol == LBRACK) {
			getsym();
			getUnsignedInteger();
			if (symbol != RBRACK) {
				error();
			}
			getsym();
		}
		if (symbol == LBRACK) {
			getsym();
			getUnsignedInteger();
			if (symbol != RBRACK) {
				error();
			}
			getsym();
		}
	}

	ofile << "<变量定义无初始化>" << endl;
}

void getVariableDefinationWithInitialization() {
	getsym();

	if (dimension == 0) {
		getConstant();
	} else if (dimension ==1) {
		if (symbol != LBRACE) {
			error();
		}
		getsym();
		getConstant();

		while (symbol == COMMA) {
			getsym();
			getConstant();
		}

		if (symbol != RBRACE) {
			error();
		}
		getsym();
	}
	else if (dimension == 2) {
		if (symbol != LBRACE) {
			error();
		}
		getsym();
		if (symbol != LBRACE) {
			error();
		}
		getsym();

		getConstant();
		while (symbol == COMMA) {
			getsym();
			getConstant();
		}

		if (symbol != RBRACE) {
			error();
		}
		getsym();

		while (symbol == COMMA) {
			getsym();
			if (symbol != LBRACE) {
				error();
			}
			getsym();

			getConstant();
			while (symbol == COMMA) {
				getsym();
				getConstant();
			}

			if (symbol != RBRACE) {
				error();
			}
			getsym();
		}

		if (symbol != RBRACE) {
			error();
		}
		getsym();
	}	

	ofile << "<变量定义及初始化>" << endl;
}

void getTypeIdentifier() {
	if (symbol != INTTK && symbol != CHARTK) {
		error();
	}
	getsym();
}

void getFunctionDefinationWithReturnValue() {
	getDeclarationHeader();
	functionWithReturnValue.insert(lastIdentifier);
	if (symbol != LPARENT) {
		error();
	}
	getsym();
	getParameterList();
	if (symbol != RPARENT) {
		error();
	}
	getsym();
	if (symbol != LBRACE) {
		error();
	}
	getsym();
	getCompoundStatements();
	if (symbol != RBRACE) {
		error();
	}
	getsym();
	ofile << "<有返回值函数定义>" << endl;
}

void getFunctionDefinationWithReturnValueForProgram() {
	getIdentifier();
	ofile << "<声明头部>" << endl;		// 类型标识符在getProgram中识别
	functionWithReturnValue.insert(lastIdentifier);
	if (symbol != LPARENT) {
		error();
	}
	getsym();
	getParameterList();
	if (symbol != RPARENT) {
		error();
	}
	getsym();
	if (symbol != LBRACE) {
		error();
	}
	getsym();
	getCompoundStatements();
	if (symbol != RBRACE) {
		error();
	}
	getsym();
	ofile << "<有返回值函数定义>" << endl;
}

void getFunctionDefinationWithoutReturnValue() {
	if (symbol != VOIDTK) {
		error();
	}
	getsym();
	getIdentifier();
	functionWithoutReturnValue.insert(lastIdentifier);
	if (symbol != LPARENT) {
		error();
	}
	getsym();
	getParameterList();
	if (symbol != RPARENT) {
		error();
	}
	getsym();
	if (symbol != LBRACE) {
		error();
	}
	getsym();
	getCompoundStatements();
	if (symbol != RBRACE) {
		error();
	}
	getsym();
	ofile << "<无返回值函数定义>" << endl;
}

void getFunctionDefinationWithoutReturnValueForProgram() {
	getIdentifier();
	functionWithoutReturnValue.insert(lastIdentifier);
	if (symbol != LPARENT) {
		error();
	}
	getsym();
	getParameterList();
	if (symbol != RPARENT) {
		error();
	}
	getsym();
	if (symbol != LBRACE) {
		error();
	}
	getsym();
	getCompoundStatements();
	if (symbol != RBRACE) {
		error();
	}
	getsym();
	ofile << "<无返回值函数定义>" << endl;
}

void getCompoundStatements() {
	if (symbol == CONSTTK) {
		getConstantDeclaration();
	}
	if (symbol == INTTK || symbol == CHARTK) {
		getVariableDeclaration();
	}
	getStatementList();
	ofile << "<复合语句>" << endl;
}

void getParameterList() {
	if (symbol == INTTK || symbol == CHARTK) {
		getTypeIdentifier();
		getIdentifier();
		while (symbol == COMMA) {
			getsym();
			getTypeIdentifier();
			getIdentifier();
		}
	}
	ofile << "<参数表>" << endl;
}

void getMain() {
	if (symbol != VOIDTK) {
		error();
	}
	getsym();
	if (symbol != MAINTK) {
		error();
	}
	getsym();
	if (symbol != LPARENT) {
		error();
	}
	getsym();
	if (symbol != RPARENT) {
		error();
	}
	getsym();
	if (symbol != LBRACE) {
		error();
	}
	getsym();
	getCompoundStatements();
	if (symbol != RBRACE) {
		error();
	}
	getsym();
	ofile << "<主函数>" << endl;
}

void getMainForProgram() {
	if (symbol != MAINTK) {
		error();
	}
	getsym();
	if (symbol != LPARENT) {
		error();
	}
	getsym();
	if (symbol != RPARENT) {
		error();
	}
	getsym();
	if (symbol != LBRACE) {
		error();
	}
	getsym();
	getCompoundStatements();
	if (symbol != RBRACE) {
		error();
	}
	getsym();
	ofile << "<主函数>" << endl;
}

void getExpression() {
	if (symbol == PLUS || symbol == MINU) {
		getAdditionOperator();
	}
	getTerm();
	while (symbol == PLUS || symbol == MINU) {
		getAdditionOperator();
		getTerm();
	}
	ofile << "<表达式>" << endl;
}

void getTerm() {
	getFactor();
	while (symbol == MULT || symbol == DIV) {
		getMultiplicationOperator();
		getFactor();
	}
	ofile << "<项>" << endl;
}

void getFactor() {
	if (symbol == IDENFR) {
		// 可能为变量或者有返回值函数调用语句
		trysym();

		if (symbol == LPARENT) {
			ungetsym();
			getFunctionCallStatementWithReturnValue();
		}
		else {
			ungetsym();
			getIdentifier();
			if (symbol == LBRACK) {
				getsym();
				getExpression();
				if (symbol != RBRACK) {
					error();
				}
				getsym();
			}
			if (symbol == LBRACK) {
				getsym();
				getExpression();
				if (symbol != RBRACK) {
					error();
				}
				getsym();
			}
		}
	}
	else if (symbol == LPARENT) {
		getsym();
		getExpression();
		if (symbol != RPARENT) {
			error();
		}
		getsym();
	}
	else if (symbol == INTCON || symbol == PLUS || symbol == MINU) {
		getInteger();
	}
	else if (symbol == CHARCON) {
		getChar();
	}
	else {
		error();
	}
	ofile << "<因子>" << endl;
}

void getStatement() {
	if (symbol == WHILETK || symbol == FORTK) {
		getLoopStatement();
	}
	else if (symbol == IFTK) {
		getConditionalStatement();
	}
	else if (symbol == IDENFR) {
		string tmpIdenfr = TOKEN;

		if (functionWithReturnValue.count(tmpIdenfr) > 0) {
			getFunctionCallStatementWithReturnValue();
		}
		else if (functionWithoutReturnValue.count(tmpIdenfr) > 0) {
			getFunctionCallStatementWithoutReturnValue();
		}
		else {
			getAssignmentStatement();
		}
		
		if (symbol != SEMICN) {
			error();
		}
		getsym();
	}
	else if (symbol == SCANFTK) {
		getScanf();
		if (symbol != SEMICN) {
			error();
		}
		getsym();
	}
	else if (symbol == PRINTFTK) {
		getPrintf();
		if (symbol != SEMICN) {
			error();
		}
		getsym();
	}
	else if (symbol == SWITCHTK) {
		getSwitch();
	}
	else if (symbol == SEMICN) {
		if (symbol != SEMICN) {
			error();
		}
		getsym();
	}
	else if (symbol == RETURNTK) {
		getReturn();
		if (symbol != SEMICN) {
			error();
		}
		getsym();
	}
	else if (symbol == LBRACE) {
		getsym();
		getStatementList();
		if (symbol != RBRACE) {
			error();
		}
		getsym();
	}
	ofile << "<语句>" << endl;
}

void getAssignmentStatement() {
	getIdentifier();

	if (symbol == LBRACK){
		getsym();
		getExpression();
		if (symbol != RBRACK) {
			error();
		}
		getsym();
	}

	if (symbol == LBRACK) {
		getsym();
		getExpression();
		if (symbol != RBRACK) {
			error();
		}
		getsym();
	}

	if (symbol != ASSIGN) {
		error();
	}
	getsym();
	getExpression();
	ofile << "<赋值语句>" << endl;
}

void getConditionalStatement() {
	if (symbol != IFTK) {
		error();
	}
	getsym();
	if (symbol != LPARENT) {
		error();
	}
	getsym();
	getCondition();
	if (symbol != RPARENT) {
		error();
	}
	getsym();
	getStatement();
	while (symbol == ELSETK) {
		getsym();
		getStatement();
	}
	ofile << "<条件语句>" << endl;
}

void getCondition() {
	getExpression();
	getRelationalOperator();
	getExpression();
	ofile << "<条件>" << endl;
}

void getLoopStatement() {
	if (symbol == WHILETK) {
		getsym();
		if (symbol != LPARENT) {
			error();
		}
		getsym();
		getCondition();
		if (symbol != RPARENT) {
			error();
		}
		getsym();
		getStatement();
	}
	else if (symbol == FORTK) {
		getsym();
		if (symbol != LPARENT) {
			error();
		}
		getsym();
		getIdentifier();
		if (symbol != ASSIGN) {
			error();
		}
		getsym();
		getExpression();

		if (symbol != SEMICN) {
			error();
		}
		getsym();
		
		getCondition();
		
		if (symbol != SEMICN) {
			error();
		}
		getsym();
		
		getIdentifier();
		if (symbol != ASSIGN) {
			error();
		}
		getsym();
		getIdentifier();
		getAdditionOperator();
		getStep();
		if (symbol != RPARENT) {
			error();
		}
		getsym();
		getStatement();
	}
	ofile << "<循环语句>" << endl;
}

void getStep() {
	getUnsignedInteger();
	ofile << "<步长>" << endl;
}

void getSwitch() {
	if (symbol != SWITCHTK) {
		error();
	}
	getsym();
	if (symbol != LPARENT) {
		error();
	}
	getsym();
	getExpression();
	if (symbol != RPARENT) {
		error();
	}
	getsym();
	if (symbol != LBRACE) {
		error();
	}
	getsym();
	getCaseTable();
	getDefault();
	if (symbol != RBRACE) {
		error();
	}
	getsym();
	ofile << "<情况语句>" << endl;
}

void getCaseTable() {
	while (symbol == CASETK) {
		getCase();
	}
	ofile << "<情况表>" << endl;
}

void getCase() {
	if (symbol != CASETK) {
		error();
	}
	getsym();
	getConstant();
	if (symbol != COLON) {
		error();
	}
	getsym();
	getStatement();
	ofile << "<情况子语句>" << endl;
}

void getDefault() {
	if (symbol != DEFAULTTK) {
		error();
	}
	getsym();
	if (symbol != COLON) {
		error();
	}
	getsym();
	getStatement();
	ofile << "<缺省>" << endl;
}

void getFunctionCallStatementWithReturnValue() {
	getIdentifier();
	if (symbol != LPARENT) {
		error();
	}
	getsym();
	getValueParameterTable();
	if (symbol != RPARENT) {
		error();
	}
	getsym();
	ofile << "<有返回值函数调用语句>" << endl;
}

void getFunctionCallStatementWithoutReturnValue() {
	getIdentifier();
	if (symbol != LPARENT) {
		error();
	}
	getsym();
	getValueParameterTable();
	if (symbol != RPARENT) {
		error();
	}
	getsym();
	ofile << "<无返回值函数调用语句>" << endl;
}

void getValueParameterTable() {
	if (symbol == RPARENT) {
		// 值参数表为空，直接返回
		ofile << "<值参数表>" << endl;
		return;
	}
	else {
		getExpression();
		while (symbol == COMMA) {
			getsym();
			getExpression();
		}
		ofile << "<值参数表>" << endl;
	}
}

void getStatementList() {
	while (symbol != RBRACE) {
		getStatement();
	}
	ofile << "<语句列>" << endl;
}

void getScanf() {
	if (symbol != SCANFTK) {
		error();
	}
	getsym();
	if (symbol != LPARENT) {
		error();
	}
	getsym();
	getIdentifier();
	if (symbol != RPARENT) {
		error();
	}
	getsym();
	ofile << "<读语句>" << endl;
}

void getPrintf() {
	if (symbol != PRINTFTK) {
		error();
	}
	getsym();
	if (symbol != LPARENT) {
		error();
	}
	getsym();

	if (symbol == STRCON) {
		getString();
		if (symbol == COMMA) {
			getsym();
			getExpression();
		}
	}
	else {
		getExpression();
	}

	if (symbol != RPARENT) {
		error();
	}
	getsym();
	ofile << "<写语句>" << endl;
}

void getReturn() {
	if (symbol != RETURNTK) {
		error();
	}
	getsym();
	if (symbol == LPARENT) {
		getsym();
		getExpression();
		if (symbol != RPARENT) {
			error();
		}
		getsym();
	}
	ofile << "<返回语句>" << endl;
}