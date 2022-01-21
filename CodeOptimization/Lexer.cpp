#include "Lexer.h"

int isDigit(char c) {
	if ('0' <= c && c <= '9')
		return 1;
	else
		return 0;
}

int isLetter(char c) {
	if ((c == '_') || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
		return 1;
	else
		return 0;
}

int isChar(char c) {
	if (c == '+' || c == '-' || c == '*' || c == '/' || isDigit(c) || isLetter(c))
		return 1;
	else
		return 0;
}

int isString(char c) {
	if (c == 32 || c == 33 || (35 <= c && c <= 126))
		return 1;
	else
		return 0;
}

void cat(char s[], char c) {
	int len = strlen(s);
	s[len] = c;
	s[len + 1] = '\0';
	return;
}

void getsym() {
	if (isFirst == 1) {
		isFirst = 0;
	}
	else {
		outputsym();
		lastlastSymbol = lastSymbol;
		strcpy_s(lastlastTOKEN, lastTOKEN);
		lastSymbol = symbol;
		strcpy_s(lastTOKEN, TOKEN);
		lastCountLine = cnt_line;
	}

	symbol = -1;
	TOKEN[0] = '\0';
	// 获取一个非空字符，如果获取失败则直接返回
	while (getnbc() == 0) {
		if (!ifile.eof()) {
			cnt = 0;
			cnt_line++;
			ifile.getline(buffer, 256);
			cnt_len = strlen(buffer);
		}
		else {
			symbol = -2;
			return;
		}
	}

	if (isLetter(CHAR)) {
		while (isDigit(CHAR) || isLetter(CHAR)) {
			cat(TOKEN, CHAR);
			getch();
		}
		ungetch();

		char tmpTOKEN[1000];
		strcpy_s(tmpTOKEN, TOKEN);
		tolowercase(tmpTOKEN);
		reserve(tmpTOKEN);
	}
	else if (isDigit(CHAR)) {
		while (isDigit(CHAR)) {
			cat(TOKEN, CHAR);
			getch();
		}
		ungetch();
		symbol = INTCON;
	}
	else if (CHAR == '\'') {
		getch();
		if (isChar(CHAR) == 0) {
			error('a');		// 字符中出现了非法符号
		}
		cat(TOKEN, CHAR);
		getch();
		if (CHAR != '\'') {
			error('a');
			ungetch();
		}
		symbol = CHARCON;
	}
	else if (CHAR == '\"') {
		getch();
		while (CHAR != '\"') {
			if (isString(CHAR) == 0) {
				error('a');		// 字符串中出现了非法符号
			}
			cat(TOKEN, CHAR);
			getch();
		}
		if (strlen(TOKEN) == 0) {
			error('a');			// 字符串中无任何符号
		}
		symbol = STRCON;
	}
	else if (CHAR == '+') symbol = PLUS;
	else if (CHAR == '-') symbol = MINU;
	else if (CHAR == '*') symbol = MULT;
	else if (CHAR == '/') symbol = DIV;
	else if (CHAR == '<') {
		getch();
		if (CHAR != '=') {
			symbol = LSS;
			ungetch();
		}
		else {
			symbol = LEQ;
		}
	}
	else if (CHAR == '>') {
		getch();
		if (CHAR != '='){
			symbol = GRE;
			ungetch();
		}
		else {
			symbol = GEQ;
		}
	}
	else if (CHAR == '=') {
		getch();
		if (CHAR == '=')
			symbol = EQL;
		else {
			symbol = ASSIGN;
			ungetch();
		}
	}
	else if (CHAR == '!') {
		getch();
		if (CHAR == '=')
			symbol = NEQ;
		else {
			error('0');
			ungetch();
		}
	}
	else if (CHAR == ':') symbol = COLON;
	else if (CHAR == ';') symbol = SEMICN;
	else if (CHAR == ',') symbol = COMMA;
	else if (CHAR == '(') symbol = LPARENT;
	else if (CHAR == ')') symbol = RPARENT;
	else if (CHAR == '[') symbol = LBRACK;
	else if (CHAR == ']') symbol = RBRACK;
	else if (CHAR == '{') symbol = LBRACE;
	else if (CHAR == '}') symbol = RBRACE;

	if (symbol < 0) {
		error('0');
	}
}


void getsym(bool isOutput) {
	if (isFirst == 1) {
		isFirst = 0;
	}
	else {
		if (isOutput) {
			outputsym();
		}
		lastlastSymbol = lastSymbol;
		strcpy_s(lastlastTOKEN, lastTOKEN);
		lastSymbol = symbol;
		strcpy_s(lastTOKEN, TOKEN);
		lastCountLine = cnt_line;
	}

	symbol = -1;
	TOKEN[0] = '\0';
	// 获取一个非空字符，如果获取失败则直接返回
	while (getnbc() == 0) {
		if (!ifile.eof()) {
			cnt = 0;
			cnt_line++;
			ifile.getline(buffer, 256);
			cnt_len = strlen(buffer);
		}
		else {
			symbol = -2;
			return;
		}
	}

	if (isLetter(CHAR)) {
		while (isDigit(CHAR) || isLetter(CHAR)) {
			cat(TOKEN, CHAR);
			getch();
		}
		ungetch();

		char tmpTOKEN[1000];
		strcpy_s(tmpTOKEN, TOKEN);
		tolowercase(tmpTOKEN);
		reserve(tmpTOKEN);
	}
	else if (isDigit(CHAR)) {
		while (isDigit(CHAR)) {
			cat(TOKEN, CHAR);
			getch();
		}
		ungetch();
		symbol = INTCON;
	}
	else if (CHAR == '\'') {
		getch();
		if (isChar(CHAR) == 0) {
			error('a');		// 字符中出现了非法符号
		}
		cat(TOKEN, CHAR);
		getch();
		if (CHAR != '\'') {
			error('a');
			ungetch();
		}
		symbol = CHARCON;
	}
	else if (CHAR == '\"') {
		getch();
		while (CHAR != '\"') {
			if (isString(CHAR) == 0) {
				error('a');		// 字符串中出现了非法符号
			}
			cat(TOKEN, CHAR);
			getch();
		}
		if (strlen(TOKEN) == 0) {
			error('a');			// 字符串中无任何符号
		}
		symbol = STRCON;
	}
	else if (CHAR == '+') symbol = PLUS;
	else if (CHAR == '-') symbol = MINU;
	else if (CHAR == '*') symbol = MULT;
	else if (CHAR == '/') symbol = DIV;
	else if (CHAR == '<') {
		getch();
		if (CHAR != '=') {
			symbol = LSS;
			ungetch();
		}
		else {
			symbol = LEQ;
		}
	}
	else if (CHAR == '>') {
		getch();
		if (CHAR != '=') {
			symbol = GRE;
			ungetch();
		}
		else {
			symbol = GEQ;
		}
	}
	else if (CHAR == '=') {
		getch();
		if (CHAR == '=')
			symbol = EQL;
		else {
			symbol = ASSIGN;
			ungetch();
		}
	}
	else if (CHAR == '!') {
		getch();
		if (CHAR == '=')
			symbol = NEQ;
		else {
			error('0');
			ungetch();
		}
	}
	else if (CHAR == ':') symbol = COLON;
	else if (CHAR == ';') symbol = SEMICN;
	else if (CHAR == ',') symbol = COMMA;
	else if (CHAR == '(') symbol = LPARENT;
	else if (CHAR == ')') symbol = RPARENT;
	else if (CHAR == '[') symbol = LBRACK;
	else if (CHAR == ']') symbol = RBRACK;
	else if (CHAR == '{') symbol = LBRACE;
	else if (CHAR == '}') symbol = RBRACE;

	if (symbol < 0) {
		error('0');
	}
}


void trysym() {
	if (isFirst == 1) {
		isFirst = 0;
	}
	else {
		lastlastSymbol = lastSymbol;
		strcpy_s(lastlastTOKEN, lastTOKEN);
		lastSymbol = symbol;
		strcpy_s(lastTOKEN, TOKEN);
	}

	symbol = -1;
	TOKEN[0] = '\0';
	// 获取一个非空字符，如果获取失败则直接返回
	while (getnbc() == 0) {
		if (!ifile.eof()) {
			cnt = 0;
			cnt_line++;
			ifile.getline(buffer, 256);
			cnt_len = strlen(buffer);
		}
		else {
			symbol = -2;
			return;
		}
	}

	if (isLetter(CHAR)) {
		while (isDigit(CHAR) || isLetter(CHAR)) {
			cat(TOKEN, CHAR);
			getch();
		}
		ungetch();

		char tmpTOKEN[1000];
		strcpy_s(tmpTOKEN, TOKEN);
		tolowercase(tmpTOKEN);
		reserve(tmpTOKEN);
	}
	else if (isDigit(CHAR)) {
		while (isDigit(CHAR)) {
			cat(TOKEN, CHAR);
			getch();
		}
		ungetch();
		symbol = INTCON;
	}
	else if (CHAR == '\'') {
		getch();
		if (isChar(CHAR) == 0) {
			error('a');		// 字符中出现了非法符号
		}
		cat(TOKEN, CHAR);
		getch();
		if (CHAR != '\'') {
			error('a');
			ungetch();
		}
		symbol = CHARCON;
	}
	else if (CHAR == '\"') {
		getch();
		while (CHAR != '\"') {
			if (isString(CHAR) == 0) {
				error('a');		// 字符串中出现了非法符号
			}
			cat(TOKEN, CHAR);
			getch();
		}
		if (strlen(TOKEN) == 0) {
			error('a');			// 字符串中无任何符号
		}
		symbol = STRCON;
	}
	else if (CHAR == '+') symbol = PLUS;
	else if (CHAR == '-') symbol = MINU;
	else if (CHAR == '*') symbol = MULT;
	else if (CHAR == '/') symbol = DIV;
	else if (CHAR == '<') {
		getch();
		if (CHAR != '=') {
			symbol = LSS;
			ungetch();
		}
		else {
			symbol = LEQ;
		}
	}
	else if (CHAR == '>') {
		getch();
		if (CHAR != '=') {
			symbol = GRE;
			ungetch();
		}
		else {
			symbol = GEQ;
		}
	}
	else if (CHAR == '=') {
		getch();
		if (CHAR == '=')
			symbol = EQL;
		else {
			symbol = ASSIGN;
			ungetch();
		}
	}
	else if (CHAR == '!') {
		getch();
		if (CHAR == '=')
			symbol = NEQ;
		else {
			error('0');
			ungetch();
		}
	}
	else if (CHAR == ':') symbol = COLON;
	else if (CHAR == ';') symbol = SEMICN;
	else if (CHAR == ',') symbol = COMMA;
	else if (CHAR == '(') symbol = LPARENT;
	else if (CHAR == ')') symbol = RPARENT;
	else if (CHAR == '[') symbol = LBRACK;
	else if (CHAR == ']') symbol = RBRACK;
	else if (CHAR == '{') symbol = LBRACE;
	else if (CHAR == '}') symbol = RBRACE;

	if (symbol < 0) {
		error('0');
	}
}

void ungetsym() {
	cnt = lastBegin;
	symbol = lastSymbol;
	strcpy_s(TOKEN, lastTOKEN);

	lastBegin = lastlastBegin;
	lastSymbol = lastlastSymbol;
	strcpy_s(lastTOKEN, lastlastTOKEN);
}

void outputsym() {
	if (0 <= symbol && symbol <= 18)
		ofile << CODEN[symbol] << " " << TOKEN << endl;
	else if (19 <= symbol && symbol <= 38)
		ofile << CODEN[symbol] << " " << CODES[symbol] << endl;
}

void reserve(char s[]) {
	if (strcmp(s, "const") == 0) symbol = 4;
	else if (strcmp(s, "int") == 0) symbol = 5;
	else if (strcmp(s, "char") == 0) symbol = 6;
	else if (strcmp(s, "void") == 0) symbol = 7;
	else if (strcmp(s, "main") == 0) symbol = 8;
	else if (strcmp(s, "if") == 0) symbol = 9;
	else if (strcmp(s, "else") == 0) symbol = 10;
	else if (strcmp(s, "switch") == 0) symbol = 11;
	else if (strcmp(s, "case") == 0) symbol = 12;
	else if (strcmp(s, "default") == 0) symbol = 13;
	else if (strcmp(s, "while") == 0) symbol = 14;
	else if (strcmp(s, "for") == 0) symbol = 15;
	else if (strcmp(s, "scanf") == 0) symbol = 16;
	else if (strcmp(s, "printf") == 0) symbol = 17;
	else if (strcmp(s, "return") == 0) symbol = 18;
	else symbol = 0;
}

void tolowercase(char s[]) {
	int len = strlen(s);
	for (int i = 0; i < len; i++) {
		if (isLetter(s[i])) {
			s[i] = tolower(s[i]);
		}
	}
}

int getnbc() {
	while ((buffer[cnt] == ' ' || buffer[cnt] == '\t' || buffer[cnt] == '\n' || buffer[cnt] == '\r') && cnt < cnt_len) {
		cnt++;
	}
	if (cnt >= cnt_len) {
		return 0;
	} else {
		lastlastBegin = lastBegin;
		lastBegin = cnt;
		CHAR = buffer[cnt];
		cnt++;
		return 1;
	}
}

void getch() {
	CHAR = buffer[cnt];
	cnt++;
}

void ungetch() {
	cnt--;
}
