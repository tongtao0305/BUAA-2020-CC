#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include "Parser.h"
#define MAIN "Main"
#define PROGRAM "Program"
using namespace std;


// 词法变量
extern ofstream ofile;			// 输出文件流
extern int symbol;				// 记录当前Token的类型
extern char TOKEN[256];			// 记录当前Token的字符串
extern int lastSymbol;			// 记录上一个Token的类型
extern char lastTOKEN[256];		// 记录上一个Token的字符串
extern int lastlastSymbol;		// 记录上上个Token的类型
extern char lastlastTOKEN[256];	// 记录上上个Token的字符串
extern int cnt_line;			// 记录当前缓冲区在源文件中的行数，以便进行错误输出

// 词法分析函数
extern void getsym();
extern void trysym();
extern void ungetsym();
extern void outputsym();
extern void outputlastsym();
extern void outputlastlastsym();

// 错误处理函数
extern void error(char errorCode);

// 符号表相关函数
extern void initCurrentSymbol();
extern void insertCurrentSymbol();
extern void insertCurrentLevel();

// 符号表
struct sym {
	// 以下为通用参数
	string name;
	int kind;	// 符号的种类，0为程序常量，1为程序变量，2为函数名，3为函数参数表
	int type;	// 符号的类型，0为int，1为char
	int dimension;	// 若为数组类型，记录其维度；若为函数类型，记录其参数数量

	// 以下为特殊参数
	int num;	// 记录数值型符号的值
	string str;	// 记录字符或字符串型符号的值
	int index;		// 若为数组类型，记录其行数
	int columns;	// 若为数组类型，记录其列数
};

// 符号表相关变量
extern map<string, map<string, struct sym>> symList;		// 符号表
extern struct sym curSym;									// 记录当前正在使用的符号
extern string curLevel;										// 记录当前所在的符号表层次名称
extern map<string, struct sym> curMap;						// 记录当前所在的符号表层次的map

// 语法分析相关变量
int dimension = 0;					// 变量定义的数组维数
int indexCount = 0;						// 无初始化变量的数组的行数
int columnsCount = 0;					// 有初始化变量的数组的列数

// 保存上一个标识符，保存到函数列表中
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


void getAdditionOperator() {
	if (!(symbol == PLUS || symbol == MINU)) {
		error('0');
	}
	getsym();
}

void getMultiplicationOperator() {
	if (!(symbol == MULT || symbol == DIV)) {
		error('0');
	}
	getsym();
}

void getRelationalOperator() {
	if (!(symbol == LSS || symbol == LEQ || symbol == GRE || symbol == GEQ || symbol == EQL || symbol == NEQ)) {
		error('0');
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
		error('0');
	}
	lastChar = TOKEN;
	getsym();
}

void getString() {
	if (symbol != STRCON) {
		error('0');
	}
	getsym();
	//ofile << "<字符串>" << endl;
}

void getProgram() {
	int hasVariable = 0;	// 判断是否有变量说明
	
	// 更新符号表的层次，此处为最外层
	curLevel = PROGRAM;
	insertCurrentLevel();

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
			//outputlastsym();
			getVariableDeclarationForProgram();
		}
		else {	
			ungetsym();
			ungetsym();
			break;
		}
	}
	if (hasVariable > 0) {
		//ofile << "<变量说明>" << endl;
	}

	// 有返回值函数定义和无返回值函数定义
	while (symbol == INTTK || symbol == CHARTK || symbol == VOIDTK) {
		if (symbol == INTTK) {
			hasReturn = 1;
		}
		else if (symbol == CHARTK) {
			hasReturn = 2;
		}
		else {
			hasReturn = 0;
		}
		
		getsym();
		if (symbol == MAINTK) {
			break;
		}

		if (hasReturn > 0) {
			getFunctionDefinationWithReturnValueForProgram();
		}
		else if (hasReturn == 0) {
			getFunctionDefinationWithoutReturnValueForProgram();
		}
	}

	// 主函数
	getMainForProgram();

	//ofile << "<程序>" << endl;
}

void getConstantDeclaration() {
	while (symbol == CONSTTK) {
		getsym();
		getConstantDefination();	// 在该子程序中添加常量到符号表
		if (symbol != SEMICN) {
			error('k');
		}
		else {
			getsym();
		}
		
	}

	//ofile << "<常量说明>" << endl;
}

void getConstantDefination() {
	if (symbol == INTTK) {

		// 准备添加当前常量到符号表
		initCurrentSymbol();
		curSym.kind = 0;
		curSym.type = 1;	// 符号表相关，一维int
		
		do {
			getsym();	// 标识符
			if (symbol != IDENFR) {
				error('0');
			}
			curSym.name = TOKEN;		// 更新当前符号的名字，即标识符的名字

			getsym();	// =
			if (symbol != ASSIGN) {
				error('0');
			}

			getsym();	// 整数
			getInteger();

			curSym.num = lastInteger;	// 更新当前符号的整数值
			insertCurrentSymbol();
		} while (symbol == COMMA);
	}
	else if (symbol == CHARTK) {
		
		// 准备添加当前常量到符号表
		initCurrentSymbol();
		curSym.kind = 0;
		curSym.type = 4;	// 符号表相关，一维char
		
		do {
			getsym();	// 标识符
			if (symbol != IDENFR) {
				error('0');
			}
			curSym.name = TOKEN;		// 更新当前符号的名字，即标识符的名字

			getsym();	// =
			if (symbol != ASSIGN) {
				error('0');
			}

			getsym();	// 字符
			if (symbol != CHARCON) {
				error('0');
			}

			curSym.str = TOKEN;			// 更新当前符号的字符值
			insertCurrentSymbol();

			getsym();	// , or else
		} while (symbol == COMMA);
	}

	//ofile << "<常量定义>" << endl;
}

void getUnsignedInteger() {
	if (symbol != INTCON) {
		error('0');
	}
	lastUnsignedInteger = atoi(TOKEN);						// 记录上一个识别的无符号整数
	lastInteger = lastOperator * lastUnsignedInteger;		// 记录上一个识别的整数
	curSym.num = lastInteger;
	getsym();
	//ofile << "<无符号整数>" << endl;
}

void getInteger() {
	// 识别符号部分
	lastOperator = 1;
	if (symbol == PLUS) {
		lastOperator = 1;
		getsym();
	}
	else if (symbol == MINU) {
		lastOperator = -1;
		getsym();
	}
	// 识别无符号整数部分
	getUnsignedInteger();
	//ofile << "<整数>" << endl;
}

void getIdentifier() {
	if (symbol != IDENFR) {
		error('0');
	}
	lastIdentifier = TOKEN;
	getsym();
}

void getDeclarationHeader() {
	if (symbol != INTTK && symbol != CHARTK) {
		error('0');
	}
	getIdentifier();
	//ofile << "<声明头部>" << endl;
}

void getConstant() {
	// 整数或者字符
	if (symbol == INTCON || symbol == PLUS || symbol == MINU) {
		constType = 0;
		getInteger();
	}
	else if (symbol == CHARCON) {
		constType = 1;
		getChar();
	}
	else {
		error('0');
	}
	//ofile << "<常量>" << endl;
}

void getVariableDeclaration() {
	while (symbol == INTTK || symbol == CHARTK) {
		getVariableDefination();
		if (symbol != SEMICN) {
			error('k');
		}
		else {
			getsym();
		}
		
	}
	//ofile << "<变量说明>" << endl;
}

void getVariableDeclarationForProgram() {
	getVariableDefinationForProgram();
	if (symbol != SEMICN) {
		error('k');
	}
	else {
		getsym();
	}
}

void getVariableDefination() {
	dimension = 0;	// 初始化数组维数

	// 变量定义的共同部分
	getTypeIdentifier();
	getIdentifier();

	// 初始化当前符号
	initCurrentSymbol();
	if (lastTypeIdentifier == INTTK) {
		curSym.type = 1;
		lastConstantType = 0;
		lastVariableType = 1;
	}
	else if (lastTypeIdentifier == CHARTK) {
		curSym.type = 4;
		lastConstantType = 1;
		lastVariableType = 4;
	}
	curSym.name = lastIdentifier;
	
	// 接下来两个判断语句用来判断数组维度
	if (symbol == LBRACK) {
		getsym();
		getUnsignedInteger();

		curSym.index = lastUnsignedInteger;		// 记录当前变量定义的行数
		
		if (symbol != RBRACK) {
			error('m');
		}
		else {
			getsym();
		}
		dimension++;
	}
	if (symbol == LBRACK) {
		getsym();
		getUnsignedInteger();

		curSym.columns = lastUnsignedInteger;		// 记录当前变量定义的列数

		if (symbol != RBRACK) {
			error('m');
		}
		else {
			getsym();
		}
		dimension++;
	}

	// 更新当前符号保存的数组维度
	curSym.type = curSym.type + dimension;

	// 判断是否进行初始化
	if (symbol == ASSIGN) {
		curSym.kind = 1;		// 更新当前符号类型，有初始化的变量定义
		getVariableDefinationWithInitialization();
		insertCurrentSymbol();	// 变量定义及初始化每行仅有一个
	}
	else {
		curSym.kind = 2;		// 更新当前符号类型，无初始化的变量定义
		insertCurrentSymbol();	// 由于变量定义无初始化每行能有多个，所以先将当前的加入符号表
		getVariableDefinationWithoutInitialization();
	}

	//ofile << "<变量定义>" << endl;
}

void getVariableDefinationForProgram() {
	dimension = 0;	// 初始化数组维数

	// 变量定义的共同部分
	getIdentifier();

	// 初始化当前符号
	initCurrentSymbol();
	if (lastlastSymbol == INTTK) {
		curSym.type = 1;
		lastConstantType = 0;
		lastVariableType = 1;
	}
	else if (lastlastSymbol == CHARTK) {
		curSym.type = 4;
		lastConstantType = 1;
		lastVariableType = 4;
	}
	curSym.name = lastIdentifier;

	// 接下来两个判断语句用来判断数组维度
	if (symbol == LBRACK) {
		getsym();
		getUnsignedInteger();

		curSym.index = lastUnsignedInteger;		// 记录当前变量定义的行数

		if (symbol != RBRACK) {
			error('m');
		}
		else {
			getsym();
		}
		dimension++;
	}
	if (symbol == LBRACK) {
		getsym();
		getUnsignedInteger();

		curSym.columns = lastUnsignedInteger;		// 记录当前变量定义的列数

		if (symbol != RBRACK) {
			error('m');
		}
		else {
			getsym();
		}
		dimension++;
	}
	
	// 更新当前符号保存的数组维度
	curSym.type = curSym.type + dimension;

	// 判断是否进行初始化
	if (symbol == ASSIGN) {
		curSym.kind = 1;		// 更新当前符号类型，有初始化的变量定义
		getVariableDefinationWithInitialization();
		insertCurrentSymbol();
	}
	else {
		curSym.kind = 2;		// 更新当前符号类型，无初始化的变量定义
		insertCurrentSymbol();
		getVariableDefinationWithoutInitialization();
	}

	//ofile << "<变量定义>" << endl;
}

void getVariableDefinationWithoutInitialization() {
	while (symbol == COMMA) {
		dimension = 0;		// 初始化维度

		getsym();
		getIdentifier();
		
		// 初始化当前符号
		initCurrentSymbol();
		curSym.kind = 2;		// 更新当前符号类型，无初始化的变量定义
		curSym.type = lastVariableType;
		curSym.name = lastIdentifier;
		
		if (symbol == LBRACK) {
			getsym();
			getUnsignedInteger();

			curSym.index = lastUnsignedInteger;		// 记录当前变量定义的行数

			if (symbol != RBRACK) {
				error('m');
			}
			else {
				getsym();
			}
			dimension++;
		}
		if (symbol == LBRACK) {
			getsym();
			getUnsignedInteger();

			curSym.columns = lastUnsignedInteger;		// 记录当前变量定义的列数

			if (symbol != RBRACK) {
				error('m');
			}
			else {
				getsym();
			}
			dimension++;
			
		}

		// 更新当前符号保存的数组维度
		curSym.type = curSym.type + dimension;
		insertCurrentSymbol();
	}

	//ofile << "<变量定义无初始化>" << endl;
}

void getVariableDefinationWithInitialization() {
	// 初始化临时变量
	indexCount = 0;
	columnsCount = 0;
	
	getsym();

	if (dimension == 0) {
		getConstant();
		if (constType != lastConstantType) {
			error('o');
		}

	} else if (dimension ==1) {
		if (symbol != LBRACE) {
			error('0');
		}
		getsym();
		getConstant();
		if (constType != lastConstantType) {
			error('o');
		}
		indexCount++;

		while (symbol == COMMA) {
			getsym();
			getConstant();
			if (constType != lastConstantType) {
				error('o');
			}
			indexCount++;
		}

		if (symbol != RBRACE) {
			error('0');
		}

		if (indexCount != curSym.index) {
			error('n');
		}

		getsym();
	}
	else if (dimension == 2) {
		if (symbol != LBRACE) {
			error('0');
		}
		getsym();
		if (symbol != LBRACE) {
			error('0');
		}
		getsym();
		getConstant();
		if (constType != lastConstantType) {
			error('o');
		}
		columnsCount++;
		while (symbol == COMMA) {
			getsym();
			getConstant();
			if (constType != lastConstantType) {
				error('o');
			}
			columnsCount++;
		}

		if (symbol != RBRACE) {
			error('0');
		}
		indexCount++;

		// 判断数组的列数是否相同
		if (columnsCount != curSym.columns) {
			error('n');
		}

		getsym();

		while (symbol == COMMA) {
			columnsCount = 0;		// 初始化列变量
			getsym();
			if (symbol != LBRACE) {
				error('0');
			}
			getsym();
			getConstant();
			if (constType != lastConstantType) {
				error('o');
			}
			columnsCount++;
			while (symbol == COMMA) {
				getsym();
				getConstant();
				if (constType != lastConstantType) {
					error('o');
				}
				columnsCount++;
			}

			if (symbol != RBRACE) {
				error('0');
			}
			indexCount++;		// 行变量叠加

			// 判断数组的列数是否相同
			if (columnsCount != curSym.columns) {
				error('n');
			}

			getsym();
		}

		if (symbol != RBRACE) {
			error('0');
		}

		// 判断数组的行数是否相同
		if (indexCount != curSym.index) {
			error('n');
		}

		getsym();
	}	

	//ofile << "<变量定义及初始化>" << endl;
}

void getTypeIdentifier() {
	if (symbol == INTTK || symbol == CHARTK)
		lastTypeIdentifier = symbol;
	else 
		error('0');
	getsym();
}

void getFunctionDefinationWithReturnValue() {
	returnedType = 0;	// 初始化已经返回的参数类型
	getDeclarationHeader();
	functionWithReturnValue.insert(lastIdentifier);
	functionReturnType[lastIdentifier] = hasReturn;


	// 将该函数名添加到符号表
	initCurrentSymbol();
	curSym.kind = 3;
	curSym.type = 0;
	curSym.name = lastIdentifier;
	insertCurrentSymbol();

	// 更新符号表，增加新的层次
	curLevel = lastIdentifier;
	insertCurrentLevel();

	if (symbol != LPARENT) {
		error('0');
	}
	getsym();
	getParameterList();
	if (symbol != RPARENT) {
		error('l');
	}
	else {
		getsym();
	}
	if (symbol != LBRACE) {
		error('0');
	}
	getsym();
	getCompoundStatements();
	if (symbol != RBRACE) {
		error('0');
	}

	// 有返回值函数缺少返回语句
	if (functionWithReturnValue.count(curLevel) > 0 && returnedType == 0) {
		error('h');
	}

	curLevel = PROGRAM;		// 返回外层函数

	getsym();
	//ofile << "<有返回值函数定义>" << endl;
}

void getFunctionDefinationWithReturnValueForProgram() {
	returnedType = 0;	// 初始化已经返回的参数类型

	getIdentifier();
	//ofile << "<声明头部>" << endl;		// 类型标识符在getProgram中识别
	functionWithReturnValue.insert(lastIdentifier);
	functionReturnType[lastIdentifier] = hasReturn;

	// 将该函数名添加到符号表
	initCurrentSymbol();
	curSym.kind = 3;
	curSym.type = 0;
	curSym.name = lastIdentifier;
	insertCurrentSymbol();

	// 更新符号表，增加新的层次
	curLevel = lastIdentifier;
	insertCurrentLevel();

	if (symbol != LPARENT) {
		error('0');
	}
	getsym();
	getParameterList();
	if (symbol != RPARENT) {
		error('l');
	}
	else {
		getsym();
	}
	if (symbol != LBRACE) {
		error('0');
	}
	getsym();
	getCompoundStatements();
	if (symbol != RBRACE) {
		error('0');
	}

	// 有返回值函数缺少返回语句
	if (functionWithReturnValue.count(curLevel) > 0 && returnedType == 0) {
		error('h');
	}

	curLevel = PROGRAM;		// 返回外层函数

	getsym();
	//ofile << "<有返回值函数定义>" << endl;
}

void getFunctionDefinationWithoutReturnValue() {
	returnedType = 0;	// 初始化已经返回的参数类型
	if (symbol != VOIDTK) {
		error('0');
	}
	getsym();
	getIdentifier();
	functionWithoutReturnValue.insert(lastIdentifier);
	functionReturnType[lastIdentifier] = hasReturn;
	
	// 将该函数名添加到符号表
	initCurrentSymbol();
	curSym.kind = 4;
	curSym.type = 0;
	curSym.name = lastIdentifier;
	insertCurrentSymbol();

	// 更新符号表，增加新的层次
	curLevel = lastIdentifier;
	insertCurrentLevel();

	if (symbol != LPARENT) {
		error('0');
	}
	getsym();
	getParameterList();
	if (symbol != RPARENT) {
		error('l');
	}
	else {
		getsym();
	}
	if (symbol != LBRACE) {
		error('0');
	}
	getsym();
	getCompoundStatements();
	if (symbol != RBRACE) {
		error('0');
	}

	// 有返回值函数缺少返回语句
	if (functionWithReturnValue.count(curLevel) > 0 && returnedType == 0) {
		error('h');
	}

	curLevel = PROGRAM;		// 返回外层函数

	getsym();
	//ofile << "<无返回值函数定义>" << endl;
}

void getFunctionDefinationWithoutReturnValueForProgram() {
	returnedType = 0;	// 初始化已经返回的参数类型
	getIdentifier();
	functionWithoutReturnValue.insert(lastIdentifier);
	functionReturnType[lastIdentifier] = hasReturn;

	// 将该函数名添加到符号表
	initCurrentSymbol();
	curSym.kind = 4;
	curSym.type = 0;
	curSym.name = lastIdentifier;
	insertCurrentSymbol();

	// 更新符号表，增加新的层次
	curLevel = lastIdentifier;
	insertCurrentLevel();

	if (symbol != LPARENT) {
		error('0');
	}
	getsym();
	getParameterList();
	if (symbol != RPARENT) {
		error('l');
	}
	else {
		getsym();
	}
	if (symbol != LBRACE) {
		error('0');
	}
	getsym();
	getCompoundStatements();
	if (symbol != RBRACE) {
		error('0');
	}

	// 有返回值函数缺少返回语句
	if (functionWithReturnValue.count(curLevel) > 0 && returnedType == 0) {
		error('h');
	}


	curLevel = PROGRAM;		// 返回外层函数

	getsym();
	//ofile << "<无返回值函数定义>" << endl;
}

void getCompoundStatements() {
	if (symbol == CONSTTK) {
		getConstantDeclaration();
	}
	if (symbol == INTTK || symbol == CHARTK) {
		getVariableDeclaration();
	}
	getStatementList();
	//ofile << "<复合语句>" << endl;
}

void getParameterList() {
	parameterCount = 0;		// 初始化函数参数计数器
	if (symbol == INTTK || symbol == CHARTK) {
		getTypeIdentifier();
		getIdentifier();
		
		// 将参数添加到函数专属符号表中，仅包含参数数量和类型

		if (lastTypeIdentifier == INTTK) {
			functionParameterType[curLevel][parameterCount] = 1;
		}
		else if (lastTypeIdentifier == CHARTK) {
			functionParameterType[curLevel][parameterCount] = 2;
		}
		parameterCount++;

		// 将参数添加到符号表中，仅有kind和type，没有value
		initCurrentSymbol();
		curSym.kind = 5;
		if (lastTypeIdentifier == INTTK) {
			curSym.type = 1;
		}
		else {
			curSym.type = 4;
		}
		curSym.name = lastIdentifier;
		insertCurrentSymbol();
		
		while (symbol == COMMA) {
			getsym();
			getTypeIdentifier();
			getIdentifier();

			// 将参数添加到函数专属符号表中，仅包含参数数量和类型
			if (lastTypeIdentifier == INTTK) {
				functionParameterType[curLevel][parameterCount] = 1;
			}
			else if (lastTypeIdentifier == CHARTK) {
				functionParameterType[curLevel][parameterCount] = 2;
			}
			parameterCount++;

			// 将参数添加到符号表中，仅有kind和type，没有value
			initCurrentSymbol();
			curSym.kind = 5;
			if (lastTypeIdentifier == INTTK) {
				curSym.type = 1;
			}
			else {
				curSym.type = 4;
			}
			curSym.name = lastIdentifier;
			insertCurrentSymbol();
		}
	}

	functionParameterCount[curLevel] = parameterCount;

	//ofile << "<参数表>" << endl;
}

void getMain() {
	// 将main添加到无返回值函数
	functionWithoutReturnValue.insert(MAIN);
	functionReturnType[MAIN] = 0;

	// 将main添加到符号表的当前层次
	initCurrentSymbol();
	curSym.kind = 6;
	curSym.type = 0;
	curSym.name = MAIN;
	insertCurrentSymbol();

	// 更新符号表的层次
	curLevel = MAIN;
	insertCurrentLevel();

	if (symbol != VOIDTK) {
		error('0');
	}
	getsym();
	if (symbol != MAINTK) {
		error('0');
	}
	getsym();
	if (symbol != LPARENT) {
		error('0');
	}
	getsym();
	if (symbol != RPARENT) {
		error('l');
	}
	else {
		getsym();
	}
	if (symbol != LBRACE) {
		error('0');
	}
	getsym();
	getCompoundStatements();
	if (symbol != RBRACE) {
		error('0');
	}

	curLevel = PROGRAM;		// 返回外层函数

	getsym();
	//ofile << "<主函数>" << endl;
}

void getMainForProgram() {
	// 将main添加到无返回值函数
	functionWithoutReturnValue.insert(MAIN);
	functionReturnType[MAIN] = 0;

	// 将main添加到符号表的当前层次
	initCurrentSymbol();
	curSym.kind = 6;
	curSym.type = 0;
	curSym.name = MAIN;
	insertCurrentSymbol();

	// 更新符号表的层次
	curLevel = MAIN;
	insertCurrentLevel();

	if (symbol != MAINTK) {
		error('0');
	}
	getsym();
	if (symbol != LPARENT) {
		error('0');
	}
	getsym();
	if (symbol != RPARENT) {
		error('l');
	}
	else {
		getsym();
	}
	if (symbol != LBRACE) {
		error('0');
	}
	getsym();
	getCompoundStatements();
	if (symbol != RBRACE) {
		error('0');
	}
	getsym();
	//ofile << "<主函数>" << endl;
}

void getExpression() {
	lastExpressionType = 2;
	if (symbol == PLUS || symbol == MINU) {
		getAdditionOperator();
		lastExpressionType = 1;
	}
	getTerm();
	while (symbol == PLUS || symbol == MINU) {
		getAdditionOperator();
		lastExpressionType = 1;
		getTerm();
	}
	//ofile << "<表达式>" << endl;
}

void getTerm() {
	getFactor();
	while (symbol == MULT || symbol == DIV) {
		getMultiplicationOperator();
		lastExpressionType = 1;
		getFactor();
	}
	//ofile << "<项>" << endl;
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

			// 检查是否有未定义的名字
			if (symList[curLevel].find(lastIdentifier) == symList[curLevel].end()
				&& symList[PROGRAM].find(lastIdentifier) == symList[PROGRAM].end()) {
				error('c');
			}
			else if (symList[curLevel].find(lastIdentifier) != symList[curLevel].end()) {
				if (symList[curLevel][lastIdentifier].type >= 1 && symList[curLevel][lastIdentifier].type <= 3) {
					lastExpressionType = 1;
				}
			}
			else if (symList[PROGRAM].find(lastIdentifier) != symList[PROGRAM].end()) {
				if (symList[PROGRAM][lastIdentifier].type >= 1 && symList[PROGRAM][lastIdentifier].type <= 3) {
					lastExpressionType = 1;
				}
			}

			int tmpLastExpressionType = lastExpressionType;		// 记录表达式类型临时变量

			if (symbol == LBRACK) {
				getsym();
				getExpression();

				// 数组下标不可为字符型
				if (lastExpressionType == 2) {
					error('i');
				}

				if (symbol != RBRACK) {
					error('m');
				}
				else {
					getsym();
				}
			}
			if (symbol == LBRACK) {
				getsym();
				getExpression();

				// 数组下标不可为字符型
				if (lastExpressionType == 2) {
					error('i');
				}

				if (symbol != RBRACK) {
					error('m');
				}
				else {
					getsym();
				}
			}
			lastExpressionType = tmpLastExpressionType;
		}
	}
	else if (symbol == LPARENT) {
		getsym();
		getExpression();
		lastExpressionType = 1;
		if (symbol != RPARENT) {
			error('l');
		}
		else {
			getsym();
		}
	}
	else if (symbol == INTCON || symbol == PLUS || symbol == MINU) {
		getInteger();
		lastExpressionType = 1;
	}
	else if (symbol == CHARCON) {
		getChar();
	}
	else {
		error('0');
	}
	//ofile << "<因子>" << endl;
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

		// 检查是否有未定义的名字
		if (symList[curLevel].find(tmpIdenfr) == symList[curLevel].end()
			&& symList[PROGRAM].find(tmpIdenfr) == symList[PROGRAM].end()) {
			error('c');
		}
		else {
			if (functionWithReturnValue.count(tmpIdenfr) > 0) {
				getFunctionCallStatementWithReturnValue();
			}
			else if (functionWithoutReturnValue.count(tmpIdenfr) > 0) {
				getFunctionCallStatementWithoutReturnValue();
			}
			else {
				// 判断是否给常量进行了赋值操作
				if ((symList[curLevel].find(tmpIdenfr) != symList[curLevel].end() && symList[curLevel][tmpIdenfr].kind == 0)
					|| (symList[PROGRAM].find(tmpIdenfr) != symList[PROGRAM].end() && symList[PROGRAM][tmpIdenfr].kind == 0)) {
					error('j');
				}
				getAssignmentStatement();
			}
		}
		
		if (symbol != SEMICN) {
			error('k');
		}
		else {
			getsym();
		}

	}
	else if (symbol == SCANFTK) {
		getScanf();
		if (symbol != SEMICN) {
			error('k');
		}
		else {
			getsym();
		}
	}
	else if (symbol == PRINTFTK) {
		getPrintf();
		if (symbol != SEMICN) {
			error('k');
		}
		else {
			getsym();
		}
	}
	else if (symbol == SWITCHTK) {
		getSwitch();
	}
	else if (symbol == SEMICN) {
		if (symbol != SEMICN) {
			error('k');
		}
		else {
			getsym();
		}
	}
	else if (symbol == RETURNTK) {
		getReturn();
		if (symbol != SEMICN) {
			error('k');
		}
		else {
			getsym();
		}
	}
	else if (symbol == LBRACE) {
		getsym();
		getStatementList();
		if (symbol != RBRACE) {
			error('0');
		}
		getsym();
	}
	//ofile << "<语句>" << endl;
}

void getAssignmentStatement() {
	getIdentifier();

	if (symbol == LBRACK){
		getsym();
		getExpression();

		// 数组下标不可为字符型
		if (lastExpressionType == 2) {
			error('i');
		}

		if (symbol != RBRACK) {
			error('m');
		}
		else {
			getsym();
		}
	}

	if (symbol == LBRACK) {
		getsym();
		getExpression();

		// 数组下标不可为字符型
		if (lastExpressionType == 2) {
			error('i');
		}

		if (symbol != RBRACK) {
			error('m');
		}
		else {
			getsym();
		}
	}

	if (symbol != ASSIGN) {
		error('0');
	}
	getsym();
	getExpression();
	//ofile << "<赋值语句>" << endl;
}

void getConditionalStatement() {
	if (symbol != IFTK) {
		error('0');
	}
	getsym();
	if (symbol != LPARENT) {
		error('0');
	}
	getsym();
	getCondition();
	if (symbol != RPARENT) {
		error('l');
	}
	else {
		getsym();
	}
	getStatement();
	while (symbol == ELSETK) {
		getsym();
		getStatement();
	}
	//ofile << "<条件语句>" << endl;
}

void getCondition() {
	getExpression();
	if (lastExpressionType == 2) {
		error('f');
	}

	getRelationalOperator();
	
	getExpression();
	if (lastExpressionType == 2) {
		error('f');
	}

	//ofile << "<条件>" << endl;
}

void getLoopStatement() {
	if (symbol == WHILETK) {
		getsym();
		if (symbol != LPARENT) {
			error('0');
		}
		getsym();
		getCondition();
		if (symbol != RPARENT) {
			error('l');
		}
		else {
			getsym();
		}
		getStatement();
	}
	else if (symbol == FORTK) {
		getsym();
		if (symbol != LPARENT) {
			error('0');
		}
		getsym();
		getIdentifier();

		// 检查是否有未定义的名字
		if (symList[curLevel].find(lastIdentifier) == symList[curLevel].end()
			&& symList[PROGRAM].find(lastIdentifier) == symList[PROGRAM].end()) {
			error('c');
		}

		if (symbol != ASSIGN) {
			error('0');
		}
		getsym();
		getExpression();

		if (symbol != SEMICN) {
			error('k');
		}
		else {
			getsym();
		}
		
		getCondition();
		
		if (symbol != SEMICN) {
			error('k');
		}
		else {
			getsym();
		}
		
		getIdentifier();

		// 检查是否有未定义的名字
		if (symList[curLevel].find(lastIdentifier) == symList[curLevel].end()
			&& symList[PROGRAM].find(lastIdentifier) == symList[PROGRAM].end()) {
			error('c');
		}

		if (symbol != ASSIGN) {
			error('0');
		}
		getsym();
		getIdentifier();

		// 检查是否有未定义的名字
		if (symList[curLevel].find(lastIdentifier) == symList[curLevel].end()
			&& symList[PROGRAM].find(lastIdentifier) == symList[PROGRAM].end()) {
			error('c');
		}

		getAdditionOperator();
		getStep();
		if (symbol != RPARENT) {
			error('l');
		}
		else {
			getsym();
		}
		getStatement();
	}
	//ofile << "<循环语句>" << endl;
}

void getStep() {
	getUnsignedInteger();
	//ofile << "<步长>" << endl;
}

void getSwitch() {
	if (symbol != SWITCHTK) {
		error('0');
	}
	getsym();
	if (symbol != LPARENT) {
		error('0');
	}
	getsym();
	getExpression();

	lastConstantType = lastExpressionType - 1;		// 更新switch语句中的常量的类型

	if (symbol != RPARENT) {
		error('l');
	}
	else {
		getsym();
	}
	if (symbol != LBRACE) {
		error('0');
	}
	getsym();
	getCaseTable();

	// 错误处理，如果没有缺省语句
	if (symbol != DEFAULTTK) {
		error('p');
	}
	else {
		getDefault();
	}

	if (symbol != RBRACE) {
		error('0');
	}
	getsym();
	//ofile << "<情况语句>" << endl;
}

void getCaseTable() {
	while (symbol == CASETK) {
		getCase();
	}
	//ofile << "<情况表>" << endl;
}

void getCase() {
	if (symbol != CASETK) {
		error('0');
	}
	getsym();
	getConstant();

	// switch语句中常量类型不一致
	if (constType != lastConstantType) {
		error('o');
	}

	if (symbol != COLON) {
		error('0');
	}
	getsym();
	getStatement();
	//ofile << "<情况子语句>" << endl;
}

void getDefault() {
	if (symbol != DEFAULTTK) {
		error('0');
	}
	getsym();
	if (symbol != COLON) {
		error('0');
	}
	getsym();
	getStatement();
	//ofile << "<缺省>" << endl;
}

void getFunctionCallStatementWithReturnValue() {
	getIdentifier();

	// 检查是否有未定义的名字
	if (symList[curLevel].find(lastIdentifier) == symList[curLevel].end()
		&& symList[PROGRAM].find(lastIdentifier) == symList[PROGRAM].end()) {
		error('c');
	}
	else {
		// 如果该标识符为函数，且返回值为整型
		if (functionReturnType.count(lastIdentifier) > 0 && functionReturnType[lastIdentifier] == 1) {
			lastExpressionType = 1;
		}
	}

	// 更新函数调用的名称
	lastFunctionCall = lastIdentifier;

	if (symbol != LPARENT) {
		error('0');
	}
	getsym();

	int tmpLastExpressionType = lastExpressionType;
	getValueParameterTable();
	lastExpressionType = tmpLastExpressionType;
	
	if (symbol != RPARENT) {
		error('l');
	}
	else {
		getsym();
	}
	//ofile << "<有返回值函数调用语句>" << endl;
}

void getFunctionCallStatementWithoutReturnValue() {
	getIdentifier();

	// 检查是否有未定义的名字
	if (symList[curLevel].find(lastIdentifier) == symList[curLevel].end()
		&& symList[PROGRAM].find(lastIdentifier) == symList[PROGRAM].end()) {
		error('c');
	}

	// 更新函数调用的名称
	lastFunctionCall = lastIdentifier;

	if (symbol != LPARENT) {
		error('0');
	}
	getsym();

	int tmpLastExpressionType = lastExpressionType;
	getValueParameterTable();
	lastExpressionType = tmpLastExpressionType;
	
	if (symbol != RPARENT) {
		error('l');
	}
	else {
		getsym();
	}
	//ofile << "<无返回值函数调用语句>" << endl;
}

void getValueParameterTable() {
	int tmpParameterCount = 0;
	string tmpFunctionCall = lastFunctionCall;

	// 如果直接遇上分号，进行错误处理
	if (symbol == RPARENT || symbol == SEMICN) {
		//ofile << "<值参数表>" << endl;		// 值参数表为空，直接返回
	}
	else {
		getExpression();
		// 判断值参数表和参数表中对应位置的参数类型是否相同
		if (tmpParameterCount < functionParameterCount[tmpFunctionCall] &&
			lastExpressionType != functionParameterType[tmpFunctionCall][tmpParameterCount]) {
			error('e');
		}
		tmpParameterCount++;

		while (symbol == COMMA) {
			getsym();
			getExpression();
			// 判断值参数表和参数表中对应位置的参数类型是否相同
			if (tmpParameterCount < functionParameterCount[tmpFunctionCall] &&
				lastExpressionType != functionParameterType[tmpFunctionCall][tmpParameterCount]) {
				error('e');
			}
			tmpParameterCount++;
		}
		//ofile << "<值参数表>" << endl;
	}

	// 判断值参数表和参数表中对应位置的参数数量是否相同
	if (tmpParameterCount != functionParameterCount[tmpFunctionCall]) {
		error('d');
	}
}

void getStatementList() {
	while (symbol != RBRACE) {
		getStatement();
	}
	//ofile << "<语句列>" << endl;
}

void getScanf() {
	if (symbol != SCANFTK) {
		error('0');
	}
	getsym();
	if (symbol != LPARENT) {
		error('0');
	}
	getsym();
	getIdentifier();

	// 检查该标识符是否定义以及是否为常量
	if (symList[curLevel].count(lastIdentifier) == 1) {
		if (symList[curLevel][lastIdentifier].kind == 0) {
			error('j');
		}
	}
	else if (symList[PROGRAM].count(lastIdentifier) == 1) {
		if (symList[PROGRAM][lastIdentifier].kind == 0) {
			error('j');
		}
	}
	else {
		error('c');
	}

	if (symbol != RPARENT) {
		error('l');
	}
	else {
		getsym();
	}
	//ofile << "<读语句>" << endl;
}

void getPrintf() {
	if (symbol != PRINTFTK) {
		error('0');
	}
	getsym();
	if (symbol != LPARENT) {
		error('0');
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
		error('l');
	}
	else {
		getsym();
	}
	//ofile << "<写语句>" << endl;
}

void getReturn() {
	if (symbol != RETURNTK) {
		error('0');
	}
	getsym();
	if (symbol == LPARENT) {
		getsym();
		getExpression();
		returnedType = lastExpressionType;	// 获取函数实际返回值类型
		if (symbol != RPARENT) {
			error('l');
		}
		else {
			getsym();
		}

		// 无返回值函数产生了返回值
		if (functionWithoutReturnValue.count(curLevel) > 0) {
			error('g');
		}
	}
	else {
		returnedType = 3;
	}

	// 有返回值函数返回类型不同
	if (functionWithReturnValue.count(curLevel) > 0 && functionReturnType[curLevel] != returnedType) {
		error('h');
	}


	//ofile << "<返回语句>" << endl;
}