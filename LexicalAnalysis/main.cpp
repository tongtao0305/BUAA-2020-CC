#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
using namespace std;

#define ifilepath "E:\\Develop\\Code\\BUAA-2020-Compiler\\testfile\\testfile.txt"
#define ofilepath "E:\\Develop\\Code\\BUAA-2020-Compiler\\testfile\\output.txt"

int isDigit(char c);		// 判断字符是否为数字
int isLetter(char c);		// 判断字符是否为字母
int isChar(char c);			// 判断是否为字符
int isString(char c);		// 判断是否为字符串中的字符
void error();				// 错误处理函数
void cat(char s[], char c); // 将字符插入到字符串的末尾
void init_CODEN();			// 初始化单词类别编码
void init_CODES();			// 初始化单词对应的字符串
void getsym();				// ***获取下一个 token
int getnbc();				// 从字符缓冲区获取下一个非空字符，返回 1 表示成功，返回 0 表示失败
void getch();				// 从字符缓冲区获取一个字符 
void ungetch();				// 从字符缓冲区回退一个字符 
void reserve(char s[]);		// 判断获取到的字符串是否为保留字
void tolowercase(char s[]);	// 将字符串中的大写英文字符转化为小写

char CHAR;					// 存放当前读入的字符
char TOKEN[256];			// 存放单词字符串
char buffer[256];			// 文件输入缓冲区
int symbol;					// 保存当前所识别单词的类型
int cnt = 0;				// 记录下一个读取的字符的位置
int cnt_len = 0;			// 记录当前缓冲区字符串的长度
int cnt_line = 0;			// 记录当前缓冲区在源文件中的行数，以便进行错误输出
int num = 0;				// 存放当前读入的整型数值
string CODEN[40];			// 保存单词类别编码
string CODES[40];			// 保存单词对应字符串

int main() {
	ifstream ifile;
	ofstream ofile;
	ifile.open("testfile.txt", ios::in);
	ofile.open("output.txt", ios::out);

	init_CODEN();		// 初始化单词类别编码
	init_CODES();		// 初始化单词对应字符串

	while (!ifile.eof()) {
		// 初始化相关变量
		cnt = 0;
		cnt_line++;
		// 从文件中读取一行字符串
		ifile.getline(buffer, 256);
		cnt_len = strlen(buffer);
		// 对该行字符串进行分析
		while (cnt < strlen(buffer)) {
			getsym();
			if (0 <= symbol && symbol <= 18)
				ofile << CODEN[symbol] << " " << TOKEN << endl;
			else if (19 <= symbol && symbol <= 38)
				ofile << CODEN[symbol] << " " << CODES[symbol] << endl;
		}
	}

	ifile.close();
	ofile.close();
	return 0;
}

int isDigit(char c) {
	if ('0' <= c && c <= '9')
		return 1;
	else
		return 0;
}

int isLetter(char c) {
	if ((c == '_') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
		return 1;
	else
		return 0;
}

int isChar(char c) {
	if (c == '+' || c == '-' || c == '*' || c == '/' || isDigit(c) || isLetter(c))
		return 1;
	else
		return 0;
}

int isString(char c) {
	if (c == 32 || c == 33 || (35 <= c && c <= 126))
		return 1;
	else
		return 0;
}

void error() {
	cout << "Error happened at line " << cnt_line << endl;
}

void cat(char s[], char c) {
	int len = strlen(s);
	s[len] = c;
	s[len + 1] = '\0';
	return;
}

void getsym() {
	symbol = -1;
	num = 0;
	TOKEN[0] = '\0';

	// 获取一个非空字符，如果获取失败则直接返回
	if (getnbc() == 0) {
		return;
	}

	if (isLetter(CHAR)) {
		while (isDigit(CHAR) || isLetter(CHAR)) {
			cat(TOKEN, CHAR);
			getch();
		}
		ungetch();

		char tmpTOKEN[1000];
		strcpy_s(tmpTOKEN, TOKEN);
		tolowercase(tmpTOKEN);
		reserve(tmpTOKEN);
	}
	else if (isDigit(CHAR)) {
		while (isDigit(CHAR)) {
			cat(TOKEN, CHAR);
			num = num * 10 + CHAR - '0';
			getch();
		}
		ungetch();
		symbol = 1;
	}
	else if (CHAR == '\'') {
		getch();
		cat(TOKEN, CHAR);
		getch();
		if (CHAR != '\'') {
			error();
			ungetch();
		}
		else
			symbol = 2;
	}
	else if (CHAR == '\"') {
		getch();
		while (CHAR != '\"') {
			cat(TOKEN, CHAR);
			getch();
		}
		symbol = 3;
	}
	else if (CHAR == '+') symbol = 19;
	else if (CHAR == '-') symbol = 20;
	else if (CHAR == '*') symbol = 21;
	else if (CHAR == '/') symbol = 22;
	else if (CHAR == '<') {
		getch();
		if (CHAR == '=')
			symbol = 24;
		else {
			symbol = 23;
			ungetch();
		}
	}
	else if (CHAR == '>') {
		getch();
		if (CHAR == '=')
			symbol = 26;
		else {
			symbol = 25;
			ungetch();
		}
	}
	else if (CHAR == '=') {
		getch();
		if (CHAR == '=')
			symbol = 27;
		else {
			symbol = 30;
			ungetch();
		}
	}
	else if (CHAR == '!') {
		getch();
		if (CHAR == '=')
			symbol = 28;
		else {
			error();
			ungetch();
		}
	}
	else if (CHAR == ':') symbol = 29;
	else if (CHAR == ';') symbol = 31;
	else if (CHAR == ',') symbol = 32;
	else if (CHAR == '(') symbol = 33;
	else if (CHAR == ')') symbol = 34;
	else if (CHAR == '[') symbol = 35;
	else if (CHAR == ']') symbol = 36;
	else if (CHAR == '{') symbol = 37;
	else if (CHAR == '}') symbol = 38;

	if (symbol < 0) {
		error();
	}
}

void reserve(char s[]) {
	if (strcmp(s, "const") == 0) symbol = 4;
	else if (strcmp(s, "int") == 0) symbol = 5;
	else if (strcmp(s, "char") == 0) symbol = 6;
	else if (strcmp(s, "void") == 0) symbol = 7;
	else if (strcmp(s, "main") == 0) symbol = 8;
	else if (strcmp(s, "if") == 0) symbol = 9;
	else if (strcmp(s, "else") == 0) symbol = 10;
	else if (strcmp(s, "switch") == 0) symbol = 11;
	else if (strcmp(s, "case") == 0) symbol = 12;
	else if (strcmp(s, "default") == 0) symbol = 13;
	else if (strcmp(s, "while") == 0) symbol = 14;
	else if (strcmp(s, "for") == 0) symbol = 15;
	else if (strcmp(s, "scanf") == 0) symbol = 16;
	else if (strcmp(s, "printf") == 0) symbol = 17;
	else if (strcmp(s, "return") == 0) symbol = 18;
	else symbol = 0;
}

void tolowercase(char s[]) {
	int len = strlen(s);
	for (int i = 0; i < len; i++) {
		if (isLetter(s[i])) {
			s[i] = tolower(s[i]);
		}
	}
}

int getnbc() {
	while ((buffer[cnt] == ' ' || buffer[cnt] == '\t' || buffer[cnt] == '\n' || buffer[cnt] == '\r') && cnt < cnt_len) {
		cnt++;
	}
	if (cnt >= cnt_len) {
		return 0;
	} else {
		CHAR = buffer[cnt];
		cnt++;
		return 1;
	}
}

void getch() {
	CHAR = buffer[cnt];
	cnt++;
}

void ungetch() {
	cnt--;
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