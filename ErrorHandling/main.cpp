#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <string.h>
using namespace std;

ifstream ifile;
ofstream ofile;
ofstream errorFile;			// ��������ļ�

// �ʷ�������ر���
string CODEN[40];			// ���浥��������
string CODES[40];			// ���浥�ʶ�Ӧ�ַ���
char buffer[256];			// �ļ����뻺����
int cnt = 0;				// ��¼��һ����ȡ���ַ���λ��
int cnt_len = 0;			// ��¼��ǰ�������ַ����ĳ���
int cnt_line = 0;			// ��¼��ǰ��������Դ�ļ��е��������Ա���д������
int symbol;					// ���浱ǰ��ʶ�𵥴ʵ�����
char CHAR;					// ��ŵ�ǰ������ַ�
char TOKEN[256];			// ��ŵ����ַ���

// �ʷ�������غ���
extern void getsym();	// ��ȡ��һ�� token������-1��ʾ��������-2��ʾ�ļ���ȡ����
extern void outputsym();
void init_CODEN();
void init_CODES();

// �﷨������غ���
extern void getProgram();

// ��������
extern void error(char errorCode);

// ���ű�
struct sym {
	// ����Ϊͨ�ò���
	string name;
	int kind;	// ���ŵ����࣬0Ϊ��������1Ϊ���������2Ϊ��������3Ϊ����������
	int type;	// ���ŵ����ͣ�0Ϊint��1Ϊchar
	int dimension;	// ��Ϊ�������ͣ���¼��ά�ȣ���Ϊ�������ͣ���¼���������

	// ����Ϊ�������
	int num;	// ��¼��ֵ�ͷ��ŵ�ֵ
	string str;	// ��¼�ַ����ַ����ͷ��ŵ�ֵ
	int index;		// ��Ϊ�������ͣ���¼������
	int columns;	// ��Ϊ�������ͣ���¼������
};
map<string, map<string, struct sym>> symList;
string curLevel;					// ��¼��ǰ���ڵķ��ű�������
map<string, struct sym> curMap;		// ��¼��ǰ���ڵķ��ű��ε�map
struct sym curSym;					// ��¼��ǰ����ʹ�õķ���

int main() {
	ifile.open("testfile.txt", ios::in);
	errorFile.open("error.txt", ios::out);

	init_CODEN();
	init_CODES();

	// �ʷ��������﷨����
	getsym();
	getProgram();

	ifile.close();
	errorFile.close();
	return 0;
}

void init_CODEN() {
	CODEN[0] = "IDENFR";        // ��ʶ��
	CODEN[1] = "INTCON";        // ���ͳ���
	CODEN[2] = "CHARCON";       // �ַ�����
	CODEN[3] = "STRCON";        // �ַ���
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
	CODES[0] = "IDENFR";        // ��ʶ��
	CODES[1] = "INTCON";        // ���ͳ���
	CODES[2] = "CHARCON";       // �ַ�����
	CODES[3] = "STRCON";        // �ַ���
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