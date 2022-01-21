#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
using namespace std;

void getsym();				// ***��ȡ��һ�� token
void trysym();				// ���Ի�ȡ��һ�� token�������
void ungetsym();			// ����һ�� token
void outputsym();			// ��� token
void outputlastsym();		// �����һ�� token
extern void error();		// ��������

int isDigit(char c);		// �ж��ַ��Ƿ�Ϊ����
int isLetter(char c);		// �ж��ַ��Ƿ�Ϊ��ĸ
int isChar(char c);			// �ж��Ƿ�Ϊ�ַ�
int isString(char c);		// �ж��Ƿ�Ϊ�ַ����е��ַ�
void cat(char s[], char c); // ���ַ����뵽�ַ�����ĩβ
int getnbc();				// ���ַ���������ȡ��һ���ǿ��ַ������� 1 ��ʾ�ɹ������� 0 ��ʾʧ��
void getch();				// ���ַ���������ȡһ���ַ� 
void ungetch();				// ���ַ�����������һ���ַ� 
void reserve(char s[]);		// �жϻ�ȡ�����ַ����Ƿ�Ϊ������
void tolowercase(char s[]);	// ���ַ����еĴ�дӢ���ַ�ת��ΪСд

extern int symbol;			// ���浱ǰ��ʶ�𵥴ʵ�����
extern char CHAR;			// ��ŵ�ǰ������ַ�
extern char TOKEN[256];		// ��ŵ����ַ���

int lastBegin;				// ��һ��Token��ʼλ��
int lastSymbol;				// ��һ��Token������
char lastTOKEN[256];		// ��һ��Token���ַ���

int lastlastBegin;			// ���ϸ�Token��ʼλ��
int lastlastSymbol;			// ���ϸ�Token������
char lastlastTOKEN[256];	// ���ϸ�Token���ַ���

extern char buffer[256];	// �ļ����뻺����
extern int cnt;				// ��¼��һ����ȡ���ַ���λ��
extern int cnt_len;			// ��¼��ǰ�������ַ����ĳ���
extern int cnt_line;		// ��¼��ǰ��������Դ�ļ��е��������Ա���д������

extern string CODEN[40];
extern string CODES[40];

extern ifstream ifile;
extern ofstream ofile;

int isFirst = 1;

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

void cat(char s[], char c) {
	int len = strlen(s);
	s[len] = c;
	s[len + 1] = '\0';
	return;
}

void getsym() {
	if (isFirst == 1) {
		isFirst = 0;
	}
	else {
		outputsym();
		lastlastSymbol = lastSymbol;
		strcpy(lastlastTOKEN, lastTOKEN);
		lastSymbol = symbol;
		strcpy(lastTOKEN, TOKEN);
	}

	symbol = -1;
	TOKEN[0] = '\0';
	// ��ȡһ���ǿ��ַ��������ȡʧ����ֱ�ӷ���
	while (getnbc() == 0) {
		if (!ifile.eof()) {
			cnt = 0;
			cnt_line++;
			ifile.getline(buffer, 256);
			cnt_len = strlen(buffer);
		}
		else {
			symbol = -2;
			return;
		}
	}

	if (isLetter(CHAR)) {
		while (isDigit(CHAR) || isLetter(CHAR)) {
			cat(TOKEN, CHAR);
			getch();
		}
		ungetch();

		char tmpTOKEN[1000];
		strcpy(tmpTOKEN, TOKEN);
		tolowercase(tmpTOKEN);
		reserve(tmpTOKEN);
	}
	else if (isDigit(CHAR)) {
		while (isDigit(CHAR)) {
			cat(TOKEN, CHAR);
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

void trysym() {
	if (isFirst == 1) {
		isFirst = 0;
	}
	else {
		lastlastSymbol = lastSymbol;
		strcpy(lastlastTOKEN, lastTOKEN);
		lastSymbol = symbol;
		strcpy(lastTOKEN, TOKEN);
	}

	symbol = -1;
	TOKEN[0] = '\0';
	// ��ȡһ���ǿ��ַ��������ȡʧ����ֱ�ӷ���
	while (getnbc() == 0) {
		if (!ifile.eof()) {
			cnt = 0;
			cnt_line++;
			ifile.getline(buffer, 256);
			cnt_len = strlen(buffer);
		}
		else {
			symbol = -2;
			return;
		}
	}

	if (isLetter(CHAR)) {
		while (isDigit(CHAR) || isLetter(CHAR)) {
			cat(TOKEN, CHAR);
			getch();
		}
		ungetch();

		char tmpTOKEN[1000];
		strcpy(tmpTOKEN, TOKEN);
		tolowercase(tmpTOKEN);
		reserve(tmpTOKEN);
	}
	else if (isDigit(CHAR)) {
		while (isDigit(CHAR)) {
			cat(TOKEN, CHAR);
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

void ungetsym() {
	cnt = lastBegin;
	symbol = lastSymbol;
	strcpy(TOKEN, lastTOKEN);

	lastBegin = lastlastBegin;
	lastSymbol = lastlastSymbol;
	strcpy(lastTOKEN, lastlastTOKEN);
}

void outputsym() {
	if (0 <= symbol && symbol <= 18)
		ofile << CODEN[symbol] << " " << TOKEN << endl;
	else if (19 <= symbol && symbol <= 38)
		ofile << CODEN[symbol] << " " << CODES[symbol] << endl;
}

void outputlastsym() {
	if (0 <= lastSymbol && lastSymbol <= 18)
		ofile << CODEN[lastSymbol] << " " << lastTOKEN << endl;
	else if (19 <= lastSymbol && lastSymbol <= 38)
		ofile << CODEN[lastSymbol] << " " << CODES[lastSymbol] << endl;
}

void outputlastlastsym() {
	if (0 <= lastlastSymbol && lastlastSymbol <= 18)
		ofile << CODEN[lastlastSymbol] << " " << lastlastTOKEN << endl;
	else if (19 <= lastlastSymbol && lastlastSymbol <= 38)
		ofile << CODEN[lastlastSymbol] << " " << CODES[lastlastSymbol] << endl;
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
		lastlastBegin = lastBegin;
		lastBegin = cnt;
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