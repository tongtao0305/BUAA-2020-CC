#pragma once

#include "Global.h"
using namespace std;

// ����Ϊ�ⲿ���õı����ͺ���
extern ofstream ofile;			// ����ļ�

// �ʷ������׶�
extern int symbol;				// ��¼��ǰToken������
extern char TOKEN[256];			// ��¼��ǰToken���ַ���
extern int lastSymbol;			// ��¼��һ��Token������
extern char lastTOKEN[256];		// ��¼��һ��Token���ַ���
extern int lastlastSymbol;		// ��¼���ϸ�Token������
extern char lastlastTOKEN[256];	// ��¼���ϸ�Token���ַ���
extern int cnt_line;			// ��¼��ǰ��������Դ�ļ��е��������Ա���д������

extern void getsym();
extern void trysym();
extern void ungetsym();
extern void outputsym();
extern void outputlastsym();
extern void outputlastlastsym();

// ������׶�
extern void error(char errorCode);

// ���ű�
extern string curLevel;										// ��¼��ǰ���ڵķ��ű�������
extern struct sym curSym;									// ��¼��ǰ����ʹ�õķ���
extern map<string, map<string, struct sym>> symList;		// ���ű�

extern void initCurrentSymbol();
extern void insertCurrentSymbol();
extern void insertCurrentLevel();
extern void insertSymbol(string name, int kind, int type);

// �м�������ɽ׶�
extern vector<string> stringList;

extern string genTmp();				// ��ȡ��ʱ��������
extern int string2int(string s);	// �ַ���ת��Ϊ����
extern string int2string(int t);	// ����ת��Ϊ�ַ���
extern void insertMidCode(string result, string left, midOp op, string right);


// ����Ϊ�ڲ�����ı����ͺ���
int dimension = 0;						// �������������ά��
int indexCount = 0;						// �޳�ʼ�����������������
int columnsCount = 0;					// �г�ʼ�����������������

int lastTypeIdentifier;						// ��¼��һ�����ͱ�ʶ����0����������1�����ַ�
string lastIdentifier;						// ��¼��һ����ʶ��
set<string> functionWithReturnValue;		// ��¼�з���ֵ����
set<string> functionWithoutReturnValue;		// ��¼�޷���ֵ����
map<string, int> functionReturnType;		// ��¼��������ֵ���ͣ�0Ϊ�޷���ֵ��1Ϊ���ͣ�2Ϊ�ַ�

// ��������ر���
int lastOperator;			// ������һ�������ķ��ţ�-1��ʾΪ����
int lastInteger;			// ������һ������
int lastUnsignedInteger;	// ������һ���޷�������
string lastChar;			// ������һ���ַ�
int constType;				// ��¼��һ�����������ͣ�0����������1�����ַ�
int lastExpressionType;		// ��¼��һ�����ʽ�����ͣ�1����������2�����ַ�
int lastConstantType;		// ��¼��һ�����������ͣ�0����������1�����ַ�
int lastVariableType;		// ��¼��һ��������������ͣ�0����������1�����ַ�

int hasReturn;			// ��¼��ǰ�����Ƿ��з���ֵ��0Ϊ�޷���ֵ��1Ϊ���ͣ�2Ϊ�ַ�
int hasReturned;		// ��¼��ǰ�����Ƿ��Ѿ����أ�0Ϊ�޷���ֵ��1Ϊ���ͣ�2Ϊ�ַ�
int returnType;			// ��¼��Ҫ�ķ���ֵ���ͣ�0Ϊ�޷���ֵ��1Ϊ���ͣ�2Ϊ�ַ�
int returnedType;		// ��¼�Ѿ�����ֵ�����ͣ�0Ϊ�޷���ֵ��1Ϊ���ͣ�2Ϊ�ַ���3Ϊreturn;

string lastFunctionCall;	// ��¼��һ����������
int parameterCount;			// ��¼���������������
int valueParameterCount;	// ��¼����ֵ�����������
map <string, int> functionParameterCount;			// ��¼�з���ֵ�����Ĳ�������
map <string, int[100]> functionParameterType;		// ��¼�з���ֵ�����Ĳ�������

// �ݹ�����Ӻ���
void getAdditionOperator();			// <�ӷ������>
void getMultiplicationOperator();	// <�˷������>
void getRelationalOperator();		// <��ϵ�����>
void getLetter();					// <��ĸ>
void getDigit();					// <����>
void getChar();						// <�ַ�>
void getString();					//*<�ַ���>

void getProgram();					//*<����>
void getConstantDeclaration();		//*<����˵��>
void getConstantDefination();		//*<��������>
void getUnsignedInteger();			//*<�޷�������>
void getUnsignedInteger(int& unsignedInteger);
void getInteger();					//*<����>
void getInteger(int& integer);

void getIdentifier();				// <��ʶ��>
void getIdentifier(string& identifier);
void getDeclarationHeader();		//*<����ͷ��>
void getConstant();					//*<����>
void getVariableDeclaration();		//*<����˵��>
void getVariableDefination();		//*<��������>
void getVariableDefinationWithoutInitialization();	//*<���������޳�ʼ��>
void getVariableDefinationWithInitialization();		//*<�������弰��ʼ��>
void getTypeIdentifier();			// <���ͱ�ʶ��>
void getTypeIdentifier(int& typeIdentifier);

void getFunctionDefinationWithReturnValue();	//*<�з���ֵ��������>
void getFunctionDefinationWithoutReturnValue();	//*<�޷���ֵ��������>
void getCompoundStatements();					//*<�������>
void getParameterList();						//*<������>
void getMain();									//*<������>

void getExpression();	// ���ʽ
void getExpression(int& type, string& ansTmp);
void getTerm();			// ��
void getTerm(int& type, string& ansTmp);
void getFactor();		// ����
void getFactor(int& type, string& ansTmp);

void getStatement();
void getAssignmentStatement();
void getConditionalStatement();
void getCondition();
void getLoopStatement();
void getStep();
void getSwitch();
void getCaseTable();
void getCase();
void getDefault();
void getFunctionCallStatementWithReturnValue();
void getFunctionCallStatementWithoutReturnValue();
void getValueParameterTable();
void getStatementList();
void getScanf();
void getPrintf();
void getReturn();

// programר������
void getVariableDeclarationForProgram();
void getVariableDefinationForProgram();
void getMainForProgram();
void getFunctionDefinationWithReturnValueForProgram();
void getFunctionDefinationWithoutReturnValueForProgram();

// ���symbol����
void checkSymbol(int symType, char errorType);
void checkSymbolandGetSym(int symType, char errorType);
