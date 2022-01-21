#include "MipsCode.h"

void generateMipsCodes() {
	// .data
	insertMipsCode(dataSeg, "", "", "", 0);

	// .asciiz
	for (int i = 0; i < stringList.size(); i++) {
		insertMipsCode(asciizSeg, "s_" + int2string(i), stringList[i], "");
	}
	insertMipsCode(asciizSeg, "nextLine", "\\n", "");

	// .text
	insertMipsCode(textSeg, "", "", "");

	// j main
	insertMipsCode(j, "main", "", "");

	// 定义相关变量
	bool isFirstFunction = true;		// 标记是否为第一个函数
	bool flag = false;
	int len = 0, addr = 0, va = 0, va2 = 0;
	bool get1 = false, get2 = false;
	int pushCnt = 0;
	int paramSize = 0;
	stack<struct midCode> pushOpStack;


	for (int i = 0; i < midCodes.size(); i++) {
		midCode mc = midCodes[i];
		midCode mcNext = mc;
		switch (mc.op) {
		case PLUSOP: {
			get1 = false;
			loadValue(mc.left, "$t0", false, va, get1);
			get2 = false;
			loadValue(mc.right, "$t1", false, va2, get2);
			if (get1 && get2) {
				insertMipsCode(li, "$t2", "", "", va + va2);
			}
			else if (get1 && !get2) {
				insertMipsCode(addi, "$t2", "$t1", "", va);
			}
			else if (!get1 && get2) {
				insertMipsCode(addi, "$t2", "$t0", "", va2);
			}
			else {
				insertMipsCode(add, "$t2", "$t0", "$t1");
			}
			storeValue(mc.result, "$t2");
			break;
		}
		case MINUOP: {
			get1 = false;
			loadValue(mc.left, "$t0", false, va, get1);
			get2 = false;
			loadValue(mc.right, "$t1", false, va2, get2);
			if (get1 && get2) {
				insertMipsCode(li, "$t2", "", "", va - va2);
			}
			else if (get1 && !get2) {  //va - $t1
				insertMipsCode(addi, "$t2", "$t1", "", -va);	// $t1-va
				insertMipsCode(sub, "$t2", "$0", "$t2");		//0-$t2    
			}
			else if (!get1 && get2) {  //$t0 - va2
				insertMipsCode(addi, "$t2", "$t0", "", -va2);
			}
			else {
				insertMipsCode(sub, "$t2", "$t0", "$t1");
			}
			storeValue(mc.result, "$t2");
			break;
		}
		case MULTOP: {
			loadValue(mc.left, "$t0", true, va, get1);
			loadValue(mc.right, "$t1", true, va2, get2);
			insertMipsCode(mult, "$t0", "$t1", "");
			insertMipsCode(mflo, "$t2", "", "");
			storeValue(mc.result, "$t2");
			break;
		}
		case DIVOP: {
			loadValue(mc.left, "$t0", true, va, get1);
			loadValue(mc.right, "$t1", true, va2, get2);
			insertMipsCode(divop, "$t0", "$t1", "");
			insertMipsCode(mflo, "$t2", "", "");
			storeValue(mc.result, "$t2");
			break;
		}
		case ASSIGNOP: {
			loadValue(mc.left, "$t0", true, va, get1);
			storeValue(mc.result, "$t0");
			break;
		}
		case SCAN: {
			string symbolName = mc.result;		// symbolName即mc.result，是局部变量或全局变量
			if (symList[curFuncName].find(symbolName) != symList[curFuncName].end()
				&& (symList[curFuncName][symbolName].kind == 1 || symList[curFuncName][symbolName].kind == 2)) {

				insertMipsCode(li, "$v0", "", "", symList[curFuncName][symbolName].type == 1 ? 5 : 12);		// int类型变量
				insertMipsCode(syscall, "", "", "");
				addr = symList[curFuncName][symbolName].addr;
				insertMipsCode(sw, "$v0", "$fp", "", -4 * addr);
			}
			else if (symList[PROGRAM].find(symbolName) != symList[PROGRAM].end()
				&& (symList[PROGRAM][symbolName].kind == 1 || symList[PROGRAM][symbolName].kind == 2)) {

				insertMipsCode(li, "$v0", "", "", symList[PROGRAM][symbolName].type == 1 ? 5 : 12);
				insertMipsCode(syscall, "", "", "");
				addr = symList[PROGRAM][symbolName].addr;
				insertMipsCode(sw, "$v0", "$gp", "", 4 * addr);
			}
			break;
		}
		case PRINT:
			if (mc.left[0] == '1') {
				// int
				loadValue(mc.result, "$a0", true, va, get1);
				insertMipsCode(li, "$v0", "", "", 1);
				insertMipsCode(syscall, "", "", "", 0);
			}
			else if (mc.left[0] == '2') {
				// char
				loadValue(mc.result, "$a0", true, va, get1);
				insertMipsCode(li, "$v0", "", "", 11);
				insertMipsCode(syscall, "", "", "", 0);
			}
			else if (mc.left[0] == '3') {
				// string
				// 从字符串表中查找对应的字符串下标
				for (int i = 0; i < stringList.size(); i++) {
					if (stringList[i] == mc.result) {
						insertMipsCode(la, "$a0", "s_" + int2string(i), "", 0);
						break;
					}
				}
				insertMipsCode(li, "$v0", "", "", 4);
				insertMipsCode(syscall, "", "", "", 0);
			}
			else if (mc.left[0] == '4') {
				// 换行
				insertMipsCode(la, "$a0", "nextLine", "", 0);
				insertMipsCode(li, "$v0", "", "", 4);
				insertMipsCode(syscall, "", "", "", 0);
			}
			break;
		case LABEL: {
			insertMipsCode(label, mc.result, "", "", 0);
			break;
		}
		case FUNC: {
			//进入函数 首先产生标号 此时的$sp就是当前函数的栈顶
			//需要为前一个函数做jr $ra 注意第一个函数不用做
			if (!isFirstFunction)
				insertMipsCode(jr, "$ra", "", "", 0);
			else
				isFirstFunction = false;

			insertMipsCode(label, mc.left, "", "", 0);

			if (mc.left == MAIN) {
				int len = symList[MAIN].size();
				insertMipsCode(moveop, "$fp", "$sp", "", 0);
				insertMipsCode(addi, "$sp", "$sp", "", -4 * len - 8);
			}

			curFuncName = mc.left;  //记录当前的函数名字

			break;
		}
		default:
			break;
		}

	}

	insertMipsCode(li, "$v0", "", "", 10);
	insertMipsCode(syscall, "", "", "");
}

void loadValue(string symbolName, string regName, bool gene, int& va, bool& get) {
	// 常量从符号表中直接读取，变量从相应的内存地址中读取，立即数直接使用string2int
	if (symList[curFuncName].find(symbolName) != symList[curFuncName].end()) {
		// 从符号表中获取 symbol
		struct sym tmpSymbol = symList[curFuncName][symbolName];

		if (tmpSymbol.kind == 0) {
			// 常量 const
			va = tmpSymbol.type == 1 ? tmpSymbol.num : tmpSymbol.str[0];
			if (gene) {
				insertMipsCode(li, regName, "", "", va);
			}
			get = true;
		}
		else {
			// 变量 var
			int addr = symList[curFuncName][symbolName].addr;
			insertMipsCode(lw, regName, "$fp", "", -4 * addr);
		}
	}
	else if (symList[PROGRAM].find(symbolName) != symList[PROGRAM].end()) {
		// 从符号表中获取 symbol
		struct sym tmpSymbol = symList[PROGRAM][symbolName];

		if (tmpSymbol.kind == 0) {
			va = tmpSymbol.type == 1 ? tmpSymbol.num : tmpSymbol.str[0];
			if (gene) {
				insertMipsCode(li, regName, "", "", va);
			}
			get = true;
		}
		else {  
			int addr = symList[PROGRAM][symbolName].addr;
			insertMipsCode(lw, regName, "$gp", "", 4 * addr);
		}
	}
	else { 
		// 立即数，且不能为空串
		if (symbolName.size() > 0) {
			va = string2int(symbolName);
			get = true;
			if (gene) {
				insertMipsCode(li, regName, "", "", va);
			}
		}
	}
}

void storeValue(string symbolName, string regName) {
	int addr;
	if (symList[curFuncName].find(symbolName) != symList[curFuncName].end()
		&& (symList[curFuncName][symbolName].kind == 1  || symList[curFuncName][symbolName].kind == 2)) {
		// 局部变量保存到$fp
		addr = symList[curFuncName][symbolName].addr;
		insertMipsCode(sw, regName, "$fp", "", -4 * addr);
	}
	else if (symList[PROGRAM].find(symbolName) != symList[PROGRAM].end()
		&& (symList[PROGRAM][symbolName].kind == 1 || symList[PROGRAM][symbolName].kind == 2)) {
		// 全局变量保存到$gp
		addr = symList[PROGRAM][symbolName].addr;
		insertMipsCode(sw, regName, "$gp", "", 4 * addr);
	}
}

void insertMipsCode(mipsOp op, string result, string left, string right, int imm) {
	struct mipsCode mipsCode = { op, result, left, right, imm };
	mipsCodes.push_back(mipsCode);
}

void insertMipsCode(mipsOp op, string result, string left, string right) {
	struct mipsCode mipsCode = { op, result, left, right };
	mipsCodes.push_back(mipsCode);
}

void outputMipsCodes() {
	for (auto mipsCode : mipsCodes)
		outputMipsCode(mipsCode);
}

void outputMipsCode(struct mipsCode mc) {
	switch (mc.op) {
	case add:
		mipsCodeFile << "add " << mc.result << "," << mc.left << "," << mc.right << "\n";
		break;
	case sub:
		mipsCodeFile << "sub " << mc.result << "," << mc.left << "," << mc.right << "\n";
		break;
	case mult:
		mipsCodeFile << "mult " << mc.result << "," << mc.left << "\n";
		break;
	case divop:
		mipsCodeFile << "div " << mc.result << "," << mc.left << "\n";
		break;
	case addi:
		mipsCodeFile << "addi " << mc.result << "," << mc.left << "," << mc.imm << "\n";
		break;
	case sll:
		mipsCodeFile << "sll " << mc.result << "," << mc.left << "," << mc.imm << "\n";
		break;
	case mflo:
		mipsCodeFile << "mflo " << mc.result << "\n";
		break;
	case mfhi:
		mipsCodeFile << "mfhi " << mc.result << "\n";
	case beq:
		mipsCodeFile << "beq " << mc.result << "," << mc.left << "," << mc.right << "\n";
		break;
	case bne:
		mipsCodeFile << "bne " << mc.result << "," << mc.left << "," << mc.right << "\n";
		break;
	case bgt:
		mipsCodeFile << "bgt " << mc.result << "," << mc.left << "," << mc.right << "\n";
		break;
	case bge:
		mipsCodeFile << "bge " << mc.result << "," << mc.left << "," << mc.right << "\n";
		break;
	case blt:
		mipsCodeFile << "blt " << mc.result << "," << mc.left << "," << mc.right << "\n";
		break;
	case ble:
		mipsCodeFile << "ble " << mc.result << "," << mc.left << "," << mc.right << "\n";
		break;
	case j:
		mipsCodeFile << "j " << mc.result << "\n";
		break;
	case jal:
		mipsCodeFile << "jal " << mc.result << "\n";
		break;
	case jr:
		mipsCodeFile << "jr " << mc.result << "\n";
		break;
	case lw:
		mipsCodeFile << "lw " << mc.result << "," << mc.imm << "(" << mc.left << ")\n";
		break;
	case sw:
		mipsCodeFile << "sw " << mc.result << "," << mc.imm << "(" << mc.left << ")\n";
		break;
	case syscall:
		mipsCodeFile << "syscall\n";
		break;
	case li:
		mipsCodeFile << "li " << mc.result << "," << mc.imm << "\n";
		break;
	case la:
		mipsCodeFile << "la " << mc.result << "," << mc.left << "\n";
		break;
	case moveop:
		mipsCodeFile << "move " << mc.result << "," << mc.left << "\n";
		break;
	case dataSeg:
		mipsCodeFile << ".data\n";
		break;
	case textSeg:
		mipsCodeFile << ".text\n";
		break;
	case asciizSeg:
		mipsCodeFile << mc.result << ": .asciiz \"" << mc.left << "\"\n";
		break;
	case globlSeg:
		mipsCodeFile << ".globl main\n";
		break;
	case label:
		mipsCodeFile << mc.result << ":\n";
		break;
	default:
		break;

	}
}
