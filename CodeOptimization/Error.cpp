#include "Error.h"

extern int isDebug;

void error(char errorCode) {
	// 根据错误类型判断错误所在行数
	int tmpCountLine = (errorCode == 'k' || errorCode == 'l' || errorCode == 'm') ? lastCountLine : cnt_line;

	if (errorCode != '0') {
		errorFile << tmpCountLine << " " << errorCode << endl;
	}

	if (isDebug == 1) {
		cout << tmpCountLine << " " << errorCode << endl;
	}

	return;
}
