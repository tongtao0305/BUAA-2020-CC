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
	// insertMipsCode(j, "main", "", "");

	// 定义相关变量
	bool isFirstFunction = true;		// 标记是否为第一个函数
	bool flag = false;
	int len = 0, addr = 0, va = 0, va2 = 0, column = 0;
	bool get1 = false, get2 = false;
	int pushCnt = 0;
	int paramSize = 0;
	stack<struct midCode> pushOpStack;

	string switchEndLabel;

	for (int i = 0; i < midCodes.size(); i++) {
		midCode mc = midCodes[i], mcNext;
		if (i < midCodes.size() - 1) {
			mcNext = midCodes[i + 1];
		}
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


		case LSSOP: {  
			// <
			get1 = false;
			loadValue(mc.left, "$t0", false, va, get1);
			get2 = false;
			loadValue(mc.right, "$t1", false, va2, get2);
			if (mcNext.op == BZ) {  //0跳  x>=y跳
				if (get1 && get2) {
					if (va >= va2) {
						insertMipsCode(j, mcNext.result, "", "");
					}
				}
				else if (get1 && !get2) {  //va >= $t1
					insertMipsCode(ble, "$t1", int2string(va), mcNext.result);//ble <=跳转 代价4
				}
				else if (!get1 && get2) {  //$t0 >= va2
					insertMipsCode(bge, "$t0", int2string(va2), mcNext.result);//bge >=跳转 代价3
				}
				else {  //$t0 >= $t1
					insertMipsCode(bge, "$t0", "$t1", mcNext.result);//bge >=跳转 代价3
				}
			}
			else if (mcNext.op == BNZ) {  //1跳 x<y跳
				if (get1 && get2) {
					if (va < va2) {
						insertMipsCode(j, mcNext.result, "", "");
					}
				}
				else if (get1 && !get2) {  //va < $t1
					insertMipsCode(bgt, "$t1", int2string(va), mcNext.result);//bgt >跳转 代价4
				}
				else if (!get1 && get2) {  //$t0 < va2
					insertMipsCode(blt, "$t0", int2string(va2), mcNext.result); //blt <跳转 代价3
				}
				else { //$t0 < $t1
					insertMipsCode(blt, "$t0", "$t1", mcNext.result); //blt <跳转 代价3
				}
			}
			i++;
			break;
		}
		case LEQOP: {  //<=
			get1 = false;
			loadValue(mc.left, "$t0", false, va, get1);
			get2 = false;
			loadValue(mc.right, "$t1", false, va2, get2);
			if (mcNext.op == BZ) {  //0跳  x>y跳
				if (get1 && get2) {
					if (va > va2) {
						insertMipsCode(j, mcNext.result, "", "");
					}
				}
				else if (get1 && !get2) {  //va > $t1
					insertMipsCode(blt, "$t1", int2string(va), mcNext.result);//blt <跳转 代价3
				}
				else if (!get1 && get2) {  //$t0 > va2
					insertMipsCode(bgt, "$t0", int2string(va2), mcNext.result);//bgt >跳转 代价4
				}
				else {  //$t0 > $t1
					insertMipsCode(bgt, "$t0", "$t1", mcNext.result);//bgt >跳转 代价3
				}
			}
			else if (mcNext.op == BNZ) {  //1跳 x<=y跳
				if (get1 && get2) {
					if (va <= va2) {
						insertMipsCode(j, mcNext.result, "", "");
					}
				}
				else if (get1 && !get2) {  //va <= $t1
					insertMipsCode(bge, "$t1", int2string(va), mcNext.result);//bge >=跳转 代价3
				}
				else if (!get1 && get2) {  //$t0 <= va2
					insertMipsCode(ble, "$t0", int2string(va2), mcNext.result);//ble <=跳转 代价4
				}
				else { //$t0 <= $t1
					insertMipsCode(ble, "$t0", "$t1", mcNext.result);//ble <=跳转 代价3
				}
			}
			i++;
			break;
		}
		case GREOP: {  //>
			get1 = false;
			loadValue(mc.left, "$t0", false, va, get1);
			get2 = false;
			loadValue(mc.right, "$t1", false, va2, get2);
			if (mcNext.op == BZ) {  //0跳  x<=y跳
				if (get1 && get2) {
					if (va <= va2) {
						insertMipsCode(j, mcNext.result, "", "");
					}
				}
				else if (get1 && !get2) {  //va <= $t1
					insertMipsCode(bge, "$t1", int2string(va), mcNext.result);//bge >=跳转 代价3
				}
				else if (!get1 && get2) {  //$t0 <= va2
					insertMipsCode(ble, "$t0", int2string(va2), mcNext.result);//ble <=跳转 代价4
				}
				else { //$t0 <= $t1
					insertMipsCode(ble, "$t0", "$t1", mcNext.result);//ble <=跳转 代价3
				}
			}
			else if (mcNext.op == BNZ) {  //1跳 x>y跳
				if (get1 && get2) {
					if (va > va2) {
						insertMipsCode(j, mcNext.result, "", "");
					}
				}
				else if (get1 && !get2) {  //va > $t1
					insertMipsCode(blt, "$t1", int2string(va), mcNext.result);//blt <跳转 代价3
				}
				else if (!get1 && get2) {  //$t0 > va2
					insertMipsCode(bgt, "$t0", int2string(va2), mcNext.result);//bgt >跳转 代价4
				}
				else {  //$t0 > $t1
					insertMipsCode(bgt, "$t0", "$t1", mcNext.result);//bgt >跳转 代价3
				}
			}
			i++;
			break;
		}
		case GEQOP: {  //>=
			get1 = false;
			loadValue(mc.left, "$t0", false, va, get1);
			get2 = false;
			loadValue(mc.right, "$t1", false, va2, get2);
			if (mcNext.op == BZ) {  //0跳  x<y跳
				if (get1 && get2) {
					if (va < va2) {
						insertMipsCode(j, mcNext.result, "", "");
					}
				}
				else if (get1 && !get2) {  //va < $t1
					insertMipsCode(bgt, "$t1", int2string(va), mcNext.result);//bgt >跳转 代价4
				}
				else if (!get1 && get2) {  //$t0 < va2
					insertMipsCode(blt, "$t0", int2string(va2), mcNext.result);//blt <跳转 代价3
				}
				else { //$t0 < $t1
					insertMipsCode(blt, "$t0", "$t1", mcNext.result);//blt <跳转 代价3
				}
			}
			else if (mcNext.op == BNZ) {  //1跳 x>=y跳
				if (get1 && get2) {
					if (va >= va2) {
						insertMipsCode(j, mcNext.result, "", "");
					}
				}
				else if (get1 && !get2) {  //va >= $t1
					insertMipsCode(ble, "$t1", int2string(va), mcNext.result);//ble <=跳转 代价4
				}
				else if (!get1 && get2) {  //$t0 >= va2
					insertMipsCode(bge, "$t0", int2string(va2), mcNext.result);//bge >=跳转 代价3
				}
				else {  //$t0 >= $t1
					insertMipsCode(bge, "$t0", "$t1", mcNext.result);//bge >=跳转 代价3
				}
			}
			i++;
			break;
		}
		case EQLOP: {
			get1 = false;
			loadValue(mc.left, "$t0", false, va, get1);
			get2 = false;
			loadValue(mc.right, "$t1", false, va2, get2);
			if (mcNext.op == BZ) {  //0跳
				if (get1 && get2) {
					if (va != va2) {
						insertMipsCode(j, mcNext.result, "", "");
					}
				}
				else if (get1 && !get2) {
					insertMipsCode(li, "$t0", "", "", va);
					insertMipsCode(bne, "$t0", "$t1", mcNext.result);
				}
				else if (!get1 && get2) {
					insertMipsCode(li, "$t1", "", "", va2);
					insertMipsCode(bne, "$t0", "$t1", mcNext.result);
				}
				else {
					insertMipsCode(bne, "$t0", "$t1", mcNext.result);
				}
			}
			else if (mcNext.op == BNZ) {  //1跳
				if (get1 && get2) {
					if (va == va2) {
						insertMipsCode(j, mcNext.result, "", "");
					}
				}
				else if (get1 && !get2) {
					insertMipsCode(li, "$t0", "", "", va);
					insertMipsCode(beq, "$t0", "$t1", mcNext.result);
				}
				else if (!get1 && get2) {
					insertMipsCode(li, "$t1", "", "", va2);
					insertMipsCode(beq, "$t0", "$t1", mcNext.result);
				}
				else {
					insertMipsCode(beq, "$t0", "$t1", mcNext.result);
				}
			}
			i++;
			break;
		}
		case NEQOP: {
			get1 = false;
			loadValue(mc.left, "$t0", false, va, get1);
			get2 = false;
			loadValue(mc.right, "$t1", false, va2, get2);
			if (mcNext.op == BZ) {  //0跳
				if (get1 && get2) {
					if (va == va2) {
						insertMipsCode(j, mcNext.result, "", "");
					}
				}
				else if (get1 && !get2) {
					insertMipsCode(li, "$t0", "", "", va);
					insertMipsCode(beq, "$t0", "$t1", mcNext.result);
				}
				else if (!get1 && get2) {
					insertMipsCode(li, "$t1", "", "", va2);
					insertMipsCode(beq, "$t0", "$t1", mcNext.result);
				}
				else {
					insertMipsCode(beq, "$t0", "$t1", mcNext.result);
				}
			}
			else if (mcNext.op == BNZ) {  //1跳
				if (get1 && get2) {
					if (va != va2) {
						insertMipsCode(j, mcNext.result, "", "");
					}
				}
				else if (get1 && !get2) {
					insertMipsCode(li, "$t0", "", "", va);
					insertMipsCode(bne, "$t0", "$t1", mcNext.result);
				}
				else if (!get1 && get2) {
					insertMipsCode(li, "$t1", "", "", va2);
					insertMipsCode(bne, "$t0", "$t1", mcNext.result);
				}
				else {
					insertMipsCode(bne, "$t0", "$t1", mcNext.result);
				}
			}
			i++;
			break;
		}


		case ASSIGNOP: {
			loadValue(mc.left, "$t0", true, va, get1);
			storeValue(mc.result, "$t0");
			break;
		}


		case GOTO: {
			insertMipsCode(j,mc.result,"","");
			break;
		}
		case PUSH: {
			pushOpStack.push(mc);
			break;
		}
		case CALL: {
			paramSize = functionParameterCount[mc.result];
			while (paramSize) {
				paramSize--;
				if (pushOpStack.empty()) {
					cout << "ERROR!!!!!!!!\n";
				}
				midCode tmpMc = pushOpStack.top();
				pushOpStack.pop();
				loadValue(tmpMc.result, "$t0", true, va, get1);
				insertMipsCode(sw, "$t0", "$sp", "", -4 * paramSize);
			}
			insertMipsCode(addi, "$sp", "$sp", "", -4 * symList[PROGRAM][mc.result].length - 8);
			insertMipsCode(sw, "$ra", "$sp", "", 4);
			insertMipsCode(sw, "$fp", "$sp", "", 8);
			insertMipsCode(addi, "$fp", "$sp", "", 4 * symList[PROGRAM][mc.result].length + 8);
			insertMipsCode(jal, mc.result, "", "");
			insertMipsCode(lw, "$fp", "$sp", "", 8);
			insertMipsCode(lw, "$ra", "$sp", "", 4);
			insertMipsCode(addi, "$sp", "$sp", "", 4 * symList[PROGRAM][mc.result].length + 8);
			break;
		}
		case RET: {
			loadValue(mc.result, "$v0", true, va, get1);
			insertMipsCode(jr, "$ra", "", "");
			break;
		}
		case RETVALUE: {
			//mc.z 是产生的一个临时变量 需要把$v0的值赋给他
			if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
				&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2)) {
				addr = symList[curFuncName][mc.result].addr;
				insertMipsCode(sw, "$v0", "$fp", "", -4 * addr);
			}
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
		case PRINT: {
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
		}
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
				len = symList[PROGRAM][MAIN].length;
				insertMipsCode(moveop, "$fp", "$sp", "", 0);
				insertMipsCode(addi, "$sp", "$sp", "", -4 * len - 8);
			}

			curFuncName = mc.left;  //记录当前的函数名字

			break;
		}


		case GETARRAY: {
			//midCodefile << mc.result << " = " << mc.left << "[" << mc.right << "]\n";
			//mc.z是局部的变量 或 全局的变量
			//mc.x是数组名
			//mc.y可能是标识符也可能是数值 $t0--->数组的索引
			get1 = false;
			loadValue(mc.right, "$t0", false, va, get1);
			if (symList[curFuncName].find(mc.left) != symList[curFuncName].end()
				&& symList[curFuncName][mc.left].type % 3 != 1) {
				addr = symList[curFuncName][mc.left].addr;
				if (!get1) {
					insertMipsCode(addi, "$t2", "$fp", "", -4 * addr);
					insertMipsCode(sll, "$t0", "$t0", "", 2);
					insertMipsCode(sub, "$t2", "$t2", "$t0");
					insertMipsCode(lw, "$t1", "$t2", "", 0);
				}
				else {
					insertMipsCode(lw, "$t1", "$fp", "", -4 * (addr + va));
				}
			}
			else if (symList[PROGRAM].find(mc.left) != symList[PROGRAM].end()
				&& symList[PROGRAM][mc.left].type % 3 != 1) {
				addr = symList[PROGRAM][mc.left].addr;
				if (!get1) {
					insertMipsCode(addi, "$t2", "$gp", "", 4 * addr);
					insertMipsCode(sll, "$t0", "$t0", "", 2);
					insertMipsCode(add, "$t2", "$t2", "$t0");
					insertMipsCode(lw, "$t1", "$t2", "", 0);
				}
				else {
					insertMipsCode(lw, "$t1", "$gp", "", 4 * (addr + va));
				}
			}
			storeValue(mc.result, "$t1");
			break;
		}
		case PUTARRAY: {
			//midCodefile << mc.z << "[" << mc.x << "]" << " = " << mc.y << "\n";
			//mc.x可能是标识符也可能是数值 数组下标 $t0
			//mc.z是数组名
			//mc.y可能是标识符也可能是数值 $t1
			loadValue(mc.right, "$t1", true, va, get1);
			get1 = false;
			loadValue(mc.left, "$t0", false, va, get1);
			if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
				&& symList[curFuncName][mc.result].type % 3 != 1) {
				addr = symList[curFuncName][mc.result].addr;
				if (!get1) {
					insertMipsCode(addi, "$t2", "$fp", "", -4 * addr);
					insertMipsCode(sll, "$t0", "$t0", "", 2);
					insertMipsCode(sub, "$t2", "$t2", "$t0");
					insertMipsCode(sw, "$t1", "$t2", "", 0);
				}
				else { //拿到了数组下标 存在了va中
					insertMipsCode(sw, "$t1", "$fp", "", -4 * (addr + va));
				}
			}
			else if (symList[PROGRAM].find(mc.result) != symList[PROGRAM].end()
				&& symList[PROGRAM][mc.result].type % 3 != 1) {  //array
				addr = symList[PROGRAM][mc.result].addr;
				if (!get1) {
					insertMipsCode(addi, "$t2", "$gp", "", 4 * addr);
					insertMipsCode(sll, "$t0", "$t0", "", 2);
					insertMipsCode(add, "$t2", "$t2", "$t0");
					insertMipsCode(sw, "$t1", "$t2", "", 0);
				}
				else {
					insertMipsCode(sw, "$t1", "$gp", "", 4 * (addr + va));
				}
			}
			break;
		}
		case EXIT: {
			insertMipsCode(li, "$v0", "", "", 10);
			insertMipsCode(syscall, "", "", "");
			break;
		}


		case GETARRAY2: {
			//mc.result << " = " << mc.left << "[" << mc.right << "]" << "[" << mc.backup << "]" << endl;
			get1 = false;
			loadValue(mc.right, "$t0", true, va, get1);
			get2 = false;
			loadValue(mc.backup, "$t1", true, va2, get2);

			if (symList[curFuncName].find(mc.left) != symList[curFuncName].end()
				&& symList[curFuncName][mc.left].type % 3 != 1) {
				
				addr = symList[curFuncName][mc.left].addr;			// 该数组的起始点地址
				column = symList[curFuncName][mc.left].columns;		// 该数组的每一行元素数量

				// $t0 -> index, $t1 -> column
				// $t0 * column($t3) + $t1
				insertMipsCode(addi, "$t2", "$fp", "", -4 * addr);	// 获取该数组所在地址

				// tocheck
				insertMipsCode(li, "$t3", "", "", column);
				insertMipsCode(mult, "$t0", "$t3", "");				// index * column
				insertMipsCode(mflo, "$t0", "", "");
				insertMipsCode(add, "$t0", "$t0", "$t1");			// + $t1
				insertMipsCode(sll, "$t0", "$t0", "", 2);

				insertMipsCode(sub, "$t2", "$t2", "$t0");			// 获取该元素所在地址
				insertMipsCode(lw, "$t1", "$t2", "", 0);

			}
			else if (symList[PROGRAM].find(mc.left) != symList[PROGRAM].end()
				&& symList[PROGRAM][mc.left].type % 3 != 1) {
				addr = symList[PROGRAM][mc.left].addr;			// 该数组的起始点地址
				column = symList[PROGRAM][mc.left].columns;		// 该数组的每一行元素数量
				
				// $t0 -> index, $t1 -> column
				// $t0 * column($t3) + $t1
				insertMipsCode(addi, "$t2", "$gp", "", 4 * addr);	// 获取该数组所在地址

				// tocheck
				insertMipsCode(li, "$t3", "", "", column);
				insertMipsCode(mult, "$t0", "$t3", "");				// index * column
				insertMipsCode(mflo, "$t0", "", "");
				insertMipsCode(add, "$t0", "$t0", "$t1");			// + $t1
				insertMipsCode(sll, "$t0", "$t0", "", 2);

				insertMipsCode(add, "$t2", "$t2", "$t0");			// 获取该元素所在地址
				insertMipsCode(lw, "$t1", "$t2", "", 0);
			}
			storeValue(mc.result, "$t1");
			break;
		}
		case PUTARRAY2: {
			// mc.result << "[" << mc.left << "]" << "[" << mc.right << "]" << " = " << mc.backup << endl;

			loadValue(mc.backup, "$t1", true, va, get1);
			get1 = false;
			loadValue(mc.left, "$t0", true, va, get1);
			get2 = false;
			loadValue(mc.right, "$t4", true, va, get2);

			if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
				&& symList[curFuncName][mc.result].type % 3 != 1) {
				addr = symList[curFuncName][mc.result].addr;		// 该数组的起始点地址	
				column = symList[curFuncName][mc.result].columns;	// 该数组的每一行元素数量
				
				// $t0 -> index, $t2 -> column
				// $t0 * column($t3) + $t4
				insertMipsCode(addi, "$t2", "$fp", "", -4 * addr);	// 获取该数组所在地址

				// tocheck
				insertMipsCode(li, "$t3", "", "", column);
				insertMipsCode(mult, "$t0", "$t3", "");				// index * column
				insertMipsCode(mflo, "$t0", "", "");
				insertMipsCode(add, "$t0", "$t0", "$t4");			// + $t1
				insertMipsCode(sll, "$t0", "$t0", "", 2);

				insertMipsCode(sub, "$t2", "$t2", "$t0");			// 获取该元素所在地址
				insertMipsCode(sw, "$t1", "$t2", "", 0);
			}
			else if (symList[PROGRAM].find(mc.result) != symList[PROGRAM].end()
				&& symList[PROGRAM][mc.result].type % 3 != 1) {  //array
				addr = symList[PROGRAM][mc.result].addr;		// 该数组的起始点地址	
				column = symList[PROGRAM][mc.result].columns;	// 该数组的每一行元素数量
				
				
				insertMipsCode(addi, "$t2", "$gp", "", 4 * addr);	// 获取该数组所在地址

				// tocheck
				insertMipsCode(li, "$t3", "", "", column);
				insertMipsCode(mult, "$t0", "$t3", "");				// index * column
				insertMipsCode(mflo, "$t0", "", "");
				insertMipsCode(add, "$t0", "$t0", "$t4");			// + $t1
				insertMipsCode(sll, "$t0", "$t0", "", 2);

				insertMipsCode(add, "$t2", "$t2", "$t0");			// 获取该元素所在地址
				insertMipsCode(sw, "$t1", "$t2", "", 0);
			}
			break;
		}

		case JMAIN: {
			insertMipsCode(j, "main", "", "");
			break;
		}

		default: {
			break;
		}
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

		// 常量或者有初始值变量
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
		&& (symList[curFuncName][symbolName].kind == 1  || symList[curFuncName][symbolName].kind == 2 || symList[curFuncName][symbolName].kind == 5)) {
		// 局部变量保存到$fp
		addr = symList[curFuncName][symbolName].addr;
		insertMipsCode(sw, regName, "$fp", "", -4 * addr);
	}
	else if (symList[PROGRAM].find(symbolName) != symList[PROGRAM].end()
		&& (symList[PROGRAM][symbolName].kind == 1 || symList[PROGRAM][symbolName].kind == 2 || symList[curFuncName][symbolName].kind == 5)) {
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
		mipsCodeFile << "addu " << mc.result << "," << mc.left << "," << mc.right << "\n";
		break;
	case sub:
		mipsCodeFile << "subu " << mc.result << "," << mc.left << "," << mc.right << "\n";
		break;
	case mult:
		mipsCodeFile << "mult " << mc.result << "," << mc.left << "\n";
		break;
	case divop:
		mipsCodeFile << "div " << mc.result << "," << mc.left << "\n";
		break;
	case addi:
		mipsCodeFile << "addiu " << mc.result << "," << mc.left << "," << mc.imm << "\n";
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
