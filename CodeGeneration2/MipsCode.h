#pragma once

#include "Global.h"
using namespace std;

// ����Ϊ�ⲿ���õı����ͺ���
extern ofstream mipsCodeFile;
extern map<string, map<string, struct sym>> symList;
extern vector <struct midCode> midCodes;
extern vector <struct mipsCode> mipsCodes;

extern  map <string, int> functionParameterCount;	// ��¼��������������
vector<string> stringList;			//�������е��ַ���

string curFuncName = "";			// ��¼��ǰ���ں�������
extern int string2int(string s);
extern string int2string(int t);

// ����Ϊ�ڲ�����ı����ͺ���
void generateMipsCodes();

void loadValue(string symbolName, string regName, bool gene, int& va, bool& get);

void storeValue(string symbolName, string regName);	

void insertMipsCode(mipsOp op, string result, string left, string right, int imm);

void insertMipsCode(mipsOp op, string result, string left, string right);

void outputMipsCodes();

void outputMipsCode(struct mipsCode mc);