#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include <stack>
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#define MAIN "main"
#define PROGRAM "Program"
using namespace std;

// �ʷ�������ض���
enum classCode {
	IDENFR,
	INTCON,
	CHARCON,
	STRCON,
	CONSTTK, 
	INTTK, 
	CHARTK, 
	VOIDTK, 
	MAINTK, 
	IFTK, 
	ELSETK, 
	SWITCHTK, 
	CASETK, 
	DEFAULTTK, 
	WHILETK, 
	FORTK, 
	SCANFTK, 
	PRINTFTK,
	RETURNTK, 
	PLUS, MINU, 
	MULT, 
	DIV, 
	LSS, 
	LEQ, 
	GRE, 
	GEQ, 
	EQL, 
	NEQ, 
	COLON, 
	ASSIGN, 
	SEMICN, 
	COMMA, 
	LPARENT, 
	RPARENT, 
	LBRACK, 
	RBRACK, 
	LBRACE, 
	RBRACE
};

// ���ű���ض���
struct sym {
	// ����Ϊͨ�ò���
	string name;
	int kind;	// ���ŵ����࣬0������1�г�ʼֵ������2�޳�ʼֵ������3�з���ֵ������4�޷���ֵ������5����������6main����
	int type;	// ���ŵ����ͣ�1int��2int[]��3int[][]��4char��5char[]��6char[][]
	int dimension;	// ��Ϊ�������ͣ���¼��ά�ȣ���Ϊ�������ͣ���¼���������
	int addr;

	// ����Ϊ�������
	int num;	// ��¼��ֵ�ͷ��ŵ�ֵ
	string str;	// ��¼�ַ����ַ����ͷ��ŵ�ֵ
	int index;		// ��Ϊ�������ͣ���¼������
	int columns;	// ��Ϊ�������ͣ���¼������
};

// �м������ض���
enum midOp {
	PLUSOP, //+
	MINUOP, //-
	MULTOP, //*
	DIVOP,  // /
	LSSOP,  //<
	LEQOP,  //<=
	GREOP,  //>
	GEQOP,  //>=
	EQLOP,  //==
	NEQOP,  //!=
	ASSIGNOP,  //=
	GOTO,  //��������ת
	BZ,    //������������ת
	BNZ,   //����������ת
	PUSH,  //��������ʱ��������
	CALL,  //��������
	RET,   //�����������
	RETVALUE, //�з���ֵ�������صĽ��
	SCAN,  //��
	PRINT, //д
	LABEL, //���
	CONST, //����
	ARRAY, //����
	VAR,   //����
	FUNC,  //��������
	PARAM, //��������
	GETARRAY,  //ȡ�����ֵ  t = a[]
	PUTARRAY,  //�����鸳ֵ  a[] = t
	EXIT,  //�˳� main���
};
struct midCode {
	string result;		// ���
	string left;		// �������
	midOp op;			// �����
	string right;		// �Ҳ�����
	set<string> out;
};


// ����������ض���
enum mipsOp {
	add,
	addi,
	sub,
	mult,
	divop,
	mflo,
	mfhi,
	sll,
	beq,
	bne,
	bgt, //��չָ�� �൱��һ��ALU��ָ��+һ��branchָ��
	bge, //��չָ�� �൱��һ��ALU��ָ��+һ��branchָ��
	blt, //��չָ�� �൱��һ��ALU��ָ��+һ��branchָ��
	ble, //��չָ�� �൱��һ��ALU��ָ��+һ��branchָ��
	j,
	jal,
	jr,
	lw,
	sw,
	syscall,
	li,
	la,
	moveop,
	dataSeg,  //.data
	textSeg,  //.text
	asciizSeg,  //.asciiz
	globlSeg,  //.globl
	label,  //�������
};
struct mipsCode {
	mipsOp op;		// ����
	string result;	// ���
	string left;	// �������
	string right;	// �Ҳ�����
	int imm;		// ������
};
