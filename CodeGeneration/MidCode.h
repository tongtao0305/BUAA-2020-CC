#pragma once

#include "Global.h"
using namespace std;

// ����Ϊ�ⲿ���õı����ͺ���
extern ofstream midCodeFile;
extern vector <struct midCode> midCodes;
extern map<string, map<string, struct sym>> symList;

// ����Ϊ�ڲ�����ı����ͺ���
// ��������
void insertMidCode(string result, string left, midOp op, string right);

void outputMidCodes();

void outputMidCode(struct midCode m);