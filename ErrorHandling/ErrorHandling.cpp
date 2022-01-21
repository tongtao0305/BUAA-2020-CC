#include <fstream>
#include <iostream>
using namespace std;

extern ofstream errorFile;
extern int cnt_line;
extern int isFirstInLine;
extern int lastCountLine;

void error(char errorCode) {
	int tmpCountLine;
	
	// 根据错误类型判断错误所在行数
	if (errorCode == 'k' || errorCode == 'l' || errorCode == 'm') 
		tmpCountLine = lastCountLine;
	else 
		tmpCountLine = cnt_line;
	
	if (errorCode == '0') {
		// cout << "Other Error, at line " << cnt_line << endl;
	}
	else {
		// cout << cnt_line << " " << errorCode << endl;			// 调试输出
		errorFile << tmpCountLine << " " << errorCode << endl;		// 文件输出
	}
	return;
}
