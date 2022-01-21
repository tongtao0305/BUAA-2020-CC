#include <map>
#include <string>
#include <algorithm>
using namespace std;

// ��������
extern void error(char errorCode);

void initCurrentSymbol();		// ����ǰ��curSym��ʼ��

void insertCurrentSymbol();

void insertCurrentLevel();

// �����ķ������е������������ַ���ת����Сд
extern int isString(char c);		// �ж��Ƿ�Ϊ�ַ����е��ַ�
extern void tolowercase(char s[]);	// ���ַ����еĴ�дӢ���ַ�ת��ΪСд

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
extern map<string, map<string, struct sym>> symList;
extern struct sym curSym;					// ��¼��ǰ����ʹ�õķ���
extern string curLevel;						// ��¼��ǰ���ڵķ��ű�������
extern map<string, struct sym> curMap;		// ��¼��ǰ���ڵķ��ű��ε�map

void initCurrentSymbol() {
	curSym.name = "";
	curSym.kind = -1;
	curSym.type = -1;
	curSym.dimension = 0;

	curSym.num = -1;
	curSym.str = "";
	curSym.index = -1;
	curSym.columns = -1;
}

// �ú�������ǰ�Ĳ�β�����ű�
void insertCurrentLevel() {
	curMap.clear();
	symList[curLevel] = curMap;
}

// �ú�������ǰ�ķ��Ų��뵱ǰ�Ĳ��
void insertCurrentSymbol() {
	//transform(curSym.name.begin(), curSym.name.end(), curSym.name.begin(), ::tolower);
	if (symList[curLevel].find(curSym.name) == symList[curLevel].end()) {
		symList[curLevel][curSym.name] = curSym;
	}
	else {
		error('b');
	}
}

