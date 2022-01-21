#include "Parser.h"



// ＜加法运算符＞ ::= +｜-  
void getAdditionOperator(int& additionOperator) {
	if (!(symbol == PLUS || symbol == MINU)) {
		error('0');
	}
	additionOperator = symbol;
	getsym();
}

// ＜乘法运算符＞ ::= *｜/
void getMultiplicationOperator(int& multiplicationOperator) {
	if (!(symbol == MULT || symbol == DIV)) {
		error('0');
	}
	multiplicationOperator = symbol;
	getsym();
}

// ＜关系运算符＞ ::= <｜<=｜>｜>=｜!=｜==
void getRelationalOperator(int& tmpRelationalOperator) {
	if (!(symbol == LSS || symbol == LEQ || symbol == GRE || symbol == GEQ || symbol == EQL || symbol == NEQ)) {
		error('0');
	}
	tmpRelationalOperator = symbol;
	getsym();
}

void getChar(string& c) {
	checkSymbol(CHARCON, '0');
	c = TOKEN;
	getsym();
}

void getString(string& s) {
	checkSymbol(STRCON, '0');
	s = TOKEN;
	getsym();
	ofile << "<字符串>" << endl;
}

// ＜程序＞ ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void getProgram() {
	int hasVariable = 0;	// 判断是否有变量说明
	
	curLevel = PROGRAM;		// 更新符号表的层次，此处为最外层
	insertCurrentLevel();

	// 常量说明
	if (symbol == CONSTTK) {
		getConstantDeclaration();
	}

	// 变量说明
	while (symbol == INTTK || symbol == CHARTK) {
		int tmpTypeIdentifier = symbol;
		trysym();
		trysym();
		if (symbol != LPARENT) {
			hasVariable++;
			ungetsym();
			getVariableDeclarationForProgram(tmpTypeIdentifier);
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

	insertMidCode(JMAIN, "", "", "");

	// 有返回值函数定义和无返回值函数定义
	while (symbol == INTTK || symbol == CHARTK || symbol == VOIDTK) {
		int returnType = 0;			// 保存函数返回值类型
		returnType = (symbol == INTTK) ? 1 : (symbol == CHARTK) ? 2 : 0;
		
		getsym();
		if (symbol == MAINTK) {
			break;
		}

		if (returnType > 0) {
			getFunctionDefinationWithReturnValueForProgram(returnType);
		}
		else if (returnType == 0) {
			getFunctionDefinationWithoutReturnValueForProgram(returnType);
		}
	}

	getMainForProgram();			// 主函数
	checkBeforeFunc();
	ofile << "<程序>" << endl;
}

// ＜常量说明＞ ::= const＜常量定义＞;{const＜常量定义＞;} 
void getConstantDeclaration() {
	while (symbol == CONSTTK) {
		getsym();
		getConstantDefination();	// 在该子程序中添加常量到符号表
		checkSymbolandGetSym(SEMICN, 'k');
	}
	ofile << "<常量说明>" << endl;
}

//＜常量定义＞ ::= int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞} | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
void getConstantDefination() {
	int tmpInteger;
	string tmpIdentifier, tmpChar;

	if (symbol == INTTK) {
		initCurrentSymbol();	// 准备添加当前常量到符号表
		curSym.kind = 0;		// 常量
		curSym.type = 1;		// int
		do {
			getsym();
			getIdentifier(tmpIdentifier);			// <标识符>
			curSym.name = tmpIdentifier;			
			checkSymbolandGetSym(ASSIGN, '0');		// '='
			getInteger(tmpInteger);					// <整数>
			curSym.num = tmpInteger;				
			insertCurrentSymbol();					

			insertMidCode(CONST, curSym.name, "int", int2string(tmpInteger));	// 添加中间表达式，不产生mips代码
		} while (symbol == COMMA);
	}
	else if (symbol == CHARTK) {
		initCurrentSymbol();	// 准备添加当前常量到符号表
		curSym.kind = 0;		// 常量
		curSym.type = 4;		// char
		do {
			getsym();		
			getIdentifier(tmpIdentifier);			// <标识符>
			curSym.name = tmpIdentifier;
			checkSymbolandGetSym(ASSIGN, '0');		// '='
			getChar(tmpChar);						// <字符>
			curSym.str = tmpChar;
			insertCurrentSymbol();

			insertMidCode(CONST, curSym.name, "char", tmpChar);		// 添加中间表达式，不产生mips代码
		} while (symbol == COMMA);
	}

	ofile << "<常量定义>" << endl;
}

void getUnsignedInteger(int& tmpUnsignedInteger) {
	checkSymbol(INTCON, '0');
	tmpUnsignedInteger = atoi(TOKEN);
	getsym();
	ofile << "<无符号整数>" << endl;
}

// ＜整数＞ ::= ［＋｜－］＜无符号整数＞
void getInteger(int& tmpInteger) {
	int isPLUS = 1, tmpUnsignedInteger;
	if (symbol == PLUS) {
		getsym();
	}
	if (symbol == MINU) {
		isPLUS = -1;
		getsym();
	}
	getUnsignedInteger(tmpUnsignedInteger);
	tmpInteger = isPLUS * tmpUnsignedInteger;
	ofile << "<整数>" << endl;
}

void getIdentifier(string& tmpIdentifier) {
	checkSymbol(IDENFR, '0');
	tmpIdentifier = TOKEN;
	transform(tmpIdentifier.begin(), tmpIdentifier.end(), tmpIdentifier.begin(), ::tolower);
	getsym();
}

// ＜常量＞ ::= ＜整数＞ | ＜字符＞
void getConstant(int& tmpConstType, int& constInteger, string& constChar) {
	if (symbol == INTCON || symbol == PLUS || symbol == MINU) {
		tmpConstType = 0;
		getInteger(constInteger);
	}
	else if (symbol == CHARCON) {
		tmpConstType = 1;
		string tmpChar;
		getChar(tmpChar);
		constChar = int2string((int)tmpChar[0]);
	}
	else {
		error('0');
	}
	ofile << "<常量>" << endl;
}

// ＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}
void getVariableDeclaration() {
	while (symbol == INTTK || symbol == CHARTK) {
		getVariableDefination();
		checkSymbolandGetSym(SEMICN, 'k');
	}
	ofile << "<变量说明>" << endl;
}

void getVariableDeclarationForProgram(int tmpTypeIdentifier) {
	getVariableDefinationForProgram(tmpTypeIdentifier);
	checkSymbolandGetSym(SEMICN, 'k');
}

// ＜变量定义＞ ::= ＜变量定义无初始化＞|＜变量定义及初始化＞
void getVariableDefination() {
	int dimension = 0, tmpTypeIdentifier, tmpUnsignedInteger;
	string tmpIdentifier;

	getTypeIdentifier(tmpTypeIdentifier);
	getIdentifier(tmpIdentifier);

	initCurrentSymbol();				// 初始化当前符号
	curSym.name = tmpIdentifier;
	curSym.type = tmpTypeIdentifier == INTTK ? 1 : 4;

	// 接下来两个判断语句用来判断数组维度
	if (symbol == LBRACK) {
		dimension++;
		getsym();
		getUnsignedInteger(tmpUnsignedInteger);
		curSym.index = tmpUnsignedInteger;			// 记录当前变量定义的数组的行数
		checkSymbolandGetSym(RBRACK, 'm');
	}
	if (symbol == LBRACK) {
		dimension++;
		getsym();
		getUnsignedInteger(tmpUnsignedInteger);
		curSym.columns = tmpUnsignedInteger;		// 记录当前变量定义的数组的列数
		checkSymbolandGetSym(RBRACK, 'm');
	}

	curSym.type = curSym.type + dimension;		// 更新当前符号保存的数组维度

	// 判断是否进行初始化
	if (symbol == ASSIGN) {
		curSym.kind = 1;		// 更新当前符号类型，有初始化的变量定义
		getsym();
		getVariableDefinationWithInitialization(tmpTypeIdentifier, dimension);
		insertCurrentSymbol();	// 变量定义及初始化每行仅有一个
	}
	else {
		curSym.kind = 2;		// 更新当前符号类型，无初始化的变量定义
		insertCurrentSymbol();	// 由于变量定义无初始化每行能有多个，所以先将当前的加入符号表
		getVariableDefinationWithoutInitialization(tmpTypeIdentifier);
	}

	insertMidCode(VAR, tmpTypeIdentifier == INTTK ? "int" : "char", tmpIdentifier, "");

	ofile << "<变量定义>" << endl;
}

void getVariableDefinationForProgram(int tmpTypeIdentifier) {
	int dimension = 0, tmpUnsignedInteger;
	string tmpIdentifier;
	
	getIdentifier(tmpIdentifier);

	initCurrentSymbol();
	curSym.name = tmpIdentifier;
	curSym.type = tmpTypeIdentifier == INTTK ? 1 : 4;

	if (symbol == LBRACK) {
		dimension++;
		getsym();
		getUnsignedInteger(tmpUnsignedInteger);			// <无符号整数>
		curSym.index = tmpUnsignedInteger;				// 记录当前变量定义的行数
		checkSymbolandGetSym(RBRACK, 'm');				// ']'
	}
	if (symbol == LBRACK) {
		dimension++;
		getsym();
		getUnsignedInteger(tmpUnsignedInteger);			// <无符号整数>
		curSym.columns = tmpUnsignedInteger;			// 记录当前变量定义的列数
		checkSymbolandGetSym(RBRACK, 'm');				// ']'
	}

	curSym.type = curSym.type + dimension;		// 更新当前符号保存的数组维度

	// 判断是否进行初始化
	if (symbol == ASSIGN) {
		curSym.kind = 1;		// 更新当前符号类型，有初始化的变量定义
		getsym();
		getVariableDefinationWithInitialization(tmpTypeIdentifier, dimension);
		insertCurrentSymbol();
	}
	else {
		curSym.kind = 2;		// 更新当前符号类型，无初始化的变量定义
		insertCurrentSymbol();
		getVariableDefinationWithoutInitialization(tmpTypeIdentifier);
	}

	insertMidCode(VAR, tmpTypeIdentifier == INTTK ? "int" : "char", tmpIdentifier, "");

	ofile << "<变量定义>" << endl;
}

void getVariableDefinationWithoutInitialization(int tmpTypeIdentifier) {
	while (symbol == COMMA) {
		string tmpIdentifier;
		int dimension = 0, tmpUnsignedInteger;
		
		getsym();
		getIdentifier(tmpIdentifier);
		
		initCurrentSymbol();			// 初始化当前符号
		curSym.name = tmpIdentifier;
		curSym.kind = 2;				// 无初始化的变量定义
		curSym.type = tmpTypeIdentifier == INTTK ? 1 : 4;
		
		if (symbol == LBRACK) {
			dimension++;
			getsym();
			getUnsignedInteger(tmpUnsignedInteger);
			curSym.index = tmpUnsignedInteger;		// 记录当前变量定义的行数
			checkSymbolandGetSym(RBRACK, 'm');
		}
		if (symbol == LBRACK) {
			dimension++;
			getsym();
			getUnsignedInteger(tmpUnsignedInteger);
			curSym.columns = tmpUnsignedInteger;	// 记录当前变量定义的列数
			checkSymbolandGetSym(RBRACK, 'm');
		}

		curSym.type = curSym.type + dimension;		// 更新当前符号保存的数组维度
		insertCurrentSymbol();
	}

	ofile << "<变量定义无初始化>" << endl;
}

void getVariableDefinationWithInitialization(int tmpTypeIdentifier, int dimension) {
	string tmpChar;
	int lastConstantType = tmpTypeIdentifier == INTTK ? 0 : 1;
	int tmpConstantType, tmpInteger, indexCount = 0, columnsCount = 0;

	if (dimension == 0) {
		getConstant(tmpConstantType, tmpInteger, tmpChar);
		if (tmpConstantType != lastConstantType) {
			error('o');
		}
		if (tmpConstantType == 0) {
			curSym.num = tmpInteger;
			insertMidCode(ASSIGNOP, curSym.name, int2string(tmpInteger), "");	// 含有初始值的变量定义
		}
		else if (tmpConstantType == 1) {
			curSym.str = tmpChar;
			insertMidCode(ASSIGNOP, curSym.name, tmpChar, "");	// 含有初始值的变量定义
		}
	} else if (dimension == 1) {
		checkSymbolandGetSym(LBRACE, '0');						// '{'
		getConstant(tmpConstantType, tmpInteger, tmpChar);		// <常量>
		if (tmpConstantType != lastConstantType) {
			error('o');
		}
		insertMidCode(PUTARRAY, curSym.name, int2string(indexCount), tmpConstantType == 0 ? int2string(tmpInteger) : tmpChar);		// 添加一维数组元素
		indexCount++;

		while (symbol == COMMA) {
			getsym();
			getConstant(tmpConstantType, tmpInteger, tmpChar);
			if (tmpConstantType != lastConstantType) {
				error('o');
			}
			insertMidCode(PUTARRAY, curSym.name, int2string(indexCount), tmpConstantType == 0 ? int2string(tmpInteger) : tmpChar);		// 添加一维数组元素
			indexCount++;
		}

		checkSymbolandGetSym(RBRACE, '0');			// 匹配 '}'

		if (indexCount != curSym.index) {
			error('n');
		}
	} else if (dimension == 2) {
		checkSymbolandGetSym(LBRACE, '0');			// 匹配 '{'
		checkSymbolandGetSym(LBRACE, '0');			// 匹配 '{'
		getConstant(tmpConstantType, tmpInteger, tmpChar);
		if (tmpConstantType != lastConstantType) {
			error('o');
		}
		insertMidCode(PUTARRAY2, curSym.name, int2string(indexCount), int2string(columnsCount), tmpConstantType == 0 ? int2string(tmpInteger) : tmpChar);	// 添加二维数组元素
		columnsCount++;
		while (symbol == COMMA) {
			getsym();
			getConstant(tmpConstantType, tmpInteger, tmpChar);
			if (tmpConstantType != lastConstantType) {
				error('o');
			}
			insertMidCode(PUTARRAY2, curSym.name, int2string(indexCount), int2string(columnsCount), tmpConstantType == 0 ? int2string(tmpInteger) : tmpChar);	// 添加二维数组元素
			columnsCount++;
		}

		checkSymbolandGetSym(RBRACE, '0');			// 匹配 '}'
		indexCount++;
	
		if (columnsCount != curSym.columns) {
			error('n');			// 判断数组的列数是否相同
		}

		while (symbol == COMMA) {
			columnsCount = 0;		// 初始化列计数器
			getsym();
			checkSymbolandGetSym(LBRACE, '0');			// 匹配 '{'
			getConstant(tmpConstantType, tmpInteger, tmpChar);
			if (tmpConstantType != lastConstantType) {
				error('o');
			}
			insertMidCode(PUTARRAY2, curSym.name, int2string(indexCount), int2string(columnsCount), tmpConstantType == 0 ? int2string(tmpInteger) : tmpChar);	// 添加二维数组元素
			columnsCount++;
			while (symbol == COMMA) {
				getsym();
				getConstant(tmpConstantType, tmpInteger, tmpChar);
				if (tmpConstantType != lastConstantType) {
					error('o');
				}
				insertMidCode(PUTARRAY2, curSym.name, int2string(indexCount), int2string(columnsCount), tmpConstantType == 0 ? int2string(tmpInteger) : tmpChar);	// 添加二维数组元素
				columnsCount++;
			}

			checkSymbolandGetSym(RBRACE, '0');			// 匹配 '}'
			indexCount++;								// 行变量叠加

			if (columnsCount != curSym.columns) {
				error('n');								// 判断数组的列数是否相同
			}
		}

		checkSymbolandGetSym(RBRACE, '0');			// 匹配 '}'

		
		if (indexCount != curSym.index) {
			error('n');								// 判断数组的行数是否相同
		}
	}	

	ofile << "<变量定义及初始化>" << endl;
}

// ＜类型标识符＞ ::= int | char
void getTypeIdentifier(int& typeIdentifier) {
	if (symbol == INTTK || symbol == CHARTK) {
		typeIdentifier = symbol;
	}
	else
		error('0');
	getsym();
}

// ＜有返回值函数定义＞ ::= ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}' 
void getFunctionDefinationWithReturnValueForProgram(int returnType) {
	string tmpIdentifier;
	returnedType = 0;	// 全局变量，初始化已经返回的参数类型

	getIdentifier(tmpIdentifier);
	ofile << "<声明头部>" << endl;		// 类型标识符在getProgram中识别
	functionReturnType[tmpIdentifier] = returnType;
	funcNameList.push_back(tmpIdentifier);
	checkBeforeFunc();

	initCurrentSymbol();
	insertSymbol(tmpIdentifier, 3, 0);		// 更新符号表
	curLevel = tmpIdentifier;
	insertCurrentLevel();					// 更新符号表，增加新的层次

	insertMidCode(FUNC, returnType == 1 ? "int" : "char", tmpIdentifier, "");

	checkSymbolandGetSym(LPARENT, '0');		// '('
	getParameterList();						// <参数表>
	checkSymbolandGetSym(RPARENT, 'l');		// ')'
	checkSymbolandGetSym(LBRACE, '0');		// '{'
	getCompoundStatements();				// <复合语句>
	checkSymbolandGetSym(RBRACE, '0');		// '}'

	if (functionReturnType.count(curLevel) > 0 && functionReturnType[curLevel] > 0 && returnedType == 0) {
		error('h');		// 有返回值函数缺少返回语句
	}
	
	symList[PROGRAM][curLevel].length = localAddr;		// 更新函数参数数量

	curLevel = PROGRAM;		// 返回外层函数

	ofile << "<有返回值函数定义>" << endl;
}

// ＜无返回值函数定义＞ ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void getFunctionDefinationWithoutReturnValueForProgram(int returnType) {
	string tmpIdentifier;
	returnedType = 0;	// 初始化已经返回的参数类型
	
	getIdentifier(tmpIdentifier);
	functionReturnType[tmpIdentifier] = returnType;
	funcNameList.push_back(tmpIdentifier);
	checkBeforeFunc();

	initCurrentSymbol();
	insertSymbol(tmpIdentifier, 4, 0);		// 将该函数名添加到符号表
	curLevel = tmpIdentifier;
	insertCurrentLevel();					// 更新符号表，增加新的层次

	insertMidCode(FUNC, "void", tmpIdentifier, "");

	checkSymbolandGetSym(LPARENT, '0');		// '('
	getParameterList();						// <参数表>
	checkSymbolandGetSym(RPARENT, 'l');		// ')'
	checkSymbolandGetSym(LBRACE, '0');		// '{'
	getCompoundStatements();				// <复合语句>
	checkSymbolandGetSym(RBRACE, '0');		// '}'

	/*
	// 有返回值函数缺少返回语句
	if (functionReturnType.count(curLevel) > 0 && functionReturnType[curLevel] > 0 && returnedType == 0) {
		error('h');
	}
	*/

	symList[PROGRAM][curLevel].length = localAddr;		// 更新函数参数数量

	curLevel = PROGRAM;		// 返回外层函数

	ofile << "<无返回值函数定义>" << endl;
}

// ＜复合语句＞ ::= [＜常量说明＞］［＜变量说明＞］＜语句列＞
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

// ＜参数表＞ ::= ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞} | ＜空＞
void getParameterList() {
	int parameterCount = 0, tmpTypeIdentifier;
	string tmpIdentifier;

	if (symbol == INTTK || symbol == CHARTK) {
		getTypeIdentifier(tmpTypeIdentifier);
		getIdentifier(tmpIdentifier);
		
		insertMidCode(PARAM, tmpTypeIdentifier == INTTK ? "int" : "char", tmpIdentifier, "");

		// 将参数添加到函数专属符号表中，仅包含参数数量和类型
		functionParameterType[curLevel][parameterCount] = tmpTypeIdentifier == INTTK ? 1 : 2;
		parameterCount++;

		initCurrentSymbol();
		curSym.name = tmpIdentifier;
		curSym.kind = 5;
		curSym.type = tmpTypeIdentifier == INTTK ? 1 : 4;
		insertCurrentSymbol();			// 将参数添加到符号表中
		
		while (symbol == COMMA) {
			getsym();
			getTypeIdentifier(tmpTypeIdentifier);
			getIdentifier(tmpIdentifier);

			insertMidCode(PARAM, tmpTypeIdentifier == INTTK ? "int" : "char", tmpIdentifier, "");

			functionParameterType[curLevel][parameterCount] = tmpTypeIdentifier == INTTK ? 1 : 2;
			parameterCount++;

			initCurrentSymbol();
			curSym.name = tmpIdentifier;
			curSym.kind = 5;
			curSym.type = tmpTypeIdentifier == INTTK ? 1 : 4;
			insertCurrentSymbol();
		}
	}

	functionParameterCount[curLevel] = parameterCount;

	ofile << "<参数表>" << endl;
}

// ＜主函数＞ ::= void main '(' ')' '{' ＜复合语句＞ '}'
void getMainForProgram() {
	// 将main添加到无返回值函数
	functionReturnType[MAIN] = 0;
	funcNameList.push_back(MAIN);
	checkBeforeFunc();

	// 将main添加到符号表的当前层次
	initCurrentSymbol();
	curSym.kind = 6;
	curSym.type = 0;
	curSym.name = MAIN;
	insertCurrentSymbol();

	// 更新符号表的层次
	curLevel = MAIN;
	insertCurrentLevel();
	checkBeforeFunc();
	insertMidCode(FUNC, "void", MAIN, "");

	checkSymbolandGetSym(MAINTK, '0');		// main
	checkSymbolandGetSym(LPARENT, '0');		// '('
	checkSymbolandGetSym(RPARENT, 'l');		// ')'
	checkSymbolandGetSym(LBRACE, '0');		// '{'
	getCompoundStatements();				// <复合语句>
	checkSymbolandGetSym(RBRACE, '0');		// '}'

	symList[PROGRAM][curLevel].length = localAddr;		// 更新函数参数数量

	ofile << "<主函数>" << endl;
}

// ＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}  
void getExpression(int& type, string& ansTmp) {
	string result, left, right;
	bool isPLUS = true;
	int tmpAdditionOperator;
	
	if (symbol == PLUS || symbol == MINU) {
		isPLUS = (symbol == PLUS);
		getAdditionOperator(tmpAdditionOperator);		// ［＋｜－］
		type = 1;
	}
	
	getTerm(type, left);								// <项>
	
	// 处理第一项之前的符号
	if (!isPLUS) {
		if (isdigit(left[0]) || left[0] == '-' || left[0] == '+') {
			// 若第一项为常数
			int v1 = string2int(left);
			left = int2string(-v1);
		}
		else {
			result = genTmp();				// 获取中间临时变量
			insertSymbol(result, 1, 0);		// 将中间临时变量添加到符号表
			insertMidCode(MINUOP, result, int2string(0), left);
			left = result;
		}	
	}

	// 处理 {＜加法运算符＞＜项＞} 
	while (symbol == PLUS || symbol == MINU) {
		type = 1;
		isPLUS = (symbol == PLUS);
		getAdditionOperator(tmpAdditionOperator);	// <加法运算符>
		getTerm(type, right);						// <项>

		if ((isdigit(left[0]) || left[0] == '-' || left[0] == '+') && (isdigit(right[0]) || right[0] == '-' || right[0] == '+')) {
			// 若两个运算数均为常数
			int v1 = string2int(left);
			int v2 = string2int(right);
			result = isPLUS ? int2string(v1 + v2) : int2string(v1 - v2);
		}
		else {
			result = genTmp();							// 获取中间临时变量
			insertSymbol(result, 1, 0);					// 将中间临时变量添加到符号表
			insertMidCode(isPLUS ? PLUSOP : MINUOP, result, left, right);
		}
		left = result;
	}

	ansTmp = left;
	
	ofile << "<表达式>" << endl;
}

// ＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void getTerm(int& type, string& ansTmp) {
	string result, left, right;
	int tmpMultiplicationOperator;

	getFactor(type, left);		// <因子>

	// 处理 {＜乘法运算符＞＜因子＞}
	while (symbol == MULT || symbol == DIV) {
		type = 1;
		getMultiplicationOperator(tmpMultiplicationOperator);		// <乘法运算符>
		getFactor(type, right);										// <因子>

		if ((isdigit(left[0]) || left[0] == '-' || left[0] == '+') && (isdigit(right[0]) || right[0] == '-' || right[0] == '+')) {
			// 若两个运算数均为常数
			int v1 = string2int(left);
			int v2 = string2int(right);
			result = tmpMultiplicationOperator == MULT ? int2string(v1 * v2) : int2string(v1 / v2);
		}
		else {
			result = genTmp();											// 获取中间临时变量
			insertSymbol(result, 1, 0);									// 将中间临时变量添加到符号表
			insertMidCode(tmpMultiplicationOperator == MULT ? MULTOP : DIVOP, result, left, right);
		}
		left = result;
	}

	ansTmp = left;

	ofile << "<项>" << endl;
}

// ＜因子＞ ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']' | ＜标识符＞'['＜表达式＞']''['＜表达式＞']' | '('＜表达式＞')'｜＜整数＞ | ＜字符＞｜＜有返回值函数调用语句＞
void getFactor(int& type, string& ansTmp) {
	if (symbol == IDENFR) {
		// 可能为 变量 或者 <有返回值函数调用语句>
		trysym();

		if (symbol == LPARENT) {
			ungetsym();
			getFunctionCallStatementWithReturnValue(type);

			string result = genTmp();
			insertSymbol(result, 1, 0);

			// 如果当前中间代码最后一个是INLINEEND 说明此函数内联了
			if (midCodes[midCodes.size() - 1].op == INLINEEND) {
				if (midCodes[midCodes.size() - 2].op == INLINERET) {
					string value = midCodes[midCodes.size() - 2].result;
					midCodes[midCodes.size() - 2].op = ASSIGNOP;
					midCodes[midCodes.size() - 2].result = result;
					midCodes[midCodes.size() - 2].left = value;
				}
			}
			else {
				insertMidCode(RETVALUE, result, "RET", "");
			}

			ansTmp = result;
		}
		else {
			ungetsym();
			int tmpExpressionType;
			string tmpIdentifier, indexExpression, columnExpression;
			getIdentifier(tmpIdentifier);

			if (symList[curLevel].find(tmpIdentifier) != symList[curLevel].end()) {
				if (symList[curLevel][tmpIdentifier].type >= 1 && symList[curLevel][tmpIdentifier].type <= 3) {
					type = 1;
				}
			}
			else if (symList[PROGRAM].find(tmpIdentifier) != symList[PROGRAM].end()) {
				if (symList[PROGRAM][tmpIdentifier].type >= 1 && symList[PROGRAM][tmpIdentifier].type <= 3) {
					type = 1;
				}
			}
			else {
				error('c');		// 检查是否有未定义的名字
			}

			int dimension = 0;
			if (symbol == LBRACK) {
				dimension++;
				getsym();
				tmpExpressionType = 2;
				getExpression(tmpExpressionType, indexExpression);
				if (tmpExpressionType == 2) {
					error('i');		// 数组下标不可为字符型
				}
				checkSymbolandGetSym(RBRACK, 'm');		// 匹配 ']'
			}
			if (symbol == LBRACK) {
				dimension++;
				getsym();
				tmpExpressionType = 2;
				getExpression(tmpExpressionType, columnExpression);
				if (tmpExpressionType == 2) {
					error('i');		// 数组下标不可为字符型
				}
				checkSymbolandGetSym(RBRACK, 'm');		// 匹配 ']'
			}

			if (dimension == 0) {
				// 非数组标识符


				bool isConstValue = false;
				int value;
				if (symList[curLevel].find(tmpIdentifier) != symList[curLevel].end()) {
					if (symList[curLevel][tmpIdentifier].kind == 0) {
						isConstValue = true;
						if (symList[curLevel][tmpIdentifier].type == 1) {
							value = symList[curLevel][tmpIdentifier].num;
						}
						else {
							value = symList[curLevel][tmpIdentifier].str[0];
						}
					}
				}
				else if (symList[PROGRAM].find(tmpIdentifier) != symList[PROGRAM].end() && symList[PROGRAM][tmpIdentifier].kind != 3) {
					if (symList[PROGRAM][tmpIdentifier].kind == 0) {
						isConstValue = true;
						if (symList[PROGRAM][tmpIdentifier].type == 1) {
							value = symList[PROGRAM][tmpIdentifier].num;
						}
						else {
							value = symList[PROGRAM][tmpIdentifier].str[0];
						}
					}
				}
					
				if (isConstValue) {  //常量的话直接返回值
					ansTmp = int2string(value);
				}
				else {
					ansTmp = tmpIdentifier;  //直接返回token
				}
			}
			else if (dimension == 1) {
				// 一维数组
				string result = genTmp();
				insertSymbol(result, 1, 0);
				insertMidCode(GETARRAY, result, tmpIdentifier, indexExpression);
				ansTmp = result;
			}
			else if (dimension == 2) {
				// 二维数组
				string result = genTmp();
				insertSymbol(result, 1, 0);
				insertMidCode(GETARRAY2, result, tmpIdentifier, indexExpression, columnExpression);
				ansTmp = result;
			}
		}
	}
	else if (symbol == LPARENT) {
		getsym();
		getExpression(type, ansTmp);
		type = 1;
		checkSymbolandGetSym(RPARENT, 'l');		// ')'
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
		ansTmp = int2string((int)c[0]);
	}
	else {
		error('0');
	}
	ofile << "<因子>" << endl;
}

// ＜语句＞ ::= ＜循环语句＞｜＜条件语句＞ | ＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞; ｜＜赋值语句＞; ｜＜读语句＞; ｜＜写语句＞; ｜＜情况语句＞｜＜空＞; | ＜返回语句＞; | '{'＜语句列＞'}'
void getStatement() {
	if (symbol == WHILETK || symbol == FORTK) {
		getLoopStatement();
	}
	else if (symbol == IFTK) {
		getConditionalStatement();
	}
	else if (symbol == IDENFR) {
		// ＜有返回值函数调用语句＞; | ＜无返回值函数调用语句＞; ｜＜赋值语句＞; ｜
		string tmpIdenfr = TOKEN;
		transform(tmpIdenfr.begin(), tmpIdenfr.end(), tmpIdenfr.begin(), ::tolower);

		if (symList[curLevel].find(tmpIdenfr) == symList[curLevel].end()
			&& symList[PROGRAM].find(tmpIdenfr) == symList[PROGRAM].end()) {
			error('c');			// 检查是否有未定义的名字
		}
		else {
			if (functionReturnType.count(tmpIdenfr) > 0 && functionReturnType[tmpIdenfr] > 0) {
				int type = 2;			// 该变量无用，仅是为了充当函数参数
				getFunctionCallStatementWithReturnValue(type);
			}
			else if (functionReturnType.count(tmpIdenfr) > 0 && functionReturnType[tmpIdenfr] == 0) {
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

// ＜赋值语句＞ ::= ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞|＜标识符＞'['＜表达式＞']''['＜表达式＞']' =＜表达式＞
void getAssignmentStatement() {
	int dimension = 0, tmpExpressionType;
	string tmpIdentifier, indexExpression, columnExpression, valueExpression;
	
	getIdentifier(tmpIdentifier);

	if (symbol == LBRACK){
		getsym();
		dimension++;
		tmpExpressionType = 2;
		getExpression(tmpExpressionType, indexExpression);
		if (tmpExpressionType == 2)
			error('i');						// 数组下标不可为字符型
		checkSymbolandGetSym(RBRACK, 'm');	// 匹配']'
	}
	if (symbol == LBRACK) {
		getsym();
		dimension++;
		tmpExpressionType = 2;
		getExpression(tmpExpressionType, columnExpression);
		if (tmpExpressionType == 2) 
			error('i');						// 数组下标不可为字符型
		checkSymbolandGetSym(RBRACK, 'm');	// 匹配']'
	}

	checkSymbolandGetSym(ASSIGN, '0');		// 匹配 '='

	tmpExpressionType = 2;
	getExpression(tmpExpressionType, valueExpression);

	if (dimension == 0) {
		if (valueExpression[0] == '#') {
			// 如果上一条赋值语句的结果运算数为中间变量
			int index = midCodes.size() - 1;
			if (midCodes.back().op == INLINEEND) {
				index--;
			}
			if (midCodes[index].result == valueExpression) {
				midCodes[index].result = tmpIdentifier;
			}
			else {
				insertMidCode(ASSIGNOP, tmpIdentifier, valueExpression, "");
			}
		}
		else {
			insertMidCode(ASSIGNOP, tmpIdentifier, valueExpression, "");
		}
	}
	else if (dimension == 1) {
		insertMidCode(PUTARRAY, tmpIdentifier, indexExpression, valueExpression);
	}
	else if (dimension == 2) {
		insertMidCode(PUTARRAY2, tmpIdentifier, indexExpression, columnExpression, valueExpression);
	}

	ofile << "<赋值语句>" << endl;
}

// ＜条件语句＞ :== if '('＜条件＞')'＜语句＞［else＜语句＞］
void getConditionalStatement() {
	string ifLabel, elseLabel, tmpCondition;

	checkSymbolandGetSym(IFTK, '0');		// if
	checkSymbolandGetSym(LPARENT, '0');		// '('
	getCondition(tmpCondition);				// <条件>
	checkSymbolandGetSym(RPARENT, 'l');		// ')'
	
	ifLabel = genLabel();					// 创建 if 标签
	
	insertMidCode(BZ, ifLabel, "<>", "");
	string lbegin = genLabel("CB");
	insertMidCode(LABEL, lbegin, "", "");
	// insertMidCode(BZ, ifLabel, tmpCondition, "");

	getStatement();							// <语句>

	if (symbol == ELSETK) {
		getsym();
		elseLabel = genLabel();						// 创建 else 标签
		insertMidCode(GOTO, elseLabel, "", "");		// 无条件跳转到标签 else
		insertMidCode(LABEL, ifLabel, "", "");		// 在 else 语句开始处插入 if 标签
		getStatement();
		insertMidCode(LABEL, elseLabel, "", "");	// 在 else 语句结束处插入 else 标签
	}
	else {
		insertMidCode(LABEL, ifLabel, "", "");		// 没有 else 在语句最后插入 if 标签
	}

	string lend = genLabel("CE");
	insertMidCode(LABEL, lend, "", "");

	ofile << "<条件语句>" << endl;
}

// ＜条件＞ ::= ＜表达式＞＜关系运算符＞＜表达式＞
void getCondition(string& tmpCondition) {
	int leftExpressionType = 2, rightExpressionType = 2, tmpRelationalOperator;
	string resultExpression, leftExpression, rightExpression;
	midOp tmpMidOp = EQLOP;				// 该初始值不会用到

	/*
	resultExpression = genTmp();
	tmpCondition = resultExpression;
	insertSymbol(resultExpression, 1, 0);
	*/

	getExpression(leftExpressionType, leftExpression);		// <表达式>
	if (leftExpressionType == 2) {
		error('f');			// 表达式必须为整型
	}
	getRelationalOperator(tmpRelationalOperator);			// <关系运算符>
	getExpression(rightExpressionType, rightExpression);	// <表达式>
	if (rightExpressionType == 2) {
		error('f');			// 表达式必须为整型
	}

	switch (tmpRelationalOperator) {
	case LSS:
		tmpMidOp = LSSOP; break;
	case LEQ:
		tmpMidOp = LEQOP; break;
	case GRE:
		tmpMidOp = GREOP; break;
	case GEQ:
		tmpMidOp = GEQOP; break;
	case EQL:
		tmpMidOp = EQLOP; break;
	case NEQ:
		tmpMidOp = NEQOP; break;
	default:
		break;
	}

	insertMidCode(tmpMidOp, "<>", leftExpression, rightExpression);
	// insertMidCode(tmpMidOp, resultExpression, leftExpression, rightExpression);

	ofile << "<条件>" << endl;

}

// 暂未启用
// ＜条件＞ ::= ＜表达式＞＜关系运算符＞＜表达式＞ | <表达式>
void getCondition2(string& tmpCondition) {
	int leftExpressionType = 2, rightExpressionType = 2, tmpRelationalOperator;
	string resultExpression, leftExpression, rightExpression;
	midOp tmpMidOp = EQLOP;				// 该初始值不会用到

	getExpression(leftExpressionType, leftExpression);		// <表达式>
	if (leftExpressionType == 2) {
		error('f');			// 表达式必须为整型
	}

	if (symbol == LSS || symbol == LEQ || symbol == GRE || symbol == GEQ || symbol == EQL || symbol == NEQ) {
		getRelationalOperator(tmpRelationalOperator);			// <关系运算符>
		getExpression(rightExpressionType, rightExpression);	// <表达式>
		if (rightExpressionType == 2) {
			error('f');			// 表达式必须为整型
		}

		switch (tmpRelationalOperator) {
		case LSS: tmpMidOp = LSSOP; break;
		case LEQ: tmpMidOp = LEQOP; break;
		case GRE: tmpMidOp = GREOP; break;
		case GEQ: tmpMidOp = GEQOP; break;
		case EQL: tmpMidOp = EQLOP; break;
		case NEQ: tmpMidOp = NEQOP; break;
		default: break;
		}

		insertMidCode(tmpMidOp, "<>", leftExpression, rightExpression);

		ofile << "<条件>" << endl;
	}
	else {
		// 要求表达式 != 0
		insertMidCode(tmpMidOp, "<>", leftExpression, int2string(0));
		ofile << "<条件>" << endl;
	}
}

// ＜循环语句＞ :: = 
// while '('＜条件＞')'＜语句＞ | 
// for'('＜标识符＞＝＜表达式＞; ＜条件＞; ＜标识符＞＝＜标识符＞(+| -)＜步长＞')'＜语句＞
void getLoopStatement() {
	if (symbol == WHILETK) {
		getsym();

		string beginLabel, endLabel, tmpCondition;

		beginLabel = genLabel("LB");
		insertMidCode(LABEL, beginLabel, "", "");
		int conditionBeginIndex = midCodes.size();
		
		checkSymbolandGetSym(LPARENT, '0');				// '('
		getCondition(tmpCondition);						// <条件>
		checkSymbolandGetSym(RPARENT, 'l');				// ')'

		endLabel = genLabel("LE");
		insertMidCode(BZ, endLabel, "<>", "");			// 不满足条件(result==0)的话 跳转到labf 
		int conditionEndIndex = midCodes.size() - 1;
		string whileBody = genLabel("LO");
		insertMidCode(LABEL, whileBody, "", "");

		getStatement();									// <语句>

		map<string, string> nMap;
		for (int ix = conditionBeginIndex; ix < conditionEndIndex; ix++) {
			midCode mc = midCodes[ix];
			if (mc.left[0] == '#' && nMap.find(mc.left) == nMap.end()) {
				nMap[mc.left] = genTmp();
			}
			if (mc.right[0] == '#' && nMap.find(mc.right) == nMap.end()) {
				nMap[mc.right] = genTmp();
			}
			if (mc.result[0] == '#' && nMap.find(mc.result) == nMap.end()) {
				nMap[mc.result] = genTmp();
			}
			if (mc.left[0] == '%' && nMap.find(mc.left) == nMap.end()) {
				nMap[mc.left] = genName();
			}
			if (mc.right[0] == '%' && nMap.find(mc.right) == nMap.end()) {
				nMap[mc.right] = genName();
			}
			if (mc.result[0] == '%' && nMap.find(mc.result) == nMap.end()) {
				nMap[mc.result] = genName();
			}
		}
		for (int ix = conditionBeginIndex; ix < conditionEndIndex; ix++) {
			midCode mc = midCodes[ix];
			if (mc.left[0] == '#' || mc.left[0] == '%') {
				mc.left = nMap[mc.left];
			}
			if (mc.right[0] == '#' || mc.right[0] == '%') {
				mc.right = nMap[mc.right];
			}
			if (mc.result[0] == '#' || mc.result[0] == '%') {
				mc.result = nMap[mc.result];
			}
			midCodes.push_back(mc);
		}
		for (map<string, string>::iterator it = nMap.begin(); it != nMap.end(); it++) {
			string oriName = (*it).first;
			string newName = (*it).second;
			struct sym sitem = symList[curLevel][oriName];
			sitem.name = newName;
			sitem.addr = localAddr;
			insertSymbol(newName, sitem);
		}
		insertMidCode(BNZ, whileBody, "<>", "");//回到whileBody
		insertMidCode(LABEL, endLabel, "", "");			// 设置labf 用于结束循环
	}
	else if (symbol == FORTK) {
		getsym();

		int tmpExpressionType, tmpAdditionOperator, tmpStep;
		string beginLabel, endLabel, tmpIdentifier, tmpExpression, tmpCondition;
		string leftIdentifier, rightIdentifier;

		checkSymbolandGetSym(LPARENT, '0');					// '('
		
		// ＜标识符＞＝＜表达式＞
		getIdentifier(tmpIdentifier);						// <标识符>
		checkUndefinedIdentifier(tmpIdentifier);			// 检查是否有未定义的名字
		checkSymbolandGetSym(ASSIGN, '0');					// '='
		tmpExpressionType = 2;
		getExpression(tmpExpressionType, tmpExpression);	// <表达式>
		
		
		if (tmpExpression[0] == '#') {  
			// 若赋值语句的标识符为中间变量
			int index = midCodes.size() - 1;
			if (midCodes.back().op == INLINEEND) {
				index--;
			}
			if (midCodes[index].result == tmpExpression) {
				midCodes[index].result = tmpIdentifier;
			}
			else {
				insertMidCode(ASSIGNOP, tmpIdentifier, tmpExpression, "");
			}
		}
		else {
			insertMidCode(ASSIGNOP, tmpIdentifier, tmpExpression, "");
		}
		
		checkSymbolandGetSym(SEMICN, 'k');					// ';'

		beginLabel = genLabel("LB");
		insertMidCode(LABEL, beginLabel, "", "");			// 在条件判断前插入 beginLabel 标签
		int conditionBeginIndex = midCodes.size();
		getCondition(tmpCondition);							// <条件>
		endLabel = genLabel("LE");
		insertMidCode(BZ, endLabel, "<>", "");				// 不满足条件跳到 endLabel 结束循环
		int conditionEndIndex = midCodes.size() - 1;
		string forBody = genLabel("LO");
		insertMidCode(LABEL, forBody, "", "");

		checkSymbolandGetSym(SEMICN, 'k');					// ';'

		// ＜标识符＞＝＜标识符＞(+| -)＜步长＞
		getIdentifier(leftIdentifier);						// <标识符>
		checkUndefinedIdentifier(leftIdentifier);			// 检查是否有未定义的标识符
		checkSymbolandGetSym(ASSIGN, '0');					// '='
		getIdentifier(rightIdentifier);						// <标识符>
		checkUndefinedIdentifier(rightIdentifier);			// 检查是否有未定义的标识符
		getAdditionOperator(tmpAdditionOperator);			// (+|-)		
		getStep(tmpStep);									// <步长>
		checkSymbolandGetSym(RPARENT, 'l');					// ')'
		getStatement();										// <语句>

		insertMidCode(tmpAdditionOperator == PLUS ? PLUSOP : MINUOP, leftIdentifier, rightIdentifier, int2string(tmpStep));//增加步长
		
		
		map<string, string> nMap;
		for (int ix = conditionBeginIndex; ix < conditionEndIndex; ix++) {
			midCode mc = midCodes[ix];
			if (mc.left[0] == '#' && nMap.find(mc.left) == nMap.end()) {
				nMap[mc.left] = genTmp();
			}
			if (mc.right[0] == '#' && nMap.find(mc.right) == nMap.end()) {
				nMap[mc.right] = genTmp();
			}
			if (mc.result[0] == '#' && nMap.find(mc.result) == nMap.end()) {
				nMap[mc.result] = genTmp();
			}
			if (mc.left[0] == '%' && nMap.find(mc.left) == nMap.end()) {
				nMap[mc.left] = genName();
			}
			if (mc.right[0] == '%' && nMap.find(mc.right) == nMap.end()) {
				nMap[mc.right] = genName();
			}
			if (mc.result[0] == '%' && nMap.find(mc.result) == nMap.end()) {
				nMap[mc.result] = genName();
			}
		}
		for (int ix = conditionBeginIndex; ix < conditionEndIndex; ix++) {
			midCode mc = midCodes[ix];
			if (mc.left[0] == '#' || mc.left[0] == '%') {
				mc.left = nMap[mc.left];
			}
			if (mc.right[0] == '#' || mc.right[0] == '%') {
				mc.right = nMap[mc.right];
			}
			if (mc.result[0] == '#' || mc.result[0] == '%') {
				mc.result = nMap[mc.result];
			}
			midCodes.push_back(mc);
		}
		for (map<string, string>::iterator it = nMap.begin(); it != nMap.end(); it++) {
			string oriName = (*it).first;
			string newName = (*it).second;
			struct sym sitem = symList[curLevel][oriName];
			sitem.name = newName;
			sitem.addr = localAddr;
			symList[curLevel].insert(make_pair(newName, sitem));
		}
		insertMidCode(BNZ, forBody, "<>", "");		//回到forBody
		
		
		// insertMidCode(GOTO, beginLabel, "", "");			// 回到 beginLabel 进行条件判断
		insertMidCode(LABEL, endLabel, "", "");				// 插入 endLabel 结束循环
	}
	ofile << "<循环语句>" << endl;
}

// ＜步长＞::= ＜无符号整数＞  
void getStep(int& tmpStep) {
	getUnsignedInteger(tmpStep);
	ofile << "<步长>" << endl;
}

// ＜情况语句＞ ::= switch '('＜表达式＞')' '{'＜情况表＞＜缺省＞'}'
void getSwitch() {
	int tmpExpressionType = 2;
	string tmpExpression;

	string beginLabel = genLabel("SB");
	insertMidCode(LABEL, beginLabel, "", "");

	insertMidCode(ENTERSWITCH, "", "", "");						// 进入switch语句
	string endLabel = genLabel("SE");

	checkSymbolandGetSym(SWITCHTK, '0');						// switch
	checkSymbolandGetSym(LPARENT, '0');							// '('
	getExpression(tmpExpressionType, tmpExpression);			// <表达式>

	string switchExpression = genSTmp();
	insertSymbol(switchExpression, 1, 0);
	insertMidCode(ASSIGNOP, switchExpression, tmpExpression, "");

	checkSymbolandGetSym(RPARENT, 'l');							// ')'
	checkSymbolandGetSym(LBRACE, '0');							// '{'
	//getCaseTable(tmpExpressionType, tmpExpression, endLabel);	// <情况表>
	getCaseTable(tmpExpressionType, switchExpression, endLabel);	// <情况表>
	
	if (symbol != DEFAULTTK) 
		error('p');												// 缺少<缺省>
	else 
		getDefault(endLabel);											// <缺省>
	checkSymbolandGetSym(RBRACE, '0');							// '}'

	insertMidCode(EXITSWITCH, "", "", "");						// 退出switch语句
	insertMidCode(LABEL, endLabel, "", "");

	ofile << "<情况语句>" << endl;
}

// ＜情况表＞ ::= ＜情况子语句＞{ ＜情况子语句＞ }
void getCaseTable(int tmpExpressionType, string tmpExpression, string endLabel) {
	while (symbol == CASETK) {
		getCase(tmpExpressionType, tmpExpression, endLabel);
	}
	ofile << "<情况表>" << endl;
}

// ＜情况子语句＞ ::= case＜常量＞：＜语句＞
void getCase(int tmpExpressionType, string tmpExpression, string endLabel) {
	int tmpConstantType, tmpInteger;
	string tmpChar, resultExpression, leftExpression, rightExpression;

	checkSymbolandGetSym(CASETK, '0');					// case
	getConstant(tmpConstantType, tmpInteger, tmpChar);	// <常量>
	if (tmpConstantType != tmpExpressionType - 1) {
		error('o');			// switch语句中常量类型不一致
	}

	// 将情况子语句当作if语句处理
	resultExpression = genTmp();
	insertSymbol(resultExpression, 1, 0);
	leftExpression = tmpExpression;
	rightExpression = tmpConstantType == 0 ? int2string(tmpInteger) : tmpChar;
	
	insertMidCode(EQLOP, "<>", leftExpression, rightExpression);		//try
	// insertMidCode(EQLOP, resultExpression, leftExpression, rightExpression);		


	// 若不相同则进行跳转
	string caseLabel = genLabel();
	insertMidCode(BZ, caseLabel, "<>", "");
	string lbegin = genLabel("CB");
	insertMidCode(LABEL, lbegin, "", "");


	checkSymbolandGetSym(COLON, '0');			// ':'
	getStatement();								// <语句>

	insertMidCode(EXITCASE, "", "", "");
	insertMidCode(GOTO, endLabel, "", "");		// 无条件跳转到END
	
	//try
	insertMidCode(LABEL, caseLabel, "", "");
	string lend = genLabel("CE");
	insertMidCode(LABEL, lend, "", "");
												
	//insertMidCode(LABEL, caseLabel, "", "");

	ofile << "<情况子语句>" << endl;
}

// ＜缺省＞ ::= default :＜语句＞
void getDefault(string endLabel) {
	checkSymbolandGetSym(DEFAULTTK, '0');		// default
	checkSymbolandGetSym(COLON, '0');			// ':'

	string lbegin = genLabel("CB");
	insertMidCode(LABEL, lbegin, "", "");
	getStatement();								// <语句>
	insertMidCode(GOTO, endLabel, "", "");		// 无条件跳转到END
	string lend = genLabel("CE");
	insertMidCode(LABEL, lend, "", "");

	ofile << "<缺省>" << endl;
}

// ＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void getFunctionCallStatementWithReturnValue(int& type) {
	string tmpIdentifier;
	getIdentifier(tmpIdentifier);			// 标识符

	if (symList[curLevel].find(tmpIdentifier) == symList[curLevel].end()
		&& symList[PROGRAM].find(tmpIdentifier) == symList[PROGRAM].end()) {
		error('c');
	}
	else {
		// 如果该标识符为函数，且返回值为整型
		if (functionReturnType.count(tmpIdentifier) > 0 && functionReturnType[tmpIdentifier] == 1) {
			type = 1;
		}
	}

	checkSymbolandGetSym(LPARENT, '0');		// '('
	getValueParameterTable(tmpIdentifier);	// <值参数表>
	checkSymbolandGetSym(RPARENT, 'l');		// ')'

	if (funcInlineAble[tmpIdentifier]) {  //函数可以内联
		int be, en;
		dealInlineFunc(tmpIdentifier, be, en);
		insertMidCode(INLINEEND, int2string(be), int2string(en), "");
	}
	else {
		insertMidCode(CALL, tmpIdentifier, "", "");
	}

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

	checkSymbolandGetSym(LPARENT, '0');			// '('
	getValueParameterTable(tmpIdentifier);		// <值参数表>
	checkSymbolandGetSym(RPARENT, 'l');			// ')'

	if (funcInlineAble[tmpIdentifier]) {  //函数可以内联
		int be, en;
		dealInlineFunc(tmpIdentifier, be, en);
		insertMidCode(INLINEEND, int2string(be), int2string(en), "");
	}
	else {
		insertMidCode(CALL, tmpIdentifier, "", "");
	}

	ofile << "<无返回值函数调用语句>" << endl;
}

// ＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞             
void getValueParameterTable(string tmpFunctionCall) {
	string tmpExpression;
	int tmpParameterCount = 0, tmpExpressionType = 2;

	// 如果直接遇上分号，进行错误处理
	if (symbol == RPARENT || symbol == SEMICN) {
		ofile << "<值参数表>" << endl;		// 值参数表为空，直接返回
	}
	else {
		tmpExpressionType = 2;
		getExpression(tmpExpressionType, tmpExpression);		// <表达式>
		insertMidCode(PUSH, tmpExpression, "", tmpFunctionCall);
		// 判断值参数表和参数表中对应位置的参数类型是否相同
		if (tmpParameterCount < functionParameterCount[tmpFunctionCall] &&
			tmpExpressionType != functionParameterType[tmpFunctionCall][tmpParameterCount]) {
			error('e');
		}
		tmpParameterCount++;

		while (symbol == COMMA) {
			getsym();
			tmpExpressionType = 2;
			getExpression(tmpExpressionType, tmpExpression);	// <表达式>
			insertMidCode(PUSH, tmpExpression, "", tmpFunctionCall);
			// 判断值参数表和参数表中对应位置的参数类型是否相同
			if (tmpParameterCount < functionParameterCount[tmpFunctionCall] &&
				tmpExpressionType != functionParameterType[tmpFunctionCall][tmpParameterCount]) {
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

// ＜语句列＞ ::= ｛＜语句＞}
void getStatementList() {
	while (symbol != RBRACE)
		getStatement();
	ofile << "<语句列>" << endl;
}

// ＜读语句＞ ::= scanf '('＜标识符＞')' 
void getScanf() {
	string tmpIdentifier;

	checkSymbolandGetSym(SCANFTK, '0');				// scanf
	checkSymbolandGetSym(LPARENT, '0');				// '('
	getIdentifier(tmpIdentifier);					// <标识符>

	// 检查该标识符是否定义以及是否为常量
	if (symList[curLevel].count(tmpIdentifier) == 1) {
		if (symList[curLevel][tmpIdentifier].kind == 0) {
			error('j');
		}
	}
	else if (symList[PROGRAM].count(tmpIdentifier) == 1) {
		if (symList[PROGRAM][tmpIdentifier].kind == 0) {
			error('j');
		}
	}
	else {
		error('c');
	}

	insertMidCode(SCAN, tmpIdentifier, "", "");		// 将该语句添加到中间代码

	checkSymbolandGetSym(RPARENT, 'l');				// ')'

	ofile << "<读语句>" << endl;
}

// ＜写语句＞ ::= printf'('＜字符串＞,＜表达式＞')' | printf '('＜字符串＞')' | printf '('＜表达式＞')’
void getPrintf() {
	int tmpExpressionType = 2;
	string tmpString, tmpExpression;
	
	checkSymbolandGetSym(PRINTFTK, '0');	// printf
	checkSymbolandGetSym(LPARENT, '0');		// '('

	if (symbol == STRCON) {
		getString(tmpString);
		int p = -2;
		while ((p = tmpString.find("\\", p + 2)) != string::npos) {
			tmpString.replace(p, 1, "\\\\");
		}
		stringList.push_back(tmpString);
		
		if (symbol == COMMA) {
			// printf'('＜字符串＞,＜表达式＞')'
			getsym();
			getExpression(tmpExpressionType, tmpExpression);
			
			insertMidCode(PRINT, tmpString, "3", "");
			insertMidCode(PRINT, tmpExpression, int2string(tmpExpressionType), "");
			insertMidCode(PRINT, "EndLine", "4", "");
		}
		else {
			// printf'('＜字符串＞')'
			insertMidCode(PRINT, tmpString, "3", "");
			insertMidCode(PRINT, "EndLine", "4", "");
		}
	}
	else {
		// printf '('＜表达式＞')'
		getExpression(tmpExpressionType, tmpExpression);
		insertMidCode(PRINT, tmpExpression, int2string(tmpExpressionType), "");
		insertMidCode(PRINT, "EndLine", "4", "");
	}

	checkSymbolandGetSym(RPARENT, 'l');	// 匹配 ')'

	ofile << "<写语句>" << endl;
}

// ＜返回语句＞ ::= return['('＜表达式＞')']   
void getReturn() {
	checkSymbolandGetSym(RETURNTK, '0');		// return
	if (symbol == LPARENT) {
		checkSymbolandGetSym(LPARENT, '0');		// '('

		int tmpExpressionType = 2;
		string tmpExpression;
		getExpression(tmpExpressionType, tmpExpression);
		returnedType = tmpExpressionType;		// 获取函数实际返回值类型
		
		checkSymbolandGetSym(RPARENT, 'l');		// ')'

		if (functionReturnType.count(curLevel) > 0 && functionReturnType[curLevel] == 0) {
			error('g');							// 无返回值函数产生了返回值
		}

		if (curLevel != MAIN) {
			insertMidCode(RET, tmpExpression, "", "");
		}
		else {
			insertMidCode(EXIT, "", "", "");
		}
	}
	else {
		returnedType = 0;					// 仅有return;
		if (curLevel != MAIN) {
			insertMidCode(RET, "", "", "");
		}
		else {
			insertMidCode(EXIT, "", "", "");
		}
	}
	
	if (functionReturnType.count(curLevel) > 0 && functionReturnType[curLevel] != returnedType) {
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

void checkUndefinedIdentifier(string tmpIdentifier) {
	if (symList[curLevel].find(tmpIdentifier) == symList[curLevel].end()
		&& symList[PROGRAM].find(tmpIdentifier) == symList[PROGRAM].end()) {
		error('c');		// 检查是否有未定义的名字
	}
}

// 检查前一个定义完毕的函数是否可以内联
void checkBeforeFunc() {
	int i, j;
	string funcName;

	for (i = midCodes.size() - 1; i >= 0; i--) {
		// 寻找函数定义
		if (midCodes[i].op == FUNC) {
			funcName = midCodes[i].left;
			break;
		}
	}

	// 如果没有找到该函数或者该函数已经完成内联则直接返回
	if (i == -1 || funcMidCodeTable.find(funcName) != funcMidCodeTable.end()) {
		return;
	}
	else {
		vector<midCode> ve = vector<midCode>();
		bool flag = true;

		//参数+临时变量+局部变量 减去 参数个数
		int varCnt = symList[funcName].size() - functionParameterCount[funcName];

		// 遍历找临时变量
		for (map<string, sym>::iterator it = symList[funcName].begin(); it != symList[funcName].end(); it++) {
			if ((*it).first[0] == '#') {
				varCnt--;
			}

			if (symList[funcName][(*it).first].type == 2 || symList[funcName][(*it).first].type == 3 || symList[funcName][(*it).first].type == 5 || symList[funcName][(*it).first].type == 6) {
				flag = false;
			}

		}
		if (varCnt > 2) {  //局部变量超过2个 就不内联了
			flag = false;
		}

		//带有全局变量的不内联 全局常量可以 因为常量已经传播成了数字了
		for (j = i; j < midCodes.size(); j++) {
			midCode mc = midCodes[j];
			ve.push_back(mc);
			switch (mc.op) {
			case PLUSOP:
			case MINUOP:
			case MULTOP:
			case DIVOP:
				if (symList[funcName].find(mc.result) == symList[funcName].end() && symList[PROGRAM].find(mc.result) != symList[PROGRAM].end()) {  //全局变量
					flag = false;
				}
				if (symList[funcName].find(mc.right) == symList[funcName].end() && symList[PROGRAM].find(mc.right) != symList[PROGRAM].end()) {  //全局变量
					flag = false;
				}
				if (symList[funcName].find(mc.left) == symList[funcName].end() && symList[PROGRAM].find(mc.left) != symList[PROGRAM].end()) {  //全局变量
					flag = false;
				}
				break;
			case LSSOP:
			case LEQOP:
			case GREOP:
			case GEQOP:
			case EQLOP:
			case NEQOP:
				if (symList[funcName].find(mc.right) == symList[funcName].end() && symList[PROGRAM].find(mc.right) != symList[PROGRAM].end()) {  //全局变量
					flag = false;
				}
				if (symList[funcName].find(mc.left) == symList[funcName].end() && symList[PROGRAM].find(mc.left) != symList[PROGRAM].end()) {  //全局变量
					flag = false;
				}
				break;
			case ASSIGNOP:
			case GETARRAY:
				//mc.result << " = " << mc.left << "[" << mc.right << "]
				// 二维数组 TODO
				if (symList[funcName].find(mc.result) == symList[funcName].end() && symList[PROGRAM].find(mc.result) != symList[PROGRAM].end()) {  //全局变量
					flag = false;
				}
				if (symList[funcName].find(mc.left) == symList[funcName].end() && symList[PROGRAM].find(mc.left) != symList[PROGRAM].end()) {  //全局变量
					flag = false;
				}
				break;
			case GOTO:
			case BZ:
			case BNZ:
			case LABEL:
			case PUSH:
			case CALL:
			case RETVALUE:
				flag = false;
				break;
			case RET:
			case INLINERET:
			case SCAN:
				if (symList[funcName].find(mc.result) == symList[funcName].end() && symList[PROGRAM].find(mc.result) != symList[PROGRAM].end()) {  //全局变量
					flag = false;
				}
				break;
			case PRINT:
				if (mc.left == "1" || mc.left == "2") {
					if (symList[funcName].find(mc.result) == symList[funcName].end() && symList[PROGRAM].find(mc.result) != symList[PROGRAM].end()) {  //全局变量
						flag = false;
					}
				}
				break;
			case CONST:
			case ARRAY:
			case VAR:
			case PARAM:
				if (symList[funcName].find(mc.left) == symList[funcName].end() && symList[PROGRAM].find(mc.left) != symList[PROGRAM].end()) {
					flag = false;
				}
				break;
			case PUTARRAY:
				//mc.result << "[" << mc.left << "]" << " = " << mc.right
				if (symList[funcName].find(mc.result) == symList[funcName].end() && symList[PROGRAM].find(mc.result) != symList[PROGRAM].end()) {
					flag = false;
				}
				if (symList[funcName].find(mc.right) == symList[funcName].end() && symList[PROGRAM].find(mc.right) != symList[PROGRAM].end()) {
					flag = false;
				}
				break;
			case FUNC:
			case EXIT:
			default:
				break;
			}
		}
		funcMidCodeTable.insert(make_pair(funcName, ve));
		funcInlineAble.insert(make_pair(funcName, flag));
		return;
	}
}

// 更改内联函数的名称
void fullNameMap(map<string, string>& nameMap, vector<midCode> ve, string funcName) {
	for (int i = 0; i < ve.size(); i++) {
		midCode mc = ve[i];
		switch (mc.op) {
		case PLUSOP:
		case MINUOP:
		case MULTOP:
		case DIVOP:
		case GETARRAY:  //mc.result << " = " << mc.left << "[" << mc.right << "]
		case PUTARRAY:  //mc.result << "[" << mc.left << "]" << " = " << mc.right
			if (symList[funcName].find(mc.result) != symList[funcName].end() && nameMap.find(mc.result) == nameMap.end()) {
				if (mc.result[0] == '#') {
					nameMap[mc.result] = genTmp();
				}
				else {
					nameMap[mc.result] = genName();
				}
			}
			if (symList[funcName].find(mc.right) != symList[funcName].end() &&  //局部变量
				nameMap.find(mc.right) == nameMap.end()) {
				if (mc.right[0] == '#') {
					nameMap[mc.right] = genTmp();
				}
				else {
					nameMap[mc.right] = genName();
				}
			}
			if (symList[funcName].find(mc.left) != symList[funcName].end() &&  //局部变量
				nameMap.find(mc.left) == nameMap.end()) {
				if (mc.left[0] == '#') {
					nameMap[mc.left] = genTmp();
				}
				else {
					nameMap[mc.left] = genName();
				}
			}
			break;
		case LSSOP:
		case LEQOP:
		case GREOP:
		case GEQOP:
		case EQLOP:
		case NEQOP:
			if (symList[funcName].find(mc.right) != symList[funcName].end() &&  //局部变量
				nameMap.find(mc.right) == nameMap.end()) {
				if (mc.right[0] == '#') {
					nameMap[mc.right] = genTmp();
				}
				else {
					nameMap[mc.right] = genName();
				}
			}
			if (symList[funcName].find(mc.left) != symList[funcName].end() &&  //局部变量
				nameMap.find(mc.left) == nameMap.end()) {
				if (mc.left[0] == '#') {
					nameMap[mc.left] = genTmp();
				}
				else {
					nameMap[mc.left] = genName();
				}
			}
			break;
		case ASSIGNOP:
			if (symList[funcName].find(mc.result) != symList[funcName].end() &&  //局部变量
				nameMap.find(mc.result) == nameMap.end()) {
				if (mc.result[0] == '#') {
					nameMap[mc.result] = genTmp();
				}
				else {
					nameMap[mc.result] = genName();
				}
			}
			if (symList[funcName].find(mc.left) != symList[funcName].end() &&  //局部变量
				nameMap.find(mc.left) == nameMap.end()) {
				if (mc.left[0] == '#') {
					nameMap[mc.left] = genTmp();
				}
				else {
					nameMap[mc.left] = genName();
				}
			}
			break;
		case GOTO:
		case BZ:
		case BNZ:
		case LABEL:
			if (symList[funcName].find(mc.result) != symList[funcName].end() &&  //局部变量
				nameMap.find(mc.result) == nameMap.end()) {
				nameMap[mc.result] = genLabel();
			}
			break;
		case RET:
		case INLINERET:
		case SCAN:
			if (symList[funcName].find(mc.result) != symList[funcName].end() &&  //局部变量
				nameMap.find(mc.result) == nameMap.end()) {
				if (mc.result[0] == '#') {
					nameMap[mc.result] = genTmp();
				}
				else {
					nameMap[mc.result] = genName();
				}
			}
			break;
		case PUSH:
		case CALL:
		case RETVALUE:
			break;
		case PRINT:
			if (mc.left == "1" || mc.left == "2") {
				if (symList[funcName].find(mc.result) != symList[funcName].end() &&  //局部变量
					nameMap.find(mc.result) == nameMap.end()) {
					if (mc.result[0] == '#') {
						nameMap[mc.result] = genTmp();
					}
					else {
						nameMap[mc.result] = genName();
					}
				}
			}
			break;
		case CONST:
		case ARRAY:
		case VAR:
		case PARAM:
			if (symList[funcName].find(mc.left) != symList[funcName].end() &&  //局部变量
				nameMap.find(mc.left) == nameMap.end()) {
				if (mc.left[0] == '#') {
					nameMap[mc.left] = genTmp();
				}
				else {
					nameMap[mc.left] = genName();
				}
			}
			break;
		case FUNC:
		case EXIT:
			break;
		default:
			break;
		}
	}
}

// 处理内联函数
void dealInlineFunc(string name, int& begin, int& end) {
	vector<midCode> ve = funcMidCodeTable[name];  //这个函数所有的中间代码
	//函数内联需要修改这个函数里边的变量名常量名数组名
	map<string, string> nameMap;
	begin = nameId + 1;
	fullNameMap(nameMap, ve, name);
	end = nameId;

	//传参改成赋值
	int paramSize = functionParameterCount[name];  //参数个数
	for (int i = midCodes.size() - 1, j = paramSize; i >= 0; i--) {
		if (midCodes[i].op == PUSH && midCodes[i].right == name) {  //不一定就是midCodes的最后几个是PUSH
			midCodes[i].op = ASSIGNOP;
			midCodes[i].left = midCodes[i].result;
			midCodes[i].result = nameMap[ve[j].left];
			midCodes[i].right = "";
			j--;
			if (j == 0) {
				break;
			}
		}
	}
	//改各种名字
	for (int i = paramSize + 1; i < ve.size(); i++) {
		midCode mc = ve[i];
		if (nameMap.find(mc.result) != nameMap.end()) {
			mc.result = nameMap[mc.result];
		}
		if (nameMap.find(mc.left) != nameMap.end()) {
			mc.left = nameMap[mc.left];
		}
		if (nameMap.find(mc.right) != nameMap.end()) {
			mc.right = nameMap[mc.right];
		}
		if (mc.op == RET) {
			if (mc.result == "") { //无返回值函数
				break;  //正常是continue 但是到了RET 后边就没了 直接break
			}
			else {
				mc.op = INLINERET;
			}
		}
		midCodes.push_back(mc);
		if (mc.op == RET) {
			break;
		}
	}
	//符号表也得改
	for (map<string, struct sym>::iterator it = symList[name].begin(); it != symList[name].end(); it++) {
		struct sym si = (*it).second;
		si.addr = localAddr;
		if (si.kind == 4) {  //数组类型
			localAddr += si.length;
		}
		else {
			localAddr++;
		}
		if (nameMap.find(si.name) != nameMap.end()) {
			si.name = nameMap[si.name];
		}
		symList[curLevel].insert(make_pair(si.name, si));
	}
}