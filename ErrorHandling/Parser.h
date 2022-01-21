#pragma once

enum classnum {
	IDENFR, INTCON, CHARCON, STRCON, CONSTTK, INTTK, CHARTK, VOIDTK, MAINTK, IFTK, ELSETK, SWITCHTK, CASETK, DEFAULTTK, WHILETK, FORTK, SCANFTK, PRINTFTK,
	RETURNTK, PLUS, MINU, MULT, DIV, LSS, LEQ, GRE, GEQ, EQL, NEQ, COLON, ASSIGN, SEMICN, COMMA, LPARENT, RPARENT, LBRACK, RBRACK, LBRACE, RBRACE
};

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
void getInteger();					//*<����>

void getIdentifier();				// <��ʶ��>
void getDeclarationHeader();		//*<����ͷ��>
void getConstant();					//*<����>
void getVariableDeclaration();		//*<����˵��>
void getVariableDefination();		//*<��������>
void getVariableDefinationWithoutInitialization();	//*<���������޳�ʼ��>
void getVariableDefinationWithInitialization();		//*<�������弰��ʼ��>
void getTypeIdentifier();			// <���ͱ�ʶ��>

void getFunctionDefinationWithReturnValue();	//*<�з���ֵ��������>
void getFunctionDefinationWithoutReturnValue();	//*<�޷���ֵ��������>
void getCompoundStatements();					//*<�������>
void getParameterList();						//*<������>
void getMain();									//*<������>

void getExpression();	// ���ʽ
void getTerm();			// ��
void getFactor();		// ����

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