#include "Parser.h"

void getAdditionOperator() {
	if (!(symbol == PLUS || symbol == MINU)) {
		error('0');
	}
	getsym();
}

void getAdditionOperator(int& additionOperator) {
	if (!(symbol == PLUS || symbol == MINU)) {
		error('0');
	}
	additionOperator = symbol;
	getsym();
}

void getMultiplicationOperator() {
	if (!(symbol == MULT || symbol == DIV)) {
		error('0');
	}
	getsym();
}

void getMultiplicationOperator(int& multiplicationOperator) {
	if (!(symbol == MULT || symbol == DIV)) {
		error('0');
	}
	multiplicationOperator = symbol;
	getsym();
}

void getRelationalOperator() {
	if (!(symbol == LSS || symbol == LEQ || symbol == GRE || symbol == GEQ || symbol == EQL || symbol == NEQ)) {
		error('0');
	}
	getsym();
}

void getChar() {
	if (symbol != CHARCON) {
		error('0');
	}
	lastChar = TOKEN;
	getsym();
}

void getChar(string& c) {
	if (symbol != CHARCON) {
		error('0');
	}
	lastChar = TOKEN;
	c = TOKEN;
	getsym();
}

void getString() {
	if (symbol != STRCON) {
		error('0');
	}
	getsym();
	ofile << "<字符串>" << endl;
}

void getString(string& s) {
	if (symbol != STRCON) {
		error('0');
	}
	s = TOKEN;
	getsym();
	ofile << "<字符串>" << endl;
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

	ofile << "<程序>" << endl;
}

// Polished!
void getConstantDeclaration() {
	while (symbol == CONSTTK) {
		getsym();
		getConstantDefination();	// 在该子程序中添加常量到符号表
		checkSymbolandGetSym(SEMICN, 'k');
	}
	ofile << "<常量说明>" << endl;
}

// Polished!
//＜常量定义＞ ::= int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
//             | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
void getConstantDefination() {
	if (symbol == INTTK) {
		initCurrentSymbol();	// 准备添加当前常量到符号表
		curSym.kind = 0;
		curSym.type = 1;		// 符号表相关，一维int
		
		do {
			getsym();
			string tmpIdentifier;
			getIdentifier(tmpIdentifier);
			curSym.name = tmpIdentifier;		// 更新当前符号的名字，即标识符的名字

			checkSymbolandGetSym(ASSIGN, '0');

			int tmpInteger;
			getInteger(tmpInteger);
			curSym.num = tmpInteger;	// 将获取到的整数值插入符号表
			insertCurrentSymbol();

			insertMidCode(curSym.name, "int", CONST, int2string(tmpInteger));	// 添加中间表达式，不产生mips代码
		} while (symbol == COMMA);
	}
	else if (symbol == CHARTK) {
		initCurrentSymbol();	// 准备添加当前常量到符号表
		curSym.kind = 0;
		curSym.type = 4;		// 符号表相关，一维char
		
		do {
			getsym();
			string tmpIdentifier;
			getIdentifier(tmpIdentifier);
			curSym.name = tmpIdentifier;		// 更新当前符号的名字，即标识符的名字

			checkSymbolandGetSym(ASSIGN, '0');

			string tmpChar;
			getChar(tmpChar);
			curSym.str = tmpChar;			// 更新当前符号的字符值
			insertCurrentSymbol();

			insertMidCode(curSym.name, "char", CONST, tmpChar);		// 添加中间表达式，不产生mips代码
		} while (symbol == COMMA);
	}

	ofile << "<常量定义>" << endl;
}

void getUnsignedInteger() {
	if (symbol != INTCON) {
		error('0');
	}
	lastInteger = lastOperator * atoi(TOKEN);		// 记录上一个识别的整数
	curSym.num = lastInteger;
	getsym();
	ofile << "<无符号整数>" << endl;
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
	ofile << "<整数>" << endl;
}

void getUnsignedInteger(int& unsignedInteger) {
	checkSymbol(INTCON, '0');
	lastInteger = lastOperator * atoi(TOKEN);		// 记录上一个识别的整数
	curSym.num = lastInteger;
	unsignedInteger = atoi(TOKEN);
	getsym();
	ofile << "<无符号整数>" << endl;
}

void getInteger(int& integer) {
	bool isPLUS = true;
	lastOperator = 1;
	if (symbol == PLUS) {
		lastOperator = 1;
		isPLUS = true;
		getsym();
	}
	if (symbol == MINU) {
		lastOperator = -1;
		isPLUS = false;
		getsym();
	}
	
	int unsignedInteger;
	getUnsignedInteger(unsignedInteger);
	integer = isPLUS ? unsignedInteger : -1 * unsignedInteger;
	ofile << "<整数>" << endl;
}

void getIdentifier() {
	if (symbol != IDENFR) {
		error('0');
	}
	lastIdentifier = TOKEN;
	getsym();
}

void getIdentifier(string& identifier) {
	if (symbol != IDENFR) {
		error('0');
	}
	identifier = TOKEN;
	lastIdentifier = TOKEN;
	getsym();
}

void getDeclarationHeader() {
	if (symbol != INTTK && symbol != CHARTK) {
		error('0');
	}
	getIdentifier();
	ofile << "<声明头部>" << endl;
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
	ofile << "<常量>" << endl;
}

void getConstant(int& tmpConstType, int& tInteger, string& tChar) {
	// 整数或者字符
	if (symbol == INTCON || symbol == PLUS || symbol == MINU) {
		tmpConstType = 0;
		constType = 0;
		int tmpInteger;
		getInteger(tmpInteger);
		tInteger = tmpInteger;
	}
	else if (symbol == CHARCON) {
		tmpConstType = 1;
		constType = 1;
		string tmpChar;
		getChar(tmpChar);
		tChar = tmpChar;
	}
	else {
		error('0');
	}
	ofile << "<常量>" << endl;
}

void getVariableDeclaration() {
	while (symbol == INTTK || symbol == CHARTK) {
		getVariableDefination();
		checkSymbolandGetSym(SEMICN, 'k');
	}
	ofile << "<变量说明>" << endl;
}

void getVariableDeclarationForProgram() {
	getVariableDefinationForProgram();
	checkSymbolandGetSym(SEMICN, 'k');
}

void getVariableDefination() {
	dimension = 0;	// 初始化数组维数

	// 变量定义的共同部分
	int tmpTypeIdentifier;
	getTypeIdentifier(tmpTypeIdentifier);
	string tmpIdentifier;
	getIdentifier(tmpIdentifier);

	// 初始化当前符号
	initCurrentSymbol();
	curSym.name = tmpIdentifier;
	curSym.type = tmpTypeIdentifier == INTTK ? 1 : 4;

	lastConstantType = tmpTypeIdentifier == INTTK ? 0 : 1;
	lastVariableType = tmpTypeIdentifier == INTTK ? 1 : 4;

	// 接下来两个判断语句用来判断数组维度
	if (symbol == LBRACK) {
		dimension++;
		getsym();

		int tmpUnsignedInteger;
		getUnsignedInteger(tmpUnsignedInteger);
		curSym.index = tmpUnsignedInteger;		// 记录当前变量定义的行数

		checkSymbolandGetSym(RBRACK, 'm');
	}
	if (symbol == LBRACK) {
		dimension++;
		getsym();

		int tmpUnsignedInteger;
		getUnsignedInteger(tmpUnsignedInteger);
		curSym.columns = tmpUnsignedInteger;	// 记录当前变量定义的列数

		checkSymbolandGetSym(RBRACK, 'm');
	}

	curSym.type = curSym.type + dimension;		// 更新当前符号保存的数组维度

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

	ofile << "<变量定义>" << endl;
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
		dimension++;
		getsym();

		int tmpUnsignedInteger;
		getUnsignedInteger(tmpUnsignedInteger);
		curSym.index = tmpUnsignedInteger;		// 记录当前变量定义的行数

		checkSymbolandGetSym(RBRACK, 'm');
	}
	if (symbol == LBRACK) {
		dimension++;
		getsym();

		int tmpUnsignedInteger;
		getUnsignedInteger(tmpUnsignedInteger);
		curSym.columns = tmpUnsignedInteger;	// 记录当前变量定义的列数

		checkSymbolandGetSym(RBRACK, 'm');
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

	ofile << "<变量定义>" << endl;
}

// Polished!
void getVariableDefinationWithoutInitialization() {
	while (symbol == COMMA) {
		int dimension = 0;		// 初始化维度
		getsym();

		string tmpIdentifier;
		getIdentifier(tmpIdentifier);
		
		initCurrentSymbol();	// 初始化当前符号
		curSym.kind = 2;		// 更新当前符号类型，无初始化的变量定义
		curSym.type = lastVariableType;
		curSym.name = tmpIdentifier;
		
		if (symbol == LBRACK) {
			dimension++;
			getsym();

			int tmpUnsignedInteger;
			getUnsignedInteger(tmpUnsignedInteger);
			curSym.index = tmpUnsignedInteger;		// 记录当前变量定义的行数

			checkSymbolandGetSym(RBRACK, 'm');
		}
		if (symbol == LBRACK) {
			dimension++;
			getsym();

			int tmpUnsignedInteger;
			getUnsignedInteger(tmpUnsignedInteger);
			curSym.columns = tmpUnsignedInteger;	// 记录当前变量定义的列数

			checkSymbolandGetSym(RBRACK, 'm');
		}

		// 更新当前符号保存的数组维度
		curSym.type = curSym.type + dimension;
		insertCurrentSymbol();
	}

	ofile << "<变量定义无初始化>" << endl;
}

// Polished!
void getVariableDefinationWithInitialization() {
	// 初始化临时变量
	indexCount = 0;
	columnsCount = 0;
	
	getsym();

	if (dimension == 0) {
		int tmpConstantType;
		int tmpInteger;
		string tmpChar;
		getConstant(tmpConstantType, tmpInteger, tmpChar);
		if (tmpConstantType != lastConstantType) {
			error('o');
		}
		if (tmpConstantType == 0) {
			curSym.num = tmpInteger;
		}
		else if (tmpConstantType == 1) {
			curSym.str = tmpChar;
		}
	} else if (dimension ==1) {
		checkSymbolandGetSym(LBRACE, '0');			// 匹配 '['
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

		checkSymbolandGetSym(RBRACE, '0');			// 匹配 ']'

		if (indexCount != curSym.index) {
			error('n');
		}
	} else if (dimension == 2) {
		checkSymbolandGetSym(LBRACE, '0');			// 匹配 '['
		checkSymbolandGetSym(LBRACE, '0');			// 匹配 '['
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

		checkSymbolandGetSym(RBRACE, '0');			// 匹配 ']'
		indexCount++;

		// 判断数组的列数是否相同
		if (columnsCount != curSym.columns) {
			error('n');
		}

		while (symbol == COMMA) {
			columnsCount = 0;		// 初始化列计数器
			getsym();
			checkSymbolandGetSym(LBRACE, '0');			// 匹配 '['
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

			checkSymbolandGetSym(RBRACE, '0');			// 匹配 ']'
			indexCount++;		// 行变量叠加

			// 判断数组的列数是否相同
			if (columnsCount != curSym.columns) {
				error('n');
			}
		}

		checkSymbolandGetSym(RBRACE, '0');			// 匹配 ']'

		// 判断数组的行数是否相同
		if (indexCount != curSym.index) {
			error('n');
		}
	}	

	ofile << "<变量定义及初始化>" << endl;
}

void getTypeIdentifier() {
	if (symbol == INTTK || symbol == CHARTK)
		lastTypeIdentifier = symbol;
	else 
		error('0');
	getsym();
}

void getTypeIdentifier(int& typeIdentifier) {
	if (symbol == INTTK || symbol == CHARTK) {
		lastTypeIdentifier = symbol;
		typeIdentifier = symbol;
	}
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
	ofile << "<有返回值函数定义>" << endl;
}

void getFunctionDefinationWithReturnValueForProgram() {
	returnedType = 0;	// 初始化已经返回的参数类型

	getIdentifier();
	ofile << "<声明头部>" << endl;		// 类型标识符在getProgram中识别
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
	ofile << "<有返回值函数定义>" << endl;
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
	ofile << "<无返回值函数定义>" << endl;
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

	ofile << "<参数表>" << endl;
}

// POLISHED!
// ＜主函数＞ ::= void main '(' ')' '{' ＜复合语句＞ '}'
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

	insertMidCode("void", MAIN, FUNC, "");

	checkSymbolandGetSym(MAINTK, '0');		// main
	checkSymbolandGetSym(LPARENT, '0');		// '('
	checkSymbolandGetSym(RPARENT, 'l');		// ')'
	checkSymbolandGetSym(LBRACE, '0');		// '{'
	getCompoundStatements();				// <复合语句>
	checkSymbolandGetSym(RBRACE, '0');		// '}'

	ofile << "<主函数>" << endl;
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
	ofile << "<表达式>" << endl;
}

void getTerm() {
	getFactor();
	while (symbol == MULT || symbol == DIV) {
		getMultiplicationOperator();
		lastExpressionType = 1;
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
	ofile << "<因子>" << endl;
}

// Polished!
// ＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}  
void getExpression(int& type, string& ansTmp) {
	string result, left, right;
	bool isPLUS = true;
	
	// 记录 ［＋｜－］
	if (symbol == PLUS || symbol == MINU) {
		isPLUS = (symbol == PLUS);
		getAdditionOperator();
		type = 1;
	}
	
	// 处理 <项>
	getTerm(type, left);
	
	// 处理第一项之前的符号
	if (!isPLUS) {
		result = genTmp();				// 获取中间临时变量
		insertSymbol(result, 1, 0);		// 将中间临时变量添加到符号表
		insertMidCode(result, int2string(0), MINUOP, left);
		left = result;
	}

	// 处理 {＜加法运算符＞＜项＞} 
	while (symbol == PLUS || symbol == MINU) {
		isPLUS = (symbol == PLUS);
		getAdditionOperator();
		type = 1;
		getTerm(type, right);
		result = genTmp();				// 获取中间临时变量
		insertSymbol(result, 1, 0);		// 将中间临时变量添加到符号表
		insertMidCode(result, left, isPLUS ? PLUSOP : MINUOP, right);
		left = result;
	}

	ansTmp = left;
	
	ofile << "<表达式>" << endl;
}

// Polished!
// ＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void getTerm(int& type, string& ansTmp) {
	string result, left, right;

	// 处理 <因子>
	getFactor(type, left);

	// 处理 {＜乘法运算符＞＜因子＞}
	while (symbol == MULT || symbol == DIV) {
		int tmpMultiplicationOperator;
		getMultiplicationOperator(tmpMultiplicationOperator);
		
		type = 1;
		getFactor(type, right);
		result = genTmp();				// 获取中间临时变量
		insertSymbol(result, 1, 0);		// 将中间临时变量添加到符号表
		insertMidCode(result, left, tmpMultiplicationOperator == MULT ? MULTOP : DIVOP, right);
		left = result;
	}

	// 更新返回符号
	ansTmp = left;

	ofile << "<项>" << endl;
}

// Polished!
//＜因子＞ ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']' | ＜标识符＞'['＜表达式＞']''['＜表达式＞']' 
//          | '('＜表达式＞')'｜＜整数＞ | ＜字符＞｜＜有返回值函数调用语句＞
void getFactor(int& type, string& ansTmp) {
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

			string tmpIdentifier = lastIdentifier;

			// 检查是否有未定义的名字
			if (symList[curLevel].find(lastIdentifier) == symList[curLevel].end()
				&& symList[PROGRAM].find(lastIdentifier) == symList[PROGRAM].end()) {
				error('c');
			}
			else if (symList[curLevel].find(lastIdentifier) != symList[curLevel].end()) {
				if (symList[curLevel][lastIdentifier].type >= 1 && symList[curLevel][lastIdentifier].type <= 3) {
					lastExpressionType = 1;
					type = 1;
				}
			}
			else if (symList[PROGRAM].find(lastIdentifier) != symList[PROGRAM].end()) {
				if (symList[PROGRAM][lastIdentifier].type >= 1 && symList[PROGRAM][lastIdentifier].type <= 3) {
					lastExpressionType = 1;
					type = 1;
				}
			}

			int tmpLastExpressionType = lastExpressionType;		// 记录表达式类型临时变量

			int dimension = 0;
			if (symbol == LBRACK) {
				dimension++;
				getsym();
				getExpression();

				// 数组下标不可为字符型
				if (lastExpressionType == 2) {
					error('i');
				}
				checkSymbolandGetSym(RBRACK, 'm');		// 匹配 ']'
			}
			if (symbol == LBRACK) {
				dimension++;
				getsym();
				getExpression();

				// 数组下标不可为字符型
				if (lastExpressionType == 2) {
					error('i');
				}
				checkSymbolandGetSym(RBRACK, 'm');		// 匹配 ']'
			}
			if (dimension == 0) {
				ansTmp = tmpIdentifier;
			}
			lastExpressionType = tmpLastExpressionType;
		}
	}
	else if (symbol == LPARENT) {
		getsym();
		getExpression(type, ansTmp);
		type = 1;
		if (symbol != RPARENT) 
			error('l');
		else 
			getsym();
	}
	else if (symbol == INTCON || symbol == PLUS || symbol == MINU) {
		type = 1;
		int value;
		getInteger(value);
		ansTmp = int2string(value);
	}
	else if (symbol == CHARCON) {
		string c;
		getChar(c);
		ansTmp = c;
	}
	else {
		error('0');
	}
	ofile << "<因子>" << endl;
}

// POLISHED!
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
		
		checkSymbolandGetSym(SEMICN, 'k');
	}
	else if (symbol == SCANFTK) {
		getScanf();
		checkSymbolandGetSym(SEMICN, 'k');
	}
	else if (symbol == PRINTFTK) {
		getPrintf();
		checkSymbolandGetSym(SEMICN, 'k');
	}
	else if (symbol == SWITCHTK) {
		getSwitch();
	}
	else if (symbol == SEMICN) {
		checkSymbolandGetSym(SEMICN, 'k');
	}
	else if (symbol == RETURNTK) {
		getReturn();
		checkSymbolandGetSym(SEMICN, 'k');
	}
	else if (symbol == LBRACE) {
		getsym();
		getStatementList();
		checkSymbolandGetSym(RBRACE, '0');
	}
	ofile << "<语句>" << endl;
}

// Polished!
void getAssignmentStatement() {
	string tmpIdentifier;
	getIdentifier(tmpIdentifier);

	if (symbol == LBRACK){
		getsym();
		int tmpExpressionType;
		string tmpExpression;
		getExpression(tmpExpressionType, tmpExpression);

		// 数组下标不可为字符型
		if (tmpExpressionType == 2) {
			error('i');
		}

		checkSymbolandGetSym(RBRACK, 'm');	// 匹配']'
	}
	if (symbol == LBRACK) {
		getsym();
		int tmpExpressionType;
		string tmpExpression;
		getExpression(tmpExpressionType, tmpExpression);

		// 数组下标不可为字符型
		if (tmpExpressionType == 2) {
			error('i');
		}

		checkSymbolandGetSym(RBRACK, 'm');	// 匹配']'
	}

	checkSymbolandGetSym(ASSIGN, '0');		// 匹配 '='

	int tmpExpressionType;
	string tmpExpression;
	getExpression(tmpExpressionType, tmpExpression);

	insertMidCode(tmpIdentifier, tmpExpression, ASSIGNOP, "");		// 将赋值语句添加到中间代码
	
	ofile << "<赋值语句>" << endl;
}

// polished
// ＜条件语句＞ :== if '('＜条件＞')'＜语句＞［else＜语句＞］
void getConditionalStatement() {
	checkSymbolandGetSym(IFTK, '0');		// if
	checkSymbolandGetSym(LPARENT, '0');		// '('
	getCondition();							// <条件>
	checkSymbolandGetSym(RPARENT, 'l');		// ')'
	getStatement();							// <语句>
	if (symbol == ELSETK) {
		getsym();
		getStatement();
	}
	ofile << "<条件语句>" << endl;
}

//polish
// ＜条件＞ ::= ＜表达式＞＜关系运算符＞＜表达式＞  
void getCondition() {
	int tmpExpressionType = 2;
	string tmpExpression;
	getExpression(tmpExpressionType, tmpExpression);		// <表达式>
	if (tmpExpressionType == 2) {
		error('f');
	}

	getRelationalOperator();								// <关系运算符>
	
	tmpExpressionType = 2;
	getExpression(tmpExpressionType, tmpExpression);		// <表达式>
	if (tmpExpressionType == 2) {
		error('f');
	}

	ofile << "<条件>" << endl;
}

// polish
//＜循环语句＞   :: = while '('＜条件＞')'＜语句＞ | for'('＜标识符＞＝＜表达式＞; ＜条件＞; ＜标识符＞＝＜标识符＞(+| -)＜步长＞')'＜语句＞
void getLoopStatement() {
	if (symbol == WHILETK) {
		getsym();
		checkSymbolandGetSym(LPARENT, '0');		// '('
		getCondition();							// <条件>
		checkSymbolandGetSym(RPARENT, 'l');		// ')'
		getStatement();							// <语句>
	}
	else if (symbol == FORTK) {
		getsym();
		checkSymbolandGetSym(LPARENT, '0');		// '('
		
		string tmpIdentifier;
		getIdentifier(tmpIdentifier);			// <标识符>
		if (symList[curLevel].find(tmpIdentifier) == symList[curLevel].end()
			&& symList[PROGRAM].find(tmpIdentifier) == symList[PROGRAM].end()) {
			error('c');		// 检查是否有未定义的名字
		}

		checkSymbolandGetSym(ASSIGN, '0');		// '='
		getExpression();						// <表达式>
		checkSymbolandGetSym(SEMICN, 'k');		// ';'
		getCondition();							// <条件>
		checkSymbolandGetSym(SEMICN, 'k');		// ';'
		
		getIdentifier(tmpIdentifier);			// <标识符>
		if (symList[curLevel].find(tmpIdentifier) == symList[curLevel].end()
			&& symList[PROGRAM].find(tmpIdentifier) == symList[PROGRAM].end()) {
			error('c');
		}
		
		checkSymbolandGetSym(ASSIGN, '0');		// '='
		
		getIdentifier(tmpIdentifier);			// <标识符>
		if (symList[curLevel].find(lastIdentifier) == symList[curLevel].end()
			&& symList[PROGRAM].find(lastIdentifier) == symList[PROGRAM].end()) {
			error('c');
		}

		getAdditionOperator();					// (+|-)
		getStep();								// <步长>
		checkSymbolandGetSym(RPARENT, 'l');		// '('
		getStatement();							// <语句>
	}
	ofile << "<循环语句>" << endl;
}

void getStep() {
	getUnsignedInteger();
	ofile << "<步长>" << endl;
}

//polish
// ＜情况语句＞ ::= switch '('＜表达式＞')' '{'＜情况表＞＜缺省＞'}'
void getSwitch() {
	checkSymbolandGetSym(SWITCHTK, '0');				// switch
	checkSymbolandGetSym(LPARENT, '0');					// '('

	int tmpExpressionType;
	string tmpExpression;
	getExpression(tmpExpressionType, tmpExpression);	// <表达式>
	lastConstantType = tmpExpressionType - 1;			// 更新switch语句中的常量的类型

	checkSymbolandGetSym(RPARENT, 'l');					// ')'
	checkSymbolandGetSym(LBRACE, '0');					// '{'
	getCaseTable();										// <情况表>

	if (symbol != DEFAULTTK) 
		error('p');										// 缺少<缺省>
	else 
		getDefault();									// <缺省>

	checkSymbolandGetSym(LBRACE, '0');					// '}'
	ofile << "<情况语句>" << endl;
}

//p
// ＜情况表＞ ::= ＜情况子语句＞{ ＜情况子语句＞ }
void getCaseTable() {
	while (symbol == CASETK) {
		getCase();
	}
	ofile << "<情况表>" << endl;
}

//p
// ＜情况子语句＞ ::= case＜常量＞：＜语句＞
void getCase() {
	checkSymbolandGetSym(CASETK, '0');			// case

	getConstant();

	// switch语句中常量类型不一致
	if (constType != lastConstantType) {
		error('o');
	}

	checkSymbolandGetSym(COLON, '0');			// ':'
	getStatement();								// <语句>
	ofile << "<情况子语句>" << endl;
}

//p
// ＜缺省＞ ::= default :＜语句＞
void getDefault() {
	checkSymbolandGetSym(DEFAULTTK, '0');		// default
	checkSymbolandGetSym(COLON, '0');			// ':'
	getStatement();								// <语句>
	ofile << "<缺省>" << endl;
}

// ＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void getFunctionCallStatementWithReturnValue() {
	string tmpIdentifier;
	getIdentifier(tmpIdentifier);			// 标识符
	if (symList[curLevel].find(tmpIdentifier) == symList[curLevel].end()
		&& symList[PROGRAM].find(tmpIdentifier) == symList[PROGRAM].end()) {
		error('c');
	}
	else {
		// 如果该标识符为函数，且返回值为整型
		if (functionReturnType.count(lastIdentifier) > 0 && functionReturnType[lastIdentifier] == 1) {
			lastExpressionType = 1;
		}
	}

	// 更新函数调用的名称
	lastFunctionCall = tmpIdentifier;

	checkSymbolandGetSym(LPARENT, '0');		// '('

	int tmpLastExpressionType = lastExpressionType;
	getValueParameterTable();				// <值参数表>
	lastExpressionType = tmpLastExpressionType;
	
	checkSymbolandGetSym(RPARENT, 'l');		// ')'
	ofile << "<有返回值函数调用语句>" << endl;
}

// ＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void getFunctionCallStatementWithoutReturnValue() {
	string tmpIdentifier;
	getIdentifier(tmpIdentifier);			// <标识符>
	if (symList[curLevel].find(tmpIdentifier) == symList[curLevel].end()
		&& symList[PROGRAM].find(tmpIdentifier) == symList[PROGRAM].end()) {
		error('c');
	}
	lastFunctionCall = lastIdentifier;		// 更新函数调用的名称

	checkSymbolandGetSym(LPARENT, '0');		// '('

	int tmpLastExpressionType = lastExpressionType;
	getValueParameterTable();				// <值参数表>
	lastExpressionType = tmpLastExpressionType;
	
	checkSymbolandGetSym(RPARENT, 'l');		// ')'
	ofile << "<无返回值函数调用语句>" << endl;
}

void getValueParameterTable() {
	int tmpParameterCount = 0;
	string tmpFunctionCall = lastFunctionCall;

	// 如果直接遇上分号，进行错误处理
	if (symbol == RPARENT || symbol == SEMICN) {
		ofile << "<值参数表>" << endl;		// 值参数表为空，直接返回
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
		ofile << "<值参数表>" << endl;
	}

	// 判断值参数表和参数表中对应位置的参数数量是否相同
	if (tmpParameterCount != functionParameterCount[tmpFunctionCall]) {
		error('d');
	}
}

//p
// ＜语句列＞ ::= ｛＜语句＞}
void getStatementList() {
	while (symbol != RBRACE)
		getStatement();
	ofile << "<语句列>" << endl;
}

// p
// ＜读语句＞ ::= scanf '('＜标识符＞')' 
void getScanf() {
	checkSymbolandGetSym(SCANFTK, '0');		// scanf
	checkSymbolandGetSym(LPARENT, '0');		// '('

	string tmpIdentifier;
	getIdentifier(tmpIdentifier);

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

	insertMidCode(tmpIdentifier, "", SCAN, "");		// 将该语句添加到中间代码

	checkSymbolandGetSym(RPARENT, 'l');		// ')'
	ofile << "<读语句>" << endl;
}

// ＜写语句＞ ::= printf'('＜字符串＞,＜表达式＞')' | printf '('＜字符串＞')' | printf '('＜表达式＞')’
void getPrintf() {
	checkSymbolandGetSym(PRINTFTK, '0');	// printf
	checkSymbolandGetSym(LPARENT, '0');		// '('

	if (symbol == STRCON) {
		string tmpString;
		getString(tmpString);
		int p = -2;
		while ((p = tmpString.find("\\", p + 2)) != string::npos) {
			tmpString.replace(p, 1, "\\\\");
		}
		stringList.push_back(tmpString);
		
		if (symbol == COMMA) {
			// printf'('＜字符串＞,＜表达式＞')'
			insertMidCode(tmpString, "3", PRINT, "");
			getsym();

			int type = 2;
			string tmpExpression;
			getExpression(type, tmpExpression);
			
			insertMidCode(tmpExpression, int2string(type), PRINT, "");
			insertMidCode("EndLine", "4", PRINT, "");
		}
		else {
			// printf'('＜字符串＞')'
			insertMidCode(tmpString, "3", PRINT, "");
			insertMidCode("EndLine", "4", PRINT, "");
		}
	}
	else {
		// printf '('＜表达式＞')'
		int tmpType = 2;
		string tmpExpression;
		getExpression(tmpType, tmpExpression);

		insertMidCode(tmpExpression, int2string(tmpType), PRINT, "");
		insertMidCode("EndLine", "4", PRINT, "");
	}

	checkSymbolandGetSym(RPARENT, 'l');	// 匹配 ')'

	ofile << "<写语句>" << endl;
}

// ＜返回语句＞ ::= return['('＜表达式＞')']   
void getReturn() {
	checkSymbolandGetSym(RETURNTK, '0');
	if (symbol == LPARENT) {
		checkSymbolandGetSym(LPARENT, '0');		// '('

		int tmpExpressionType;
		string tmpExpression;
		getExpression(tmpExpressionType, tmpExpression);
		returnedType = tmpExpressionType;	// 获取函数实际返回值类型
		
		checkSymbolandGetSym(RPARENT, 'l');		// ')'

		if (functionWithoutReturnValue.count(curLevel) > 0) {
			error('g');						// 无返回值函数产生了返回值
		}
	}
	else {
		returnedType = 3;
	}
	
	if (functionWithReturnValue.count(curLevel) > 0 && functionReturnType[curLevel] != returnedType) {
		error('h');							// 有返回值函数返回类型不同
	}

	ofile << "<返回语句>" << endl;
}

void checkSymbol(int symType, char errorType) {
	if (symbol != symType) 
		error(errorType);
}

void checkSymbolandGetSym(int symType, char errorType) {
	if (symbol != symType) 
		error(errorType);
	else 
		getsym();
}