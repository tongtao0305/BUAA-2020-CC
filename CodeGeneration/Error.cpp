#include "Error.h"

void error(char errorCode) {
	// 根据错误类型判断错误所在行数
	int tmpCountLine = (errorCode == 'k' || errorCode == 'l' || errorCode == 'm') ? lastCountLine : cnt_line;

	if (errorCode != '0') {
		errorFile << tmpCountLine << " " << errorCode << endl;
	}

	cout << tmpCountLine << " " << errorCode << endl;
	return;
}
