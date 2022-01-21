#pragma once

#include "Global.h"

ifstream ifile;				// Դ���� �����ļ�
ofstream ofile;				// �﷨���� ����ļ�
ofstream errorFile;			// ������ ����ļ�
ofstream midCodeFile;		// �м���� ����ļ�
ofstream mipsCodeFile;		// mips���� ����ļ�

int labelId = 0;	// ��ŵ�id
int tmpVarId = 0;	// �м������id

// �ļ������ͳ�ʼ��
void openFiles();
void closeFiles();
void init_CODEN();
void init_CODES();

// �ʷ������׶�
string CODEN[40];			// ���浥��������
string CODES[40];			// ���浥�ʶ�Ӧ�ַ���
extern void getsym();

// �﷨�����׶�
extern void getProgram();

// �м�������ɽ׶�
vector <struct midCode> midCodes;		// �����������м����
extern void outputMidCodes();			// ����м����

// Ŀ��������ɽ׶�
vector <struct mipsCode> mipsCodes;		// ����������mips����
extern void generateMipsCodes();		// ���м��������mips����
extern void outputMipsCodes();			// ���mips����