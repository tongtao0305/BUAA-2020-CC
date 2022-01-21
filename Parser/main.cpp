#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
using namespace std;

ifstream ifile;
ofstream ofile;

// 词法分析相关变量
string CODEN[40];			// 保存单词类别编码
string CODES[40];			// 保存单词对应字符串
char buffer[256];			// 文件输入缓冲区
int cnt = 0;				// 记录下一个读取的字符的位置
int cnt_len = 0;			// 记录当前缓冲区字符串的长度
int cnt_line = 0;			// 记录当前缓冲区在源文件中的行数，以便进行错误输出
int symbol;					// 保存当前所识别单词的类型
char CHAR;					// 存放当前读入的字符
char TOKEN[256];			// 存放单词字符串

// 词法分析相关函数
extern void getsym();	// 获取下一个 token，返回-1表示出错，返回-2表示文件读取结束
extern void outputsym();
void init_CODEN();
void init_CODES();

// 语法分析相关函数
extern void getProgram();

// 错误处理函数
void error();			

int main() {
	ifile.open("testfile.txt", ios::in);
	ofile.open("output.txt", ios::out);

	init_CODEN();
	init_CODES();

	// 词法分析和语法分析
	getsym();
	getProgram();

	ifile.close();
	ofile.close();
	return 0;
}

void error() {
	cout << "Error happened at line " << cnt_line << endl;
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