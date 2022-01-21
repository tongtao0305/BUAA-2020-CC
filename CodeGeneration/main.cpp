#include "main.h"

int main() {
	
	openFiles();			// 打开所有文件

	getsym();				// 词法分析

	getProgram();			// 语法分析

	outputMidCodes();		// 输出中间代码

	generateMipsCodes();	// 生成mips代码
	outputMipsCodes();		// 输出mips代码

	closeFiles();			// 关闭所有文件

	return 0;
}

void openFiles() {
	ifile.open("testfile.txt", ios::in);
	ofile.open("output.txt", ios::out);
	errorFile.open("error.txt", ios::out);
	midCodeFile.open("midcode.txt", ios::out);
	mipsCodeFile.open("mips.txt", ios::out);
	init_CODEN();
	init_CODES();
}

void closeFiles() {
	ifile.close();
	errorFile.close();
	midCodeFile.close();
	mipsCodeFile.close();
}

void init_CODEN() {
	CODEN[0] = "IDENFR";        // 标识符
	CODEN[1] = "INTCON";        // 整型常量
	CODEN[2] = "CHARCON";       // 字符常量
	CODEN[3] = "STRCON";        // 字符串
	CODEN[4] = "CONSTTK";
	CODEN[5] = "INTTK";
	CODEN[6] = "CHARTK";
	CODEN[7] = "VOIDTK";
	CODEN[8] = "MAINTK";
	CODEN[9] = "IFTK";
	CODEN[10] = "ELSETK";
	CODEN[11] = "SWITCHTK";
	CODEN[12] = "CASETK";
	CODEN[13] = "DEFAULTTK";
	CODEN[14] = "WHILETK";
	CODEN[15] = "FORTK";
	CODEN[16] = "SCANFTK";
	CODEN[17] = "PRINTFTK";
	CODEN[18] = "RETURNTK";
	CODEN[19] = "PLUS";
	CODEN[20] = "MINU";
	CODEN[21] = "MULT";
	CODEN[22] = "DIV";
	CODEN[23] = "LSS";
	CODEN[24] = "LEQ";
	CODEN[25] = "GRE";
	CODEN[26] = "GEQ";
	CODEN[27] = "EQL";
	CODEN[28] = "NEQ";
	CODEN[29] = "COLON";
	CODEN[30] = "ASSIGN";
	CODEN[31] = "SEMICN";
	CODEN[32] = "COMMA";
	CODEN[33] = "LPARENT";
	CODEN[34] = "RPARENT";
	CODEN[35] = "LBRACK";
	CODEN[36] = "RBRACK";
	CODEN[37] = "LBRACE";
	CODEN[38] = "RBRACE";
}

void init_CODES() {
	CODES[0] = "IDENFR";        // 标识符
	CODES[1] = "INTCON";        // 整型常量
	CODES[2] = "CHARCON";       // 字符常量
	CODES[3] = "STRCON";        // 字符串
	CODES[4] = "const";
	CODES[5] = "int";
	CODES[6] = "char";
	CODES[7] = "void";
	CODES[8] = "main";
	CODES[9] = "if";
	CODES[10] = "else";
	CODES[11] = "switch";
	CODES[12] = "case";
	CODES[13] = "default";
	CODES[14] = "while";
	CODES[15] = "for";
	CODES[16] = "scanf";
	CODES[17] = "printf";
	CODES[18] = "return";
	CODES[19] = "+";
	CODES[20] = "-";
	CODES[21] = "*";
	CODES[22] = "/";
	CODES[23] = "<";
	CODES[24] = "<=";
	CODES[25] = ">";
	CODES[26] = ">=";
	CODES[27] = "==";
	CODES[28] = "!=";
	CODES[29] = ":";
	CODES[30] = "=";
	CODES[31] = ";";
	CODES[32] = ",";
	CODES[33] = "(";
	CODES[34] = ")";
	CODES[35] = "[";
	CODES[36] = "]";
	CODES[37] = "{";
	CODES[38] = "}";
}

string int2string(int t) {
	stringstream ss;
	ss << t;
	return ss.str();
}

int string2int(string s) {
	stringstream ss;
	ss << s;
	int t;
	ss >> t;
	return t;
}

string genLabel() {
	labelId++;
	return "Label" + int2string(labelId);
}

string genTmp() {
	tmpVarId++;
	return "T" + int2string(tmpVarId);
}