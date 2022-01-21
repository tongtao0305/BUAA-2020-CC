#include "MipsCode.h"

// 查找空闲的临时寄存器
int findEmptyTReg() {  
	for (int i = 3; i <= 9; i++) {
		if (!tRegBusy[i]) {  //找到了空闲寄存器
			return i;
		}
	}
	return -1; //没找到
}

//判断当前中间变量是否被分配了t寄存器
int findNameHaveTReg(string& name) {  
	for (int i = 3; i <= 9; i++) {
		if (tRegBusy[i] && tRegContent[i] == name) {  //被占用的寄存器 存储着name
			return i;
		}
	}
	return -1; //没有被分配寄存器 需要lw取
}

int findEmptySReg() {  //查找空闲的s寄存器
	for (int i = 0; i <= 7; i++) {
		if (!sRegBusy[i]) {  //找到了空闲寄存器
			return i;
		}
	}
	return -1; //没找到
}

int findNameHaveSReg(string& name) {  //判断当前中间变量是否被分配了s寄存器
	for (int i = 0; i <= 7; i++) {
		if (sRegBusy[i] && sRegContent[i] == name) {  //被占用的寄存器 存储着name
			return i;
		}
	}
	return -1; //没有被分配寄存器 需要lw取
}

// 初始值的全局变量
void insertProgramMipsCodes() {
	// 定义相关变量
	bool isFirstFunction = true;		// 标记是否为第一个函数
	bool flag = false;
	int len = 0, addr = 0, va = 0, va2 = 0, column = 0;
	bool get1 = false, get2 = false;
	int pushCnt = 0;
	int paramSize = 0;
	stack<struct midCode> pushOpStack;
	int releaseCnt = 0;
	string switchEndLabel;
	
	for (int i = 0; i < midCodesForProgram.size(); i++) {
		midCode mc = midCodes[i], mcNext;
		if (i < midCodes.size() - 1) {
			mcNext = midCodes[i + 1];
		}
		switch (mc.op) {
		case ASSIGNOP: {
			string sz;
			int find;
			int sfind = -1;
			if (mc.result[0] == '#') {  //mc.result是中间变量 分配t寄存器
				find = findEmptyTReg();
				if (find != -1) {  //有空闲
					tRegBusy[find] = 1;  //打标记
					tRegContent[find] = mc.result;  //find这个寄存器保存了mc.result的值
					sz = "$t" + int2string(find);  //sz修改成$t(find) 直接给他赋值 而不需要move sz, $t(find)
					if (isDebug) {
						cout << sz << " = " << mc.result << "\n";
					}
					//直接把mc.left的值load到寄存器sz中
					loadValue(mc.left, sz, true, va, get1);
					//可能因为mc.left本身被分配了寄存器 没有load 只是返回了sz=mc.left的寄存器名
					if (sz != "$t" + int2string(find)) {
						insertMipsCode(moveop, "$t" + int2string(find), sz, "");
					}
				}
				else {  //没有空闲寄存器 就必须取到一个寄存器中 然后存到内存
					string sx = "$t0";
					loadValue(mc.left, sx, true, va, get1);
					storeValue(mc.result, sx);
				}
			}
			else {
				if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
					&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2 || symList[curFuncName][mc.result].kind == 5)) {  //局部变量
					sfind = findNameHaveSReg(mc.result);
					if (sfind != -1) {  //被分配了寄存器 直接用寄存器的值
						sz = "$s" + int2string(sfind);
						//直接把mc.left的值load到寄存器sz中
						loadValue(mc.left, sz, true, va, get1);
						//可能因为mc.left本身被分配了寄存器 没有load 只是返回了sz=mc.left的寄存器名
						if (sz != "$s" + int2string(sfind)) {
							insertMipsCode(moveop, "$s" + int2string(sfind), sz, "");
						}
					}
					else {  //没有被分配寄存器
						sfind = findEmptySReg();
						if (sfind != -1) {  //有空闲
							sRegBusy[sfind] = 1;  //打标记
							sRegContent[sfind] = mc.result;  //find这个寄存器保存了mc.result的值
							sz = "$s" + int2string(sfind);  //sz修改成$s(sfind) 直接给他赋值 而不需要move sz, $s(sfind)
							if (isDebug) {
								cout << sz << " = " << mc.result << "\n";
							}
							//直接把mc.left的值load到寄存器sz中
							loadValue(mc.left, sz, true, va, get1);
							//可能因为mc.left本身被分配了寄存器 没有load 只是返回了sz=mc.left的寄存器名
							if (sz != "$s" + int2string(sfind)) {
								insertMipsCode(moveop, "$s" + int2string(sfind), sz, "");
							}
						}
						else {  //没有空闲寄存器 就必须取到一个寄存器中 然后存到内存
							string sx = "$t0";
							loadValue(mc.left, sx, true, va, get1);
							storeValue(mc.result, sx);
						}
					}
				}
				else {  //全局变量等
					string sx = "$t0";
					loadValue(mc.left, sx, true, va, get1);
					storeValue(mc.result, sx);
				}
			}
			break;
		}
		case GETARRAY: {
			//midCodefile << mc.result << " = " << mc.left << "[" << mc.right << "]\n";
			//mc.result是局部的变量 或 全局的变量
			//mc.left是数组名
			//mc.right可能是标识符也可能是数值 $t0--->数组的索引
			string sy = "$t0", sz = "$t1";
			get1 = false;
			loadValue(mc.right, sy, false, va, get1);
			int find;
			int sfind = -1;

			if (mc.result[0] == '#') {  //mc.result是中间变量 分配t寄存器
				find = findEmptyTReg();
				if (find != -1) {  //有空闲
					tRegBusy[find] = 1;  //打标记
					tRegContent[find] = mc.result;  //find这个寄存器保存了mc.result的值
					sz = "$t" + int2string(find);  //sz修改成$t(find) 直接给他赋值 而不需要move sz, $t(find)
					if (isDebug) {
						cout << sz << " = " << mc.result << "\n";
					}
				}
			}
			else {
				if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
					&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2 || symList[curFuncName][mc.result].kind == 5)) {  //局部变量
					int sfind = findNameHaveSReg(mc.result);
					if (sfind != -1) {  //被分配了寄存器 直接用寄存器的值
						sz = "$s" + int2string(sfind);
					}
					else {  //没有被分配寄存器
						sfind = findEmptySReg();
						if (sfind != -1) {  //有空闲
							sRegBusy[sfind] = 1;  //打标记
							sRegContent[sfind] = mc.result;  //find这个寄存器保存了mc.result的值
							sz = "$s" + int2string(sfind);  //sz修改成$s(sfind) 直接给他赋值 而不需要move sz, $s(sfind)
							if (isDebug) {
								cout << sz << " = " << mc.result << "\n";
							}
						}
					}
				}
			}

			if (symList[curFuncName].find(mc.left) != symList[curFuncName].end()
				&& symList[curFuncName][mc.left].type % 3 != 1) {
				addr = symList[curFuncName][mc.left].addr;
				if (!get1) {
					insertMipsCode(addi, "$t2", "$fp", "", -4 * addr);
					insertMipsCode(sll, "$t0", sy, "", 2);
					insertMipsCode(sub, "$t2", "$t2", "$t0");
					insertMipsCode(lw, sz, "$t2", "", 0);
				}
				else {
					insertMipsCode(lw, sz, "$fp", "", -4 * (addr + va));
				}
			}
			else if (symList[PROGRAM].find(mc.left) != symList[PROGRAM].end()
				&& symList[PROGRAM][mc.left].type % 3 != 1) {
				addr = symList[PROGRAM][mc.left].addr;
				if (!get1) {
					insertMipsCode(addi, "$t2", "$gp", "", 4 * addr);
					insertMipsCode(sll, "$t0", sy, "", 2);
					insertMipsCode(add, "$t2", "$t2", "$t0");
					insertMipsCode(lw, sz, "$t2", "", 0);
				}
				else {
					insertMipsCode(lw, sz, "$gp", "", 4 * (addr + va));
				}
			}
			if (mc.result[0] == '#') {  //mc.result是中间变量
				if (find == -1) {  //没有空闲寄存器
					storeValue(mc.result, sz);
				} //有空闲寄存器的话 已经直接保存到它里边了
			}
			else {
				if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
					&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2)) {  //局部变量
					if (sfind == -1) {  //没有空闲
						storeValue(mc.result, sz);
					} //有空闲寄存器的话 已经直接保存到它里边了
				}
				else {  //全局变量等
					storeValue(mc.result, sz);
				}
			}
			break;
		}
		case PUTARRAY: {
			//midCodefile << mc.result << "[" << mc.left << "]" << " = " << mc.right << "\n";
			//mc.left可能是标识符也可能是数值 数组下标 $t0
			//mc.result是数组名
			//mc.right可能是标识符也可能是数值 $t1
			string sx = "$t0", sy = "$t1";
			get2 = false;
			loadValue(mc.right, sy, false, va2, get2);
			get1 = false;
			loadValue(mc.left, sx, false, va, get1);
			if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
				&& symList[curFuncName][mc.result].type % 3 != 1) {
				addr = symList[curFuncName][mc.result].addr;
				if (!get1) {
					insertMipsCode(addi, "$t2", "$fp", "", -4 * addr);
					insertMipsCode(sll, "$t0", sx, "", 2);
					insertMipsCode(sub, "$t2", "$t2", "$t0");
					if (get2) {
						if (va2 == 0) {
							insertMipsCode(sw, "$0", "$t2", "", 0);
						}
						else {
							insertMipsCode(li, sy, "", "", va2);
							insertMipsCode(sw, sy, "$t2", "", 0);
						}
					}
					else {
						insertMipsCode(sw, sy, "$t2", "", 0);
					}
				}
				else { //拿到了数组下标 存在了va中
					if (get2) {
						if (va2 == 0) {
							insertMipsCode(sw, "$0", "$fp", "", -4 * (addr + va));
						}
						else {
							insertMipsCode(li, sy, "", "", va2);
							insertMipsCode(sw, sy, "$fp", "", -4 * (addr + va));
						}
					}
					else {
						insertMipsCode(sw, sy, "$fp", "", -4 * (addr + va));
					}
				}
			}
			else if (symList[PROGRAM].find(mc.result) != symList[PROGRAM].end() && symList[PROGRAM][mc.result].type % 3 != 1) {  //array
				addr = symList[PROGRAM][mc.result].addr;
				if (!get1) {
					insertMipsCode(addi, "$t2", "$gp", "", 4 * addr);
					insertMipsCode(sll, "$t0", sx, "", 2);
					insertMipsCode(add, "$t2", "$t2", "$t0");
					if (get2) {
						if (va2 == 0) {
							insertMipsCode(sw, "$0", "$t2", "", 0);
						}
						else {
							insertMipsCode(li, sy, "", "", va2);
							insertMipsCode(sw, sy, "$t2", "", 0);
						}
					}
					else {
						insertMipsCode(sw, sy, "$t2", "", 0);
					}
				}
				else {
					if (get2) {
						if (va2 == 0) {
							insertMipsCode(sw, "$0", "$gp", "", (addr + va) * 4);
						}
						else {
							insertMipsCode(li, sy, "", "", va2);
							insertMipsCode(sw, sy, "$gp", "", (addr + va) * 4);
						}
					}
					else {
						insertMipsCode(sw, sy, "$gp", "", (addr + va) * 4);
					}
				}
			}
			break;
		}
		case GETARRAY2: {
			//mc.result << " = " << mc.left << "[" << mc.right << "]" << "[" << mc.backup << "]" << endl;
			string sx = "$t0", sy = "$t1", sz = "$t2", st = "$t3";
			int find;
			int sfind = -1;

			if (mc.result[0] == '#') {  //mc.result是中间变量 分配t寄存器
				find = findEmptyTReg();
				if (find != -1) {  //有空闲
					tRegBusy[find] = 1;  //打标记
					tRegContent[find] = mc.result;  //find这个寄存器保存了mc.result的值
					st = "$t" + int2string(find);  //sz修改成$t(find) 直接给他赋值 而不需要move sz, $t(find)
					if (isDebug) {
						cout << st << " = " << mc.result << "\n";
					}
				}
			}
			else {
				if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
					&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2 || symList[curFuncName][mc.result].kind == 5)) {  //局部变量
					int sfind = findNameHaveSReg(mc.result);
					if (sfind != -1) {  //被分配了寄存器 直接用寄存器的值
						st = "$s" + int2string(sfind);
					}
					else {  //没有被分配寄存器
						sfind = findEmptySReg();
						if (sfind != -1) {  //有空闲
							sRegBusy[sfind] = 1;  //打标记
							sRegContent[sfind] = mc.result;  //find这个寄存器保存了mc.result的值
							st = "$s" + int2string(sfind);  //sz修改成$s(sfind) 直接给他赋值 而不需要move sz, $s(sfind)
							if (isDebug) {
								cout << st << " = " << mc.result << "\n";
							}
						}
					}
				}
			}


			if (symList[curFuncName].find(mc.left) != symList[curFuncName].end() && symList[curFuncName][mc.left].type % 3 != 1) {
				addr = symList[curFuncName][mc.left].addr;			// 该数组的起始点地址
				column = symList[curFuncName][mc.left].columns;		// 该数组的每一行元素数量

				get1 = false;
				loadValue(mc.right, sy, true, va, get1);			// $t1 -> mc.right 第一个下标数字

				insertMipsCode(addi, "$t0", "$fp", "", -4 * addr);	// $t0 -> addr
				insertMipsCode(li, "$t2", "", "", column);			// $t2 -> column
				insertMipsCode(mult, sy, "$t2", "");				// $t1 * $t2 
				insertMipsCode(mflo, "$t1", "", "");				// $t1

				get2 = false;
				loadValue(mc.backup, sz, true, va2, get2);			// $t2 -> mc.backup 第二个下标数字

				insertMipsCode(add, "$t1", "$t1", sz);				// $t1 + $t2
				insertMipsCode(sll, "$t1", "$t1", "", 2);			// $t1 << 2

				insertMipsCode(sub, "$t0", "$t0", "$t1");			// $t0 - $t1 获取该元素所在地址 
				insertMipsCode(lw, st, "$t0", "", 0);			// lw st, 0($t0)

			}
			else if (symList[PROGRAM].find(mc.left) != symList[PROGRAM].end() && symList[PROGRAM][mc.left].type % 3 != 1) {
				addr = symList[PROGRAM][mc.left].addr;			// 该数组的起始点地址
				column = symList[PROGRAM][mc.left].columns;		// 该数组的每一行元素数量

				get1 = false;
				loadValue(mc.right, sy, true, va, get1);			// $t1 -> mc.right 第一个下标数字

				insertMipsCode(addi, "$t0", "$gp", "", 4 * addr);	// $t0 -> addr
				insertMipsCode(li, "$t2", "", "", column);			// $t2 -> column
				insertMipsCode(mult, sy, "$t2", "");				// $t1 * $t2 
				insertMipsCode(mflo, "$t1", "", "");				// $t1

				get2 = false;
				loadValue(mc.backup, sz, true, va2, get2);			// $t2 -> mc.backup 第二个下标数字

				insertMipsCode(add, "$t1", "$t1", sz);				// $t1 + $t2
				insertMipsCode(sll, "$t1", "$t1", "", 2);			// $t1 << 2

				insertMipsCode(add, "$t0", "$t0", "$t1");			// $t0 - $t1 获取该元素所在地址
				insertMipsCode(lw, st, "$t0", "", 0);			// lw st, 0($t0)		
			}

			if (mc.result[0] == '#') {  //mc.result是中间变量
				if (find == -1) {  //没有空闲寄存器
					storeValue(mc.result, st);
				} //有空闲寄存器的话 已经直接保存到它里边了
			}
			else {
				if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
					&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2)) {  //局部变量
					if (sfind == -1) {  //没有空闲
						storeValue(mc.result, st);
					} //有空闲寄存器的话 已经直接保存到它里边了
				}
				else {  //全局变量等
					storeValue(mc.result, st);
				}
			}

			break;
		}
		case PUTARRAY2: {
			// mc.result << "[" << mc.left << "]" << "[" << mc.right << "]" << " = " << mc.backup << endl;
			string sx = "$t0", sy = "$t1", sz = "$t2", s4 = "$t4";
			if (symList[curFuncName].find(mc.result) != symList[curFuncName].end() && symList[curFuncName][mc.result].type % 3 != 1) {
				addr = symList[curFuncName][mc.result].addr;		// 该数组的起始点地址	
				column = symList[curFuncName][mc.result].columns;	// 该数组的每一行元素数量

				get1 = false;
				loadValue(mc.left, sy, true, va, get1);				// $t1 -> mc.left 第一个下标数字

				insertMipsCode(addi, "$t0", "$fp", "", -4 * addr);	// $t0 -> addr
				insertMipsCode(li, "$t2", "", "", column);			// $t2 -> column
				insertMipsCode(mult, sy, "$t2", "");				// index * column
				insertMipsCode(mflo, "$t1", "", "");

				get2 = false;
				loadValue(mc.right, sz, true, va2, get2);			// $t2 -> mc.right 第二个下标数字

				insertMipsCode(add, "$t1", "$t1", sz);				// $t1 + $t2
				insertMipsCode(sll, "$t1", "$t1", "", 2);

				insertMipsCode(sub, "$t0", "$t0", "$t1");			// 获取该元素所在地址

				get1 = false;
				loadValue(mc.backup, sy, true, va, get1);			// $t1 -> mc.backup 赋值

				insertMipsCode(sw, sy, "$t0", "", 0);
			}
			else if (symList[PROGRAM].find(mc.result) != symList[PROGRAM].end() && symList[PROGRAM][mc.result].type % 3 != 1) {  //array
				addr = symList[PROGRAM][mc.result].addr;		// 该数组的起始点地址	
				column = symList[PROGRAM][mc.result].columns;	// 该数组的每一行元素数量

				get1 = false;
				loadValue(mc.left, sy, true, va, get1);				// $t1 -> mc.left 第一个下标数字

				insertMipsCode(addi, "$t0", "$gp", "", 4 * addr);	// $t0 -> addr 获取该数组所在地址
				insertMipsCode(li, "$t2", "", "", column);			// $t2 -> column
				insertMipsCode(mult, sy, "$t2", "");				// index * column
				insertMipsCode(mflo, "$t1", "", "");

				get2 = false;
				loadValue(mc.right, sz, true, va2, get2);			// $t2 -> mc.right 第二个下标数字

				insertMipsCode(add, "$t1", "$t1", sz);				// $t1 + $t2
				insertMipsCode(sll, "$t1", "$t1", "", 2);

				insertMipsCode(add, "$t0", "$t0", "$t1");			// $t0 + $t1 获取该元素所在地址

				get1 = false;
				loadValue(mc.backup, sy, true, va, get1);			// $t1 -> mc.backup 赋值

				insertMipsCode(sw, sy, "$t0", "", 0);
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
}

void generateMipsCodes() {
	
	insertMipsCode(dataSeg, "", "", "", 0);				// .data
	for (int i = 0; i < stringList.size(); i++) {
		insertMipsCode(asciizSeg, "s_" + int2string(i), stringList[i], "");
	}
	insertMipsCode(asciizSeg, "nextLine", "\\n", "");	// .asciiz
	insertMipsCode(textSeg, "", "", "");				// .text
	
	insertProgramMipsCodes();
	// TODO
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
	int releaseCnt = 0;
	string switchEndLabel;

	for (int l = 0; l < funcNameList.size(); l++) {
		
		cout << "ttttt" << funcNameList.size() << endl;
		
		curFuncName = funcNameList[l];
		if (isDebug) {
			cout << curFuncName << ":\n";
		}
		vector<Block> blVe = funcBlockTable[curFuncName];
		for (int si = 0; si <= 7; si++) {  //遍历新的函数了 可以全部清空
			sRegBusy[si] = 0;
			sRegContent[si] = "";
		}
		bool canRelese = true;
		char lorc = ' ';

		cout << blVe.size() << endl;

		for (int w = 0; w < blVe.size(); w++) {
			if (blVe[w].midCodeVector[0].op == LABEL) {
				string label = blVe[w].midCodeVector[0].result;
				int len = label.size();
				if (label[len - 1] == 'B') {
					releaseCnt++;
				}
				if (label[len - 1] == 'E') {
					releaseCnt--;
				}
			}
			if (releaseCnt == 0) {
				for (int si = 0; si <= 7; si++) {
					if (sRegBusy[si]) {
						vector<string> blockIn = blVe[w].in;

						cout << "*****look for " << sRegContent[si] << endl;
						cout << blockIn.size() << endl;
						for (string s : blockIn) {
							cout << s << endl;
						}

						if (find(blockIn.begin(), blockIn.end(), sRegContent[si]) == blockIn.end()) {
							if (isDebug) {
								cout << "del " << "$s" + int2string(si) << " = " << sRegContent[si] << "\n";
							}
							sRegBusy[si] = 0;
							sRegContent[si] = "";
						}
					}
				}
			}
			vector<midCode> mcVe = blVe[w].midCodeVector;
			for (int i = 0; i < mcVe.size(); i++) {
				midCode mc = mcVe[i], mcNext = mc;
				if (i < midCodes.size() - 1) {
					mcNext = midCodes[i + 1];
				}

				switch (mc.op) {
				case PLUSOP: {
					string sx = "$t0", sy = "$t1", sz = "$t2";
					get1 = false;
					loadValue(mc.left, sx, false, va, get1);
					get2 = false;
					loadValue(mc.right, sy, false, va2, get2);
					int find;
					int sfind = -1;


					if (mc.result[0] == '#') {  //mc.z是中间变量 分配t寄存器
						find = findEmptyTReg();
						if (find != -1) {  //有空闲
							tRegBusy[find] = 1;  //打标记
							tRegContent[find] = mc.result;  //find这个寄存器保存了mc.z的值
							sz = "$t" + int2string(find);  //sz修改成$t(find) 直接给他赋值 而不需要move sz, $t(find)
							if (isDebug) {
								cout << sz << " = " << mc.result << "\n";
							}
						}
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2 || symList[curFuncName][mc.result].kind == 5)) {  //局部变量
							sfind = findNameHaveSReg(mc.result);
							if (sfind != -1) {  //被分配了寄存器 直接用寄存器的值
								sz = "$s" + int2string(sfind);
							}
							else {  //没有被分配寄存器
								sfind = findEmptySReg();
								if (sfind != -1) {  //有空闲
									sRegBusy[sfind] = 1;  //打标记
									sRegContent[sfind] = mc.result;  //find这个寄存器保存了mc.z的值
									sz = "$s" + int2string(sfind);  //sz修改成$s(sfind) 直接给他赋值 而不需要move sz, $s(sfind)
									if (isDebug) {
										cout << sz << " = " << mc.result << "\n";
									}
								}
							}
						}
					}


					if (get1 && get2) {
						insertMipsCode(li, sz, "", "", va + va2);
					}
					else if (get1 && !get2) {
						insertMipsCode(addi, sz, sy, "", va);
					}
					else if (!get1 && get2) {
						insertMipsCode(addi, sz, sx, "", va2);
					}
					else {
						insertMipsCode(add, sz, sx, sy);
					}
					if (mc.result[0] == '#') {  //mc.z是中间变量
						if (find == -1) {  //没有空闲寄存器
							storeValue(mc.result, sz);
						} //有空闲寄存器的话 已经直接保存到它里边了
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2)) {  //局部变量SPECIAL
							if (sfind == -1) {  //没有空闲
								storeValue(mc.result, sz);
							} //有空闲寄存器的话 已经直接保存到它里边了
						}
						else {  //全局变量等
							storeValue(mc.result, sz);
						}
					}
					break;
				}
				case MINUOP: {
					string sx = "$t0", sy = "$t1", sz = "$t2";
					get1 = false;
					loadValue(mc.left, sx, false, va, get1);
					get2 = false;
					loadValue(mc.right, sy, false, va2, get2);
					
					int find;
					int sfind = -1;
					if (mc.result[0] == '#') {  //mc.result是中间变量 分配t寄存器
						find = findEmptyTReg();
						if (find != -1) {  //有空闲
							tRegBusy[find] = 1;  //打标记
							tRegContent[find] = mc.result;  //find这个寄存器保存了mc.result的值
							sz = "$t" + int2string(find);  //sz修改成$t(find) 直接给他赋值 而不需要move sz, $t(find)
							if (isDebug) {
								cout << sz << " = " << mc.result << "\n";
							}
						}
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2 || symList[curFuncName][mc.result].kind == 5)) {  //局部变量special
							sfind = findNameHaveSReg(mc.result);
							if (sfind != -1) {  //被分配了寄存器 直接用寄存器的值
								sz = "$s" + int2string(sfind);
							}
							else {  //没有被分配寄存器
								sfind = findEmptySReg();
								if (sfind != -1) {  //有空闲
									sRegBusy[sfind] = 1;  //打标记
									sRegContent[sfind] = mc.result;  //find这个寄存器保存了mc.result的值
									sz = "$s" + int2string(sfind);  //sz修改成$s(sfind) 直接给他赋值 而不需要move sz, $s(sfind)
									if (isDebug) {
										cout << sz << " = " << mc.result << "\n";
									}
								}
							}
						}
					}


					if (get1 && get2) {
						insertMipsCode(li, sz, "", "", va - va2);
					}
					else if (get1 && !get2) {  //va - $t1
						if (va != 0) {
							insertMipsCode(addi, sz, sy, "", -va);	// $t1-va
							insertMipsCode(sub, sz, "$0", sz);		//0-$t2
						}
						else {
							insertMipsCode(sub, sz, "$0", sy);
						}
					}
					else if (!get1 && get2) {  //$t0 - va2
						insertMipsCode(addi, sz, sx, "", -va2);
					}
					else {
						insertMipsCode(sub, sz, sx, sy);
					}
					
					if (mc.result[0] == '#') {  //mc.result是中间变量
						if (find == -1) {  //没有空闲寄存器
							storeValue(mc.result, sz);
						} //有空闲寄存器的话 已经直接保存到它里边了
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2)) {  //局部变量
							if (sfind == -1) {  //没有空闲
								storeValue(mc.result, sz);
							} //有空闲寄存器的话 已经直接保存到它里边了
						}
						else {  //全局变量等
							storeValue(mc.result, sz);
						}
					}
					break;
				}
				case MULTOP: {
					string sx = "$t0", sy = "$t1", sz = "$t2";
					get1 = false;
					loadValue(mc.left, sx, false, va, get1);
					get2 = false;
					loadValue(mc.right, sy, false, va2, get2);
					int find;
					int sfind = -1;
					if (mc.result[0] == '#') {  //mc.result是中间变量 分配t寄存器
						find = findEmptyTReg();
						if (find != -1) {  //有空闲
							tRegBusy[find] = 1;  //打标记
							tRegContent[find] = mc.result;  //find这个寄存器保存了mc.result的值
							sz = "$t" + int2string(find);  //sz修改成$t(find) 直接给他赋值 而不需要move sz, $t(find)
							if (isDebug) {
								cout << sz << " = " << mc.result << "\n";
							}
						}
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2 || symList[curFuncName][mc.result].kind == 5)) {  //局部变量
							sfind = findNameHaveSReg(mc.result);
							if (sfind != -1) {  //被分配了寄存器 直接用寄存器的值
								sz = "$s" + int2string(sfind);
							}
							else {  //没有被分配寄存器
								sfind = findEmptySReg();
								if (sfind != -1) {  //有空闲
									sRegBusy[sfind] = 1;  //打标记
									sRegContent[sfind] = mc.result;  //find这个寄存器保存了mc.result的值
									sz = "$s" + int2string(sfind);  //sz修改成$s(sfind) 直接给他赋值 而不需要move sz, $s(sfind)
									if (isDebug) {
										cout << sz << " = " << mc.result << "\n";
									}
								}
							}
						}
					}

					if (get1 && get2) {
						insertMipsCode(li, sz, "", "", va * va2);
					}
					else if (get1 && !get2) {
						if ((va & (va - 1)) == 0) {
							int power = 0;
							int tmpNumber = va;
							for (int k = 0; k < 32; k++) {
								if ((tmpNumber & (1 << k)) != 0) {
									power = k;
									break;
								}
							}
							insertMipsCode(sll, sz, sy, "", power);
						}


						else if (va == 1) {  //sz=1*sy=sy
							insertMipsCode(moveop, sz, sy, "");
						}
						else if (va == 0) {  //sz=0*sy=0
							insertMipsCode(li, sz, "", "", 0);
						}
						else {
							insertMipsCode(li, sx, "", "", va);
							insertMipsCode(mul, sz, sx, sy);
						}
					}
					else if (!get1 && get2) {
						if ((va2 & (va2 - 1)) == 0) {
							int power = 0;
							int tmpNumber = va2;
							for (int k = 0; k < 32; k++) {
								if ((tmpNumber & (1 << k)) != 0) {
									power = k;
									break;
								}
							}
							insertMipsCode(sll, sz, sx, "", power);
						}

						else if (va2 == 1) {  //sz=sx*1=sx
							insertMipsCode(moveop, sz, sx, "");
						}
						else if (va2 == 0) {  //sz=sx*0=0
							insertMipsCode(li, sz, "", "", 0);
						}
						else {
							insertMipsCode(li, sy, "", "", va2);
							insertMipsCode(mul, sz, sx, sy);
						}
					}
					else {
						insertMipsCode(mul, sz, sx, sy);
					}

					if (mc.result[0] == '#') {  //mc.result是中间变量
						if (find == -1) {  //没有空闲寄存器
							storeValue(mc.result, sz);
						} //有空闲寄存器的话 已经直接保存到它里边了
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2)) {  //局部变量
							if (sfind == -1) {  //没有空闲
								storeValue(mc.result, sz);
							} //有空闲寄存器的话 已经直接保存到它里边了
						}
						else {  //全局变量等
							storeValue(mc.result, sz);
						}
					}
					break;
				}
				case DIVOP: {
					string sx = "$t0", sy = "$t1", sz = "$t2";
					get1 = false;
					loadValue(mc.left, sx, false, va, get1);
					get2 = false;
					loadValue(mc.right, sy, false, va2, get2);
					int find;
					int sfind = -1;
					if (mc.result[0] == '#') {  //mc.result是中间变量 分配t寄存器
						find = findEmptyTReg();
						if (find != -1) {  //有空闲
							tRegBusy[find] = 1;  //打标记
							tRegContent[find] = mc.result;  //find这个寄存器保存了mc.result的值
							sz = "$t" + int2string(find);  //sz修改成$t(find) 直接给他赋值 而不需要move sz, $t(find)
							if (isDebug) {
								cout << sz << " = " << mc.result << "\n";
							}
						}
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2 || symList[curFuncName][mc.result].kind == 5)) {  //局部变量
							sfind = findNameHaveSReg(mc.result);
							if (sfind != -1) {  //被分配了寄存器 直接用寄存器的值
								sz = "$s" + int2string(sfind);
							}
							else {  //没有被分配寄存器
								sfind = findEmptySReg();
								if (sfind != -1) {  //有空闲
									sRegBusy[sfind] = 1;  //打标记
									sRegContent[sfind] = mc.result;  //find这个寄存器保存了mc.result的值
									sz = "$s" + int2string(sfind);  //sz修改成$s(sfind) 直接给他赋值 而不需要move sz, $s(sfind)
									if (isDebug) {
										cout << sz << " = " << mc.result << "\n";
									}
								}
							}
						}
					}

					if (get1 && get2) {
						insertMipsCode(li, sz, "", "", va / va2);
					}
					else if (get1 && !get2) {
						if (va == 0) {  //sz=0/sy=0
							insertMipsCode(li, sz, "", "", 0);
						}
						else {
							insertMipsCode(li, sx, "", "", va);
							insertMipsCode(divop, sx, sy, "");
							insertMipsCode(mflo, sz, "", "");
						}
					}
					else if (!get1 && get2) {
						if (va2 == 1) {  //sz=sx/1=sx
							insertMipsCode(moveop, sz, sx, "");
						}
						else {
							insertMipsCode(li, sy, "", "", va2);
							insertMipsCode(divop, sx, sy, "");
							insertMipsCode(mflo, sz, "", "");
						}
					}
					else {
						insertMipsCode(divop, sx, sy, "");
						insertMipsCode(mflo, sz, "", "");
					}

					if (mc.result[0] == '#') {  //mc.result是中间变量
						if (find == -1) {  //没有空闲寄存器
							storeValue(mc.result, sz);
						} //有空闲寄存器的话 已经直接保存到它里边了
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2)) {  //局部变量
							if (sfind == -1) {  //没有空闲
								storeValue(mc.result, sz);
							} //有空闲寄存器的话 已经直接保存到它里边了
						}
						else {  //全局变量等
							storeValue(mc.result, sz);
						}
					}

					break;
				}

				case LSSOP: {  
					// <
					string sx = "$t0", sy = "$t1";
					get1 = false;
					loadValue(mc.left, sx, false, va, get1);
					get2 = false;
					loadValue(mc.right, sy, false, va2, get2);
					mcNext = mcVe[i + 1];
					if (mcNext.op == BZ) {  //0跳  x>=y跳
						if (get1 && get2) {
							if (va >= va2) {
								insertMipsCode(j, mcNext.result, "", "");
							}
						}
						else if (get1 && !get2) {  //va >= $t1
							if (va != 0) {
								insertMipsCode(addi, "$t2", sy, "", -va);
								insertMipsCode(blez, "$t2", "", mcNext.result);
							}
							else {
								insertMipsCode(blez, sy, "", mcNext.result);
							}
						}
						else if (!get1 && get2) {  //$t0 >= va2
							if (va2 != 0) {
								insertMipsCode(addi, "$t2", sx, "", -va2);
								insertMipsCode(bgez, "$t2", "", mcNext.result);
							}
							else {
								insertMipsCode(bgez, sx, "", mcNext.result);
							}
						}
						else {  //$t0 >= $t1
							insertMipsCode(bge, sx, sy, mcNext.result);//bge >=跳转 代价3
						}
					}
					else if (mcNext.op == BNZ) {  //1跳 x<y跳
						if (get1 && get2) {
							if (va < va2) {
								insertMipsCode(j, mcNext.result, "", "");
							}
						}
						else if (get1 && !get2) {  // $t1 > va
							if (va != 0) {
								insertMipsCode(addi, "$t2", sy, "", -va);
								insertMipsCode(bgtz, "$t2", "", mcNext.result);
							}
							else {
								insertMipsCode(bgtz, sy, "", mcNext.result);
							}
						}
						else if (!get1 && get2) {  //$t0 < va2
							if (va2 != 0) {
								insertMipsCode(addi, "$t2", sx, "", -va2);
								insertMipsCode(bltz, "$t2", "", mcNext.result);
							}
							else {
								insertMipsCode(bltz, sx, "", mcNext.result);
							}
						}
						else {  //$t0 < $t1
							insertMipsCode(blt, sx, sy, mcNext.result);//bge <跳转 代价3
						}
					}
					i++;
					break;
				}
				case LEQOP: {  //<=
					string sx = "$t0", sy = "$t1";
					get1 = false;
					loadValue(mc.left, sx, false, va, get1);
					get2 = false;
					loadValue(mc.right, sy, false, va2, get2);
					mcNext = mcVe[i + 1];
					if (mcNext.op == BZ) {  //0跳  x>y跳
						if (get1 && get2) {
							if (va > va2) {
								insertMipsCode(j, mcNext.result, "", "");
							}
						}
						else if (get1 && !get2) {  //$t1 < va
							if (va != 0) {  //代价3
								insertMipsCode(addi, "$t2", sy, "", -va);
								insertMipsCode(bltz, "$t2", "", mcNext.result);
							}
							else {  //代价2
								insertMipsCode(bltz, sy, "", mcNext.result);
							}
						}
						else if (!get1 && get2) {  //$t0 > va2
							if (va2 != 0) { //代价3
								insertMipsCode(addi, "$t2", sx, "", -va2);
								insertMipsCode(bgtz, "$t2", "", mcNext.result);
							}
							else {  //代价2
								insertMipsCode(bgtz, sx, "", mcNext.result);
							}
						}
						else {  //$t0 > $t1
							insertMipsCode(bgt, sx, sy, mcNext.result);  //bgt >跳转 代价3
						}
					}
					else if (mcNext.op == BNZ) {  //1跳 x<=y跳
						if (get1 && get2) {
							if (va <= va2) {
								insertMipsCode(j, mcNext.result, "", "");
							}
						}
						else if (get1 && !get2) {  //$t1 >= va
							if (va != 0) {  //代价3
								insertMipsCode(addi, "$t2", sy, "", -va);
								insertMipsCode(bgez, "$t2", "", mcNext.result);
							}
							else {  //代价2
								insertMipsCode(bgez, sy, "", mcNext.result);
							}
						}
						else if (!get1 && get2) {  //$t0 <= va2
							if (va2 != 0) { //代价3
								insertMipsCode(addi, "$t2", sx, "", -va2);
								insertMipsCode(blez, "$t2", "", mcNext.result);
							}
							else {  //代价2
								insertMipsCode(blez, sx, "", mcNext.result);
							}
						}
						else { //$t0 <= $t1
							insertMipsCode(ble, sx, sy, mcNext.result);  //ble <=跳转 代价3
						}
					}
					i++;
					break;
				}
				case GREOP: {  //>
					string sx = "$t0", sy = "$t1";
					get1 = false;
					loadValue(mc.left, sx, false, va, get1);
					get2 = false;
					loadValue(mc.right, sy, false, va2, get2);
					mcNext = mcVe[i + 1];
					if (mcNext.op == BZ) {  //0跳  x<=y跳
						if (get1 && get2) {
							if (va <= va2) {
								insertMipsCode(j, mcNext.result, "", "");
							}
						}
						else if (get1 && !get2) {  //$t1 >= va
							if (va != 0) {  //代价3
								insertMipsCode(addi, "$t2", sy, "", -va);
								insertMipsCode(bgez, "$t2", "", mcNext.result);
							}
							else {  //代价2
								insertMipsCode(bgez, sy, "", mcNext.result);
							}
						}
						else if (!get1 && get2) {  //$t0 <= va2
							if (va2 != 0) { //代价3
								insertMipsCode(addi, "$t2", sx, "", -va2);
								insertMipsCode(blez, "$t2", "", mcNext.result);
							}
							else {  //代价2
								insertMipsCode(blez, sx, "", mcNext.result);
							}
						}
						else { //$t0 <= $t1
							insertMipsCode(ble, sx, sy, mcNext.result);  //ble <=跳转 代价3
						}
					}
					else if (mcNext.op == BNZ) {  //1跳 x>y跳
						if (get1 && get2) {
							if (va > va2) {
								insertMipsCode(j, mcNext.result, "", "");
							}
						}
						else if (get1 && !get2) {  //$t1 < va
							if (va != 0) {  //代价3
								insertMipsCode(addi, "$t2", sy, "", -va);
								insertMipsCode(bltz, "$t2", "", mcNext.result);
							}
							else {  //代价2
								insertMipsCode(bltz, sy, "", mcNext.result);
							}
						}
						else if (!get1 && get2) {  //$t0 > va2
							if (va2 != 0) { //代价3
								insertMipsCode(addi, "$t2", sx, "", -va2);
								insertMipsCode(bgtz, "$t2", "", mcNext.result);
							}
							else {  //代价2
								insertMipsCode(bgtz, sx, "", mcNext.result);
							}
						}
						else {  //$t0 > $t1
							insertMipsCode(bgt, sx, sy, mcNext.result);  //bgt >跳转 代价3
						}
					}
					i++;
					break;
				}
				case GEQOP: {  //>=
					string sx = "$t0", sy = "$t1";
					get1 = false;
					loadValue(mc.left, sx, false, va, get1);
					get2 = false;
					loadValue(mc.right, sy, false, va2, get2);
					mcNext = mcVe[i + 1];
					if (mcNext.op == BZ) {  //0跳  x<y跳
						if (get1 && get2) {
							if (va < va2) {
								insertMipsCode(j, mcNext.result, "", "");
							}
						}
						else if (get1 && !get2) {  //$t1 > va
							if (va != 0) {  //代价3
								insertMipsCode(addi, "$t2", sy, "", -va);
								insertMipsCode(bgtz, "$t2", "", mcNext.result);
							}
							else {  //代价2
								insertMipsCode(bgtz, sy, "", mcNext.result);
							}
						}
						else if (!get1 && get2) {  //$t0 < va2
							if (va2 != 0) { //代价3
								insertMipsCode(addi, "$t2", sx, "", -va2);
								insertMipsCode(bltz, "$t2", "", mcNext.result);
							}
							else {  //代价2
								insertMipsCode(bltz, sx, "", mcNext.result);
							}
						}
						else { //$t0 < $t1
							insertMipsCode(blt, sx, sy, mcNext.result);  //blt <跳转 代价3
						}
					}
					else if (mcNext.op == BNZ) {  //1跳 x>=y跳
						if (get1 && get2) {
							if (va >= va2) {
								insertMipsCode(j, mcNext.result, "", "");
							}
						}
						else if (get1 && !get2) {  //$t1 <= va
							if (va != 0) {  //代价3
								insertMipsCode(addi, "$t2", sy, "", -va);
								insertMipsCode(blez, "$t2", "", mcNext.result);
							}
							else {  //代价2
								insertMipsCode(blez, sy, "", mcNext.result);
							}
						}
						else if (!get1 && get2) {  //$t0 >= va2
							if (va2 != 0) { //代价3
								insertMipsCode(addi, "$t2", sx, "", -va2);
								insertMipsCode(bgez, "$t2", "", mcNext.result);
							}
							else {  //代价2
								insertMipsCode(bgez, sx, "", mcNext.result);
							}
						}
						else {  //$t0 >= $t1
							insertMipsCode(bge, sx, sy, mcNext.result);  //bge >=跳转 代价3
						}
					}
					i++;
					break;
				}
				case EQLOP: {
					string sx = "$t0", sy = "$t1";
					get1 = false;
					loadValue(mc.left, sx, false, va, get1);
					get2 = false;
					loadValue(mc.right, sy, false, va2, get2);
					mcNext = mcVe[i + 1];
					if (mcNext.op == BZ) {  //0跳
						if (get1 && get2) {
							if (va != va2) {
								insertMipsCode(j, mcNext.result, "", "");
							}
						}
						else if (get1 && !get2) {
							if (va != 0) {
								insertMipsCode(li, sx, "", "", va);
								insertMipsCode(bne, sx, sy, mcNext.result);
							}
							else {
								insertMipsCode(bne, "$0", sy, mcNext.result);
							}
						}
						else if (!get1 && get2) {
							if (va2 != 0) {
								insertMipsCode(li, sy, "", "", va2);
								insertMipsCode(bne, sx, sy, mcNext.result);
							}
							else {
								insertMipsCode(bne, sx, "$0", mcNext.result);
							}
						}
						else {
							insertMipsCode(bne, sx, sy, mcNext.result);
						}
					}
					else if (mcNext.op == BNZ) {  //1跳
						if (get1 && get2) {
							if (va == va2) {
								insertMipsCode(j, mcNext.result, "", "");
							}
						}
						else if (get1 && !get2) {
							if (va != 0) {
								insertMipsCode(li, sx, "", "", va);
								insertMipsCode(beq, sx, sy, mcNext.result);
							}
							else {
								insertMipsCode(beq, "$0", sy, mcNext.result);
							}
						}
						else if (!get1 && get2) {
							if (va2 != 0) {
								insertMipsCode(li, sy, "", "", va2);
								insertMipsCode(beq, sx, sy, mcNext.result);
							}
							else {
								insertMipsCode(beq, sx, "$0", mcNext.result);
							}
						}
						else {
							insertMipsCode(beq, sx, sy, mcNext.result);
						}
					}
					i++;
					break;
				}
				case NEQOP: {
					string sx = "$t0", sy = "$t1";
					get1 = false;
					loadValue(mc.left, sx, false, va, get1);
					get2 = false;
					loadValue(mc.right, sy, false, va2, get2);
					mcNext = mcVe[i + 1];
					if (mcNext.op == BZ) {  //0跳
						if (get1 && get2) {
							if (va == va2) {
								insertMipsCode(j, mcNext.result, "", "");
							}
						}
						else if (get1 && !get2) {
							if (va != 0) {
								insertMipsCode(li, sx, "", "", va);
								insertMipsCode(beq, sx, sy, mcNext.result);
							}
							else {
								insertMipsCode(beq, "$0", sy, mcNext.result);
							}
						}
						else if (!get1 && get2) {
							if (va2 != 0) {
								insertMipsCode(li, sy, "", "", va2);
								insertMipsCode(beq, sx, sy, mcNext.result);
							}
							else {
								insertMipsCode(beq, sx, "$0", mcNext.result);
							}
						}
						else {
							insertMipsCode(beq, sx, sy, mcNext.result);
						}
					}
					else if (mcNext.op == BNZ) {  //1跳
						if (get1 && get2) {
							if (va != va2) {
								insertMipsCode(j, mcNext.result, "", "");
							}
						}
						else if (get1 && !get2) {
							if (va != 0) {
								insertMipsCode(li, sx, "", "", va);
								insertMipsCode(bne, sx, sy, mcNext.result);
							}
							else {
								insertMipsCode(bne, "$0", sy, mcNext.result);
							}
						}
						else if (!get1 && get2) {
							if (va2 != 0) {
								insertMipsCode(li, sy, "", "", va2);
								insertMipsCode(bne, sx, sy, mcNext.result);
							}
							else {
								insertMipsCode(bne, sx, "$0", mcNext.result);
							}
						}
						else {
							insertMipsCode(bne, sx, sy, mcNext.result);
						}
					}
					i++;
					break;
				}


				case ASSIGNOP: {
					string sz;
					int find;
					int sfind = -1;
					if (mc.result[0] == '#') {  //mc.result是中间变量 分配t寄存器
						find = findEmptyTReg();
						if (find != -1) {  //有空闲
							tRegBusy[find] = 1;  //打标记
							tRegContent[find] = mc.result;  //find这个寄存器保存了mc.result的值
							sz = "$t" + int2string(find);  //sz修改成$t(find) 直接给他赋值 而不需要move sz, $t(find)
							if (isDebug) {
								cout << sz << " = " << mc.result << "\n";
							}
							//直接把mc.left的值load到寄存器sz中
							loadValue(mc.left, sz, true, va, get1);
							//可能因为mc.left本身被分配了寄存器 没有load 只是返回了sz=mc.left的寄存器名
							if (sz != "$t" + int2string(find)) {
								insertMipsCode(moveop, "$t" + int2string(find), sz, "");
							}
						}
						else {  //没有空闲寄存器 就必须取到一个寄存器中 然后存到内存
							string sx = "$t0";
							loadValue(mc.left, sx, true, va, get1);
							storeValue(mc.result, sx);
						}
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2 || symList[curFuncName][mc.result].kind == 5)) {  //局部变量
							sfind = findNameHaveSReg(mc.result);
							if (sfind != -1) {  //被分配了寄存器 直接用寄存器的值
								sz = "$s" + int2string(sfind);
								//直接把mc.left的值load到寄存器sz中
								loadValue(mc.left, sz, true, va, get1);
								//可能因为mc.left本身被分配了寄存器 没有load 只是返回了sz=mc.left的寄存器名
								if (sz != "$s" + int2string(sfind)) {
									insertMipsCode(moveop, "$s" + int2string(sfind), sz, "");
								}
							}
							else {  //没有被分配寄存器
								sfind = findEmptySReg();
								if (sfind != -1) {  //有空闲
									sRegBusy[sfind] = 1;  //打标记
									sRegContent[sfind] = mc.result;  //find这个寄存器保存了mc.result的值
									sz = "$s" + int2string(sfind);  //sz修改成$s(sfind) 直接给他赋值 而不需要move sz, $s(sfind)
									if (isDebug) {
										cout << sz << " = " << mc.result << "\n";
									}
									//直接把mc.left的值load到寄存器sz中
									loadValue(mc.left, sz, true, va, get1);
									//可能因为mc.left本身被分配了寄存器 没有load 只是返回了sz=mc.left的寄存器名
									if (sz != "$s" + int2string(sfind)) {
										insertMipsCode(moveop, "$s" + int2string(sfind), sz, "");
									}
								}
								else {  //没有空闲寄存器 就必须取到一个寄存器中 然后存到内存
									string sx = "$t0";
									loadValue(mc.left, sx, true, va, get1);
									storeValue(mc.result, sx);
								}
							}
						}
						else {  //全局变量等
							string sx = "$t0";
							loadValue(mc.left, sx, true, va, get1);
							storeValue(mc.result, sx);
						}
					}
					break;
				}


				case GOTO: {
					insertMipsCode(j, mc.result, "", "");
					break;
				}
				case PUSH: {
					pushOpStack.push(mc);
					break;
				}
				case CALL: {
					string sx;
					paramSize = functionParameterCount[mc.result];
					while (paramSize) {
						sx = "$t0";
						paramSize--;
						if (pushOpStack.empty()) {
							cout << "ERROR!!!!!!!!\n";
						}
						else {
							midCode tmpMc = pushOpStack.top();
							pushOpStack.pop();
							get1 = false;
							loadValue(tmpMc.result, sx, false, va, get1);
							if (get1) {
								if (va == 0) {
									insertMipsCode(sw, "$0", "$sp", "", -4 * paramSize);
								}
								else {
									insertMipsCode(li, sx, "", "", va);
									insertMipsCode(sw, sx, "$sp", "", -4 * paramSize);
								}
							}
							else {
								insertMipsCode(sw, sx, "$sp", "", -4 * paramSize);
							}
						}
					}
					vector<string> varList;
					for (int i = 3; i <= 9; i++) {
						if (tRegBusy[i]) {
							varList.push_back("$t" + int2string(i));
						}
					}
					for (int i = 0; i <= 7; i++) {
						if (sRegBusy[i]) {
							varList.push_back("$s" + int2string(i));
						}
					}

					int len = 4 * symList[PROGRAM][mc.result].length + 4 * varList.size() + 8;
					insertMipsCode(addi, "$sp", "$sp", "", -len);
					insertMipsCode(sw, "$ra", "$sp", "", 4);
					insertMipsCode(sw, "$fp", "$sp", "", 8);
					for (int i = 0; i < varList.size(); i++) {
						insertMipsCode(sw, varList[i], "$sp", "", 8 + 4 * i + 4);
					}
					insertMipsCode(addi, "$fp", "$sp", "", len);
					insertMipsCode(jal, mc.result, "", "");
					for (int i = 0; i < varList.size(); i++) {
						insertMipsCode(lw, varList[i], "$sp", "", 8 + 4 * i + 4);
					}
					insertMipsCode(lw, "$fp", "$sp", "", 8);
					insertMipsCode(lw, "$ra", "$sp", "", 4);
					insertMipsCode(addi, "$sp", "$sp", "", len);
					break;
				}
				case RET: {
					string sv = "$v0";
					loadValue(mc.result, sv, true, va, get1, false);
					if (sv != "$v0") {
						insertMipsCode(moveop, "$v0", sv, "");
					}
					insertMipsCode(jr, "$ra", "", "");
					break;
				}
				case RETVALUE: {
					//mc.result 是产生的一个中间变量 需要把$v0的值赋给他 尝试分配寄存器
					if (mc.result[0] == '#') {  //mc.result是中间变量 分配t寄存器
						int find = findEmptyTReg();
						if (find != -1) {  //有空闲
							tRegBusy[find] = 1;  //打标记
							tRegContent[find] = mc.result;  //find这个寄存器保存了mc.result的值
							string sz = "$t" + int2string(find);
							insertMipsCode(moveop, sz, "$v0", "");
							if (isDebug) {
								cout << sz << " = " << mc.result << "\n";
							}
						}
						else {  //没有空闲寄存器
							if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
								&& symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2) {
								addr = symList[curFuncName][mc.result].addr;
								insertMipsCode(sw, "$v0", "$fp", "", -4 * addr);
							}
						}
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2 || symList[curFuncName][mc.result].kind == 5)) {  //局部变量
							int sfind = findNameHaveSReg(mc.result);
							if (sfind != -1) {  //被分配了寄存器 直接用寄存器的值
								string sz = "$s" + int2string(sfind);
								insertMipsCode(moveop, sz, "$v0", "");
							}
							else {  //没有被分配寄存器
								sfind = findEmptySReg();
								if (sfind != -1) {  //有空闲
									sRegBusy[sfind] = 1;  //打标记
									sRegContent[sfind] = mc.result;  //find这个寄存器保存了mc.result的值
									string sz = "$s" + int2string(sfind);  //sz修改成$s(sfind) 直接给他赋值 而不需要move sz, $s(sfind)
									insertMipsCode(moveop, sz, "$v0", "");
									if (isDebug) {
										cout << sz << " = " << mc.result << "\n";
									}
								}
								else {
									addr = symList[curFuncName][mc.result].addr;
									insertMipsCode(sw, "$v0", "$fp", "", -4 * addr);
								}
							}
						}
						else if (symList[PROGRAM].find(mc.result) != symList[PROGRAM].end()
							&& (symList[PROGRAM][mc.result].kind == 1 || symList[PROGRAM][mc.result].kind == 2)) {
							addr = symList[PROGRAM][mc.result].addr;
							insertMipsCode(sw, "$v0", "$gp", "", addr * 4);
						}
					}
					break;
				}

				case SCAN: {
					string symbolName = mc.result;		// symbolName即mc.result，是局部变量或全局变量
					if (symList[curFuncName].find(symbolName) != symList[curFuncName].end()
						&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2 || symList[curFuncName][mc.result].kind == 5)) {

						insertMipsCode(li, "$v0", "", "", symList[curFuncName][symbolName].type == 1 ? 5 : 12);		// int类型变量
						insertMipsCode(syscall, "", "", "");
						
						int sfind = findNameHaveSReg(mc.result);
						if (sfind != -1) {  //被分配了寄存器 直接用寄存器的值
							string sz = "$s" + int2string(sfind);
							insertMipsCode(moveop, sz, "$v0", "");
						}
						else {  //没有被分配寄存器
							int sfind = findEmptySReg();
							if (sfind != -1) {  //有空闲
								sRegBusy[sfind] = 1;  //打标记
								sRegContent[sfind] = mc.result;  //find这个寄存器保存了mc.result的值
								string sz = "$s" + int2string(sfind);  //sz修改成$s(sfind) 直接给他赋值 而不需要move sz, $s(sfind)
								insertMipsCode(moveop, sz, "$v0", "");
								if (isDebug) {
									cout << sz << " = " << mc.result << "\n";
								}
							}
							else {
								addr = symList[curFuncName][symbolName].addr;
								insertMipsCode(sw, "$v0", "$fp", "", -4 * addr);
							}
						}
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
					if (mc.left[0] == '1' || mc.left[0] == '2') {
						//int char
						string sa = "$a0";
						loadValue(mc.result, sa, true, va, get1);
						//这里只需要单纯的给a0赋值 如果mc.result是被分配了寄存器的中间变量
						//sa就会被修改为mc.result的那个寄存器 但是这时a0没有被赋值，只是知道了mc.result的值保存在寄存器sa中
						if (sa != "$a0") {
							insertMipsCode(moveop, "$a0", sa, "");
						}
						insertMipsCode(li, "$v0", "", "", mc.left[0] == '1' ? 1 : 11);
						insertMipsCode(syscall, "", "", "");
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
				case PARAM: {  //对于所有的参数 先直接给他分配s寄存器 同时把值取出来
					int sfind = findEmptySReg();
					if (sfind != -1) {  //有空闲
						sRegBusy[sfind] = 1;  //打标记
						sRegContent[sfind] = mc.left;  //find这个寄存器保存了mc.left的值
						string sx = "$s" + int2string(sfind);
						addr = symList[curFuncName][mc.left].addr;
						insertMipsCode(lw, sx, "$fp", "", -4 * addr);
						if (isDebug) {
							cout << sx << " = " << mc.left << "\n";
						}
					}
					//没有空闲就不分配了
					break;
				}

				case GETARRAY: {
					//midCodefile << mc.result << " = " << mc.left << "[" << mc.right << "]\n";
					//mc.result是局部的变量 或 全局的变量
					//mc.left是数组名
					//mc.right可能是标识符也可能是数值 $t0--->数组的索引
					string sy = "$t0", sz = "$t1";
					get1 = false;
					loadValue(mc.right, sy, false, va, get1);
					int find;
					int sfind = -1;

					if (mc.result[0] == '#') {  //mc.result是中间变量 分配t寄存器
						find = findEmptyTReg();
						if (find != -1) {  //有空闲
							tRegBusy[find] = 1;  //打标记
							tRegContent[find] = mc.result;  //find这个寄存器保存了mc.result的值
							sz = "$t" + int2string(find);  //sz修改成$t(find) 直接给他赋值 而不需要move sz, $t(find)
							if (isDebug) {
								cout << sz << " = " << mc.result << "\n";
							}
						}
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2 || symList[curFuncName][mc.result].kind == 5)) {  //局部变量
							int sfind = findNameHaveSReg(mc.result);
							if (sfind != -1) {  //被分配了寄存器 直接用寄存器的值
								sz = "$s" + int2string(sfind);
							}
							else {  //没有被分配寄存器
								sfind = findEmptySReg();
								if (sfind != -1) {  //有空闲
									sRegBusy[sfind] = 1;  //打标记
									sRegContent[sfind] = mc.result;  //find这个寄存器保存了mc.result的值
									sz = "$s" + int2string(sfind);  //sz修改成$s(sfind) 直接给他赋值 而不需要move sz, $s(sfind)
									if (isDebug) {
										cout << sz << " = " << mc.result << "\n";
									}
								}
							}
						}
					}

					if (symList[curFuncName].find(mc.left) != symList[curFuncName].end()
						&& symList[curFuncName][mc.left].type % 3 != 1) {
						addr = symList[curFuncName][mc.left].addr;
						if (!get1) {
							insertMipsCode(addi, "$t2", "$fp", "", -4 * addr);
							insertMipsCode(sll, "$t0", sy, "", 2);
							insertMipsCode(sub, "$t2", "$t2", "$t0");
							insertMipsCode(lw, sz, "$t2", "", 0);
						}
						else {
							insertMipsCode(lw, sz, "$fp", "", -4 * (addr + va));
						}
					}
					else if (symList[PROGRAM].find(mc.left) != symList[PROGRAM].end()
						&& symList[PROGRAM][mc.left].type % 3 != 1) {
						addr = symList[PROGRAM][mc.left].addr;
						if (!get1) {
							insertMipsCode(addi, "$t2", "$gp", "", 4 * addr);
							insertMipsCode(sll, "$t0", sy, "", 2);
							insertMipsCode(add, "$t2", "$t2", "$t0");
							insertMipsCode(lw, sz, "$t2", "", 0);
						}
						else {
							insertMipsCode(lw, sz, "$gp", "", 4 * (addr + va));
						}
					}
					if (mc.result[0] == '#') {  //mc.result是中间变量
						if (find == -1) {  //没有空闲寄存器
							storeValue(mc.result, sz);
						} //有空闲寄存器的话 已经直接保存到它里边了
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2)) {  //局部变量
							if (sfind == -1) {  //没有空闲
								storeValue(mc.result, sz);
							} //有空闲寄存器的话 已经直接保存到它里边了
						}
						else {  //全局变量等
							storeValue(mc.result, sz);
						}
					}
					break;
				}
				case PUTARRAY: {
					//midCodefile << mc.result << "[" << mc.left << "]" << " = " << mc.right << "\n";
					//mc.left可能是标识符也可能是数值 数组下标 $t0
					//mc.result是数组名
					//mc.right可能是标识符也可能是数值 $t1
					string sx = "$t0", sy = "$t1";
					get2 = false;
					loadValue(mc.right, sy, false, va2, get2);
					get1 = false;
					loadValue(mc.left, sx, false, va, get1);
					if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
						&& symList[curFuncName][mc.result].type % 3 != 1) {
						addr = symList[curFuncName][mc.result].addr;
						if (!get1) {
							insertMipsCode(addi, "$t2", "$fp", "", -4 * addr);
							insertMipsCode(sll, "$t0", sx, "", 2);
							insertMipsCode(sub, "$t2", "$t2", "$t0");
							if (get2) {
								if (va2 == 0) {
									insertMipsCode(sw, "$0", "$t2", "", 0);
								}
								else {
									insertMipsCode(li, sy, "", "", va2);
									insertMipsCode(sw, sy, "$t2", "", 0);
								}
							}
							else {
								insertMipsCode(sw, sy, "$t2", "", 0);
							}
						}
						else { //拿到了数组下标 存在了va中
							if (get2) {
								if (va2 == 0) {
									insertMipsCode(sw, "$0", "$fp", "", -4 * (addr + va));
								}
								else {
									insertMipsCode(li, sy, "", "", va2);
									insertMipsCode(sw, sy, "$fp", "", -4 * (addr + va));
								}
							}
							else {
								insertMipsCode(sw, sy, "$fp", "", -4 * (addr + va));
							}
						}
					}
					else if (symList[PROGRAM].find(mc.result) != symList[PROGRAM].end()
						&& symList[PROGRAM][mc.result].type % 3 != 1) {  //array
						addr = symList[PROGRAM][mc.result].addr;
						if (!get1) {
							insertMipsCode(addi, "$t2", "$gp", "", 4 * addr);
							insertMipsCode(sll, "$t0", sx, "", 2);
							insertMipsCode(add, "$t2", "$t2", "$t0");
							if (get2) {
								if (va2 == 0) {
									insertMipsCode(sw, "$0", "$t2", "", 0);
								}
								else {
									insertMipsCode(li, sy, "", "", va2);
									insertMipsCode(sw, sy, "$t2", "", 0);
								}
							}
							else {
								insertMipsCode(sw, sy, "$t2", "", 0);
							}
						}
						else {
							if (get2) {
								if (va2 == 0) {
									insertMipsCode(sw, "$0", "$gp", "", (addr + va) * 4);
								}
								else {
									insertMipsCode(li, sy, "", "", va2);
									insertMipsCode(sw, sy, "$gp", "", (addr + va) * 4);
								}
							}
							else {
								insertMipsCode(sw, sy, "$gp", "", (addr + va) * 4);
							}
						}
					}
					break;
				}
				case EXIT: {
					insertMipsCode(li, "$v0", "", "", 10);
					insertMipsCode(syscall, "", "", "");
					break;
				}
				case INLINEEND: {
					int be = string2int(mc.result);
					int en = string2int(mc.left);
					for (int si = 0; si <= 7; si++) {
						if (sRegBusy[si] && sRegContent[si][0] == '%') {
							int id = string2int(sRegContent[si].substr(8));
							if (id >= be && id <= en) {
								if (isDebug) {
									cout << "del " << "$s" + int2string(si) << " = " << sRegContent[si] << "\n";
								}
								sRegBusy[si] = 0;
								sRegContent[si] = "";
							}
						}
					}
					break;
				}
				case INLINERET: {
					string sv = "$v0";
					loadValue(mc.result, sv, true, va, get1, false);
					if (sv != "$v0") {
						insertMipsCode(moveop, "$v0", sv, "");
					}
					break;
				}

				case GETARRAY2: {
					//mc.result << " = " << mc.left << "[" << mc.right << "]" << "[" << mc.backup << "]" << endl;
					string sx = "$t0", sy = "$t1", sz = "$t2", st="$t3";
					int find;
					int sfind = -1;
					
					if (mc.result[0] == '#') {  //mc.result是中间变量 分配t寄存器
						find = findEmptyTReg();
						if (find != -1) {  //有空闲
							tRegBusy[find] = 1;  //打标记
							tRegContent[find] = mc.result;  //find这个寄存器保存了mc.result的值
							st = "$t" + int2string(find);  //sz修改成$t(find) 直接给他赋值 而不需要move sz, $t(find)
							if (isDebug) {
								cout << st << " = " << mc.result << "\n";
							}
						}
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2 || symList[curFuncName][mc.result].kind == 5)) {  //局部变量
							int sfind = findNameHaveSReg(mc.result);
							if (sfind != -1) {  //被分配了寄存器 直接用寄存器的值
								st = "$s" + int2string(sfind);
							}
							else {  //没有被分配寄存器
								sfind = findEmptySReg();
								if (sfind != -1) {  //有空闲
									sRegBusy[sfind] = 1;  //打标记
									sRegContent[sfind] = mc.result;  //find这个寄存器保存了mc.result的值
									st = "$s" + int2string(sfind);  //sz修改成$s(sfind) 直接给他赋值 而不需要move sz, $s(sfind)
									if (isDebug) {
										cout << st << " = " << mc.result << "\n";
									}
								}
							}
						}
					}

					
					if (symList[curFuncName].find(mc.left) != symList[curFuncName].end() && symList[curFuncName][mc.left].type % 3 != 1) {
						addr = symList[curFuncName][mc.left].addr;			// 该数组的起始点地址
						column = symList[curFuncName][mc.left].columns;		// 该数组的每一行元素数量

						get1 = false;
						loadValue(mc.right, sy, true, va, get1);			// $t1 -> mc.right 第一个下标数字

						insertMipsCode(addi, "$t0", "$fp", "", -4 * addr);	// $t0 -> addr
						insertMipsCode(li, "$t2", "", "", column);			// $t2 -> column
						insertMipsCode(mult, sy, "$t2", "");				// $t1 * $t2 
						insertMipsCode(mflo, "$t1", "", "");				// $t1

						get2 = false;
						loadValue(mc.backup, sz, true, va2, get2);			// $t2 -> mc.backup 第二个下标数字

						insertMipsCode(add, "$t1", "$t1", sz);				// $t1 + $t2
						insertMipsCode(sll, "$t1", "$t1", "", 2);			// $t1 << 2

						insertMipsCode(sub, "$t0", "$t0", "$t1");			// $t0 - $t1 获取该元素所在地址 
						insertMipsCode(lw, st, "$t0", "", 0);			// lw st, 0($t0)

					}
					else if (symList[PROGRAM].find(mc.left) != symList[PROGRAM].end() && symList[PROGRAM][mc.left].type % 3 != 1) {
						addr = symList[PROGRAM][mc.left].addr;			// 该数组的起始点地址
						column = symList[PROGRAM][mc.left].columns;		// 该数组的每一行元素数量
				
						get1 = false;
						loadValue(mc.right, sy, true, va, get1);			// $t1 -> mc.right 第一个下标数字

						insertMipsCode(addi, "$t0", "$gp", "", 4 * addr);	// $t0 -> addr
						insertMipsCode(li, "$t2", "", "", column);			// $t2 -> column
						insertMipsCode(mult, sy, "$t2", "");				// $t1 * $t2 
						insertMipsCode(mflo, "$t1", "", "");				// $t1

						get2 = false;
						loadValue(mc.backup, sz, true, va2, get2);			// $t2 -> mc.backup 第二个下标数字

						insertMipsCode(add, "$t1", "$t1", sz);				// $t1 + $t2
						insertMipsCode(sll, "$t1", "$t1", "", 2);			// $t1 << 2

						insertMipsCode(add, "$t0", "$t0", "$t1");			// $t0 - $t1 获取该元素所在地址
						insertMipsCode(lw, st, "$t0", "", 0);			// lw st, 0($t0)		
					}
					
					if (mc.result[0] == '#') {  //mc.result是中间变量
						if (find == -1) {  //没有空闲寄存器
							storeValue(mc.result, st);
						} //有空闲寄存器的话 已经直接保存到它里边了
					}
					else {
						if (symList[curFuncName].find(mc.result) != symList[curFuncName].end()
							&& (symList[curFuncName][mc.result].kind == 1 || symList[curFuncName][mc.result].kind == 2)) {  //局部变量
							if (sfind == -1) {  //没有空闲
								storeValue(mc.result, st);
							} //有空闲寄存器的话 已经直接保存到它里边了
						}
						else {  //全局变量等
							storeValue(mc.result, st);
						}
					}

					break;
				}
				case PUTARRAY2: {
					// mc.result << "[" << mc.left << "]" << "[" << mc.right << "]" << " = " << mc.backup << endl;
					string sx = "$t0", sy = "$t1", sz="$t2", s4 = "$t4";
					if (symList[curFuncName].find(mc.result) != symList[curFuncName].end() && symList[curFuncName][mc.result].type % 3 != 1) {
						addr = symList[curFuncName][mc.result].addr;		// 该数组的起始点地址	
						column = symList[curFuncName][mc.result].columns;	// 该数组的每一行元素数量
				
						get1 = false;
						loadValue(mc.left, sy, true, va, get1);				// $t1 -> mc.left 第一个下标数字

						insertMipsCode(addi, "$t0", "$fp", "", -4 * addr);	// $t0 -> addr
						insertMipsCode(li, "$t2", "", "", column);			// $t2 -> column
						insertMipsCode(mult, sy, "$t2", "");				// index * column
						insertMipsCode(mflo, "$t1", "", "");

						get2 = false;
						loadValue(mc.right, sz, true, va2, get2);			// $t2 -> mc.right 第二个下标数字

						insertMipsCode(add, "$t1", "$t1", sz);				// $t1 + $t2
						insertMipsCode(sll, "$t1", "$t1", "", 2);

						insertMipsCode(sub, "$t0", "$t0", "$t1");			// 获取该元素所在地址

						sy = "$t1";
						get1 = false;
						loadValue(mc.backup, sy, true, va, get1);			// $t1 -> mc.backup 赋值

						insertMipsCode(sw, sy, "$t0", "", 0);
					}
					else if (symList[PROGRAM].find(mc.result) != symList[PROGRAM].end() && symList[PROGRAM][mc.result].type % 3 != 1) {  //array
						addr = symList[PROGRAM][mc.result].addr;		// 该数组的起始点地址	
						column = symList[PROGRAM][mc.result].columns;	// 该数组的每一行元素数量
				
						get1 = false;
						loadValue(mc.left, sy, true, va, get1);				// $t1 -> mc.left 第一个下标数字

						insertMipsCode(addi, "$t0", "$gp", "", 4 * addr);	// $t0 -> addr 获取该数组所在地址
						insertMipsCode(li, "$t2", "", "", column);			// $t2 -> column
						insertMipsCode(mult, sy, "$t2", "");				// index * column
						insertMipsCode(mflo, "$t1", "", "");

						get2 = false;
						loadValue(mc.right, sz, true, va2, get2);			// $t2 -> mc.right 第二个下标数字

						insertMipsCode(add, "$t1", "$t1", sz);				// $t1 + $t2
						insertMipsCode(sll, "$t1", "$t1", "", 2);

						insertMipsCode(add, "$t0", "$t0", "$t1");			// $t0 + $t1 获取该元素所在地址

						sy = "$t1";
						get1 = false;
						loadValue(mc.backup, sy, true, va, get1);			// $t1 -> mc.backup 赋值

						insertMipsCode(sw, sy, "$t0", "", 0);
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
		}
	}

	insertMipsCode(li, "$v0", "", "", 10);
	insertMipsCode(syscall, "", "", "");
}

void loadValue(string& symbolName, string& regName, bool gene, int& va, bool& get, bool assign) {
	int addr;
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
			// 局部变量或中间变量
			if (symbolName[0] == '#') {  //symbolName是中间变量 看他是否被分配了寄存器
				int find = findNameHaveTReg(symbolName);  //看symbolName是否被分配了寄存器
				if (find != -1) {  //被分配了寄存器 直接用寄存器的值
					regName = "$t" + int2string(find);
					tRegBusy[find] = 0;  //取消标记
					tRegContent[find] = "";  //取消内容
					if (isDebug) {
						cout << "del " << regName << " = " << symbolName << "\n";
					}
				}
				else {  //没有寄存器 需要lw
					addr = symList[curFuncName][symbolName].addr;
					insertMipsCode(lw, regName, "$fp", "", -4 * addr);
				}
			}
			else {  //symbolName是局部变量 而且非数组
				int sfind = findNameHaveSReg(symbolName);
				if (sfind != -1) {
					regName = "$s" + int2string(sfind);
				}
				else { //没有被分配寄存器
					if (assign) {
						sfind = findEmptySReg();
						if (sfind != -1) {  //有空闲
							sRegBusy[sfind] = 1;  //打标记
							sRegContent[sfind] = symbolName;  //find这个寄存器保存了symbolName的值
							regName = "$s" + int2string(sfind);
							addr = symList[curFuncName][symbolName].addr;
							insertMipsCode(lw, regName, "$fp", "", -4 * addr);
							if (isDebug) {
								cout << regName << " = " << symbolName << "\n";
							}
						}
						else {
							addr = symList[curFuncName][symbolName].addr;
							insertMipsCode(lw, regName, "$fp", "", -4 * addr);
						}
					}
					else {
						addr = symList[curFuncName][symbolName].addr;
						insertMipsCode(lw, regName, "$fp", "", -4 * addr);
					}
				}
			}
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

void storeValue(string& symbolName, string& regName) {
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
	case mul:
		mipsCodeFile << "mul " << mc.result << "," << mc.left << "," << mc.right << "\n";
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
	case blez:
		mipsCodeFile << "blez " << mc.result << "," << mc.right << "\n";
		break;
	case bgtz:
		mipsCodeFile << "bgtz " << mc.result << "," << mc.right << "\n";
		break;
	case bgez:
		mipsCodeFile << "bgez " << mc.result << "," << mc.right << "\n";
		break;
	case bltz:
		mipsCodeFile << "bltz " << mc.result << "," << mc.right << "\n";
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
