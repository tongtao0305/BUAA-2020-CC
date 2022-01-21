#include "Optimization.h"

void splitBlock() {
	for (map<string, vector<midCode> >::iterator it = funcMidCodeTable.begin(); it != funcMidCodeTable.end(); it++) {
		string funcName = (*it).first;
		vector<midCode> mcVe = (*it).second;
		set<int> split;
		vector<int> splitVe;
		map<string, int> label2poi;
		for (int i = 0; i < mcVe.size(); i++) {
			midCode mc = mcVe[i];
			if (mc.op == FUNC) {
				split.insert(i);
			}
			else if (mc.op == LABEL) {
				split.insert(i);
				label2poi[mc.result] = i;
			}
			else if (mc.op == BZ || mc.op == BNZ || mc.op == GOTO || mc.op == RET || mc.op == EXIT) {
				split.insert(i + 1);
			}
		}
		split.insert(mcVe.size());
		
		splitVe.assign(split.begin(), split.end());
		vector<Block> blockVe;
		for (int i = 0; i + 1 < splitVe.size(); i++) {
			Block bl = Block(splitVe[i], splitVe[i + 1] - 1, -1, -1);
			if (mcVe[splitVe[i + 1] - 1].op != RET && mcVe[splitVe[i + 1] - 1].op != EXIT && mcVe[splitVe[i + 1] - 1].op != GOTO) {
				bl.setnextBlock1(splitVe[i + 1]);
			}
			if (mcVe[splitVe[i + 1] - 1].op == BZ || mcVe[splitVe[i + 1] - 1].op == BNZ || mcVe[splitVe[i + 1] - 1].op == GOTO) {
				bl.setnextBlock2(label2poi[mcVe[splitVe[i + 1] - 1].result]);
			}
			for (int j = bl.start; j <= bl.end; j++) {
				bl.insert(mcVe[j]);
			}
			calUseDef(bl, funcName);
			bl.output();
			blockVe.push_back(bl);
		}
		for (int i = 0; i < blockVe.size(); i++) {
			if (blockVe[i].nextBlock1 != -1) {
				if (blockVe[i].nextBlock1 == mcVe.size()) {
					blockVe[i].nextBlock1 = -1;
				}
				else {
					for (int j = 0; j < blockVe.size(); j++) {
						if (blockVe[i].nextBlock1 == blockVe[j].start) {
							blockVe[i].nextBlock1 = j;
							break;
						}
					}
				}
			}
			if (blockVe[i].nextBlock2 != -1) {
				if (blockVe[i].nextBlock2 == mcVe.size()) {
					blockVe[i].nextBlock2 = -1;
				}
				else {
					for (int j = 0; j < blockVe.size(); j++) {
						if (blockVe[i].nextBlock2 == blockVe[j].start) {
							blockVe[i].nextBlock2 = j;
							break;
						}
					}
				}
			}
		}
		funcBlockTable.insert(make_pair(funcName, blockVe));
	}
	calInOut();
	for (map<string, vector<Block> >::iterator it = funcBlockTable.begin(); it != funcBlockTable.end(); it++) {
		string funcName = (*it).first;
		vector<Block> blockVe = (*it).second;
		//cout << blockVe.size() << endl;
		if (isDebug) {
			cout << "-----------------" << funcName << "--------------------\n";
		}
		for (map<string, struct sym>::iterator iter = symList[funcName].begin();
			iter != symList[funcName].end(); iter++) {
			struct sym sitem = (*iter).second;
			if (sitem.kind == 1 && sitem.name[0] != '#' && sitem.name[0] != '%') {  //var
				if (isDebug) {
					cout << "-----------" << (*iter).first << "-------------\n";
				}
				string name = (*iter).first;
				for (int i = 0; i < blockVe.size(); i++) {
					if (find(blockVe[i].def.begin(), blockVe[i].def.end(), name) != blockVe[i].def.end()) {
						int islive = false;
						int defPoi = -1;
						for (int l = blockVe[i].midCodeVector.size() - 1; l >= 0; l--) {
							midCode mc = blockVe[i].midCodeVector[l];
							switch (mc.op) {
							case PLUSOP:
							case MINUOP:
							case MULTOP:
							case DIVOP:
							case LSSOP:
							case LEQOP:
							case GREOP:
							case GEQOP:
							case EQLOP:
							case NEQOP:
							case PUTARRAY:  //mc.result << "[" << mc.left << "]" << " = " << mc.right
								if (mc.left == name || mc.right == name) {
									islive = true;
								}
								if (mc.result == name) {
									defPoi = l;
								}
								break;
							case ASSIGNOP:
								if (mc.left == name) {
									islive = true;
								}
								if (mc.result == name) {
									defPoi = l;
								}
								break;
							case GETARRAY:  //mc.result << " = " << mc.left << "[" << mc.right << "]
								if (mc.right == name) {
									islive = true;
								}
								if (mc.result == name) {
									defPoi = l;
								}
								break;
							case PUSH:
							case RET:
							case INLINERET:
								if (mc.result == name) {
									islive = true;
								}
								break;
							case RETVALUE:
							case SCAN:
								if (mc.result == name) {
									defPoi = l;
								}
								break;
							case PRINT:
								if (mc.left == "1" || mc.left == "2") {
									if (mc.result == name) {
										islive = true;
									}
								}
								break;
							case CONST:
							case ARRAY:
							case VAR:
							case PARAM:
							case FUNC:
							case EXIT:
							case GOTO:
							case BZ:
							case BNZ:
							case CALL:
							case LABEL:
							default:
								break;
							}
							if (islive || defPoi != -1) {
								break;
							}
						}
						if (!islive) {
							for (int j = 0; j < blockVe.size(); j++) {
								if (find(blockVe[j].in.begin(), blockVe[j].in.end(), name) != blockVe[j].in.end()) {
									islive = true;
									break;
								}
							}
						}
						if (!islive) {
							if (isDebug) {
								cout << "%%%%%%%%%%%%%%" << i << " " << name << "%%%%%%%%%%%%%%%%%%%%\n";
							}
							queue<string> que;
							if (blockVe[i].midCodeVector[defPoi].left[0] == '#') {
								que.push(blockVe[i].midCodeVector[defPoi].left);
							}
							if (blockVe[i].midCodeVector[defPoi].right[0] == '#') {
								que.push(blockVe[i].midCodeVector[defPoi].right);
							}
							blockVe[i].midCodeVector.erase(blockVe[i].midCodeVector.begin() + defPoi,
								blockVe[i].midCodeVector.begin() + defPoi + 1);  //defPoiɾȥ
							while (!que.empty()) {
								string name = que.front();
								que.pop();
								for (int qi = defPoi; qi >= 0; qi--) {  //defPoi��ʼ����
									if (blockVe[i].midCodeVector[qi].right == name) {
										if (blockVe[i].midCodeVector[qi].left[0] == '#') {
											que.push(blockVe[i].midCodeVector[qi].left);
										}
										if (blockVe[i].midCodeVector[qi].right[0] == '#') {
											que.push(blockVe[i].midCodeVector[qi].right);
										}
										blockVe[i].midCodeVector.erase(blockVe[i].midCodeVector.begin() + qi,
											blockVe[i].midCodeVector.begin() + qi + 1);  //qiɾȥ
										break;
									}
								}
							}
							funcBlockTable[funcName] = blockVe;
						}
					}
				}
			}
		}
	}
}


void calUseDef(Block& bl, string funcName) {
	set<string> use, def;
	for (int i = 0; i < bl.midCodeVector.size(); i++) {
		midCode mc = bl.midCodeVector[i];
		switch (mc.op) {
		case PLUSOP:
		case MINUOP:
		case MULTOP:
		case DIVOP:
			if (symList[funcName].find(mc.left) != symList[funcName].end()
				&& (symList[funcName][mc.left].kind == 1 || symList[funcName][mc.left].kind == 2) && mc.left[0] != '#') {  //�ֲ�����
				if (use.find(mc.left) == use.end() && def.find(mc.left) == def.end()) {
					use.insert(mc.left);
				}
			}
			if (symList[funcName].find(mc.right) != symList[funcName].end()
				&& (symList[funcName][mc.right].kind == 1 || symList[funcName][mc.right].kind == 2) && mc.right[0] != '#') {  //�ֲ�����
				if (use.find(mc.right) == use.end() && def.find(mc.right) == def.end()) {
					use.insert(mc.right);
				}
			}
			if (symList[funcName].find(mc.result) != symList[funcName].end()
				&& (symList[funcName][mc.result].kind == 1 || symList[funcName][mc.result].kind == 2) && mc.result[0] != '#') {  //�ֲ�����
				if (use.find(mc.result) == use.end() && def.find(mc.result) == def.end()) {
					def.insert(mc.result);
				}
			}
			break;
		case LSSOP:
		case LEQOP:
		case GREOP:
		case GEQOP:
		case EQLOP:
		case NEQOP:
			if (symList[funcName].find(mc.left) != symList[funcName].end()
				&& (symList[funcName][mc.left].kind == 1 || symList[funcName][mc.left].kind == 2) && mc.left[0] != '#') {  //�ֲ�����
				if (use.find(mc.left) == use.end() && def.find(mc.left) == def.end()) {
					use.insert(mc.left);
				}
			}
			if (symList[funcName].find(mc.right) != symList[funcName].end()
				&& (symList[funcName][mc.right].kind == 1 || symList[funcName][mc.right].kind == 2) && mc.right[0] != '#') {  //�ֲ�����
				if (use.find(mc.right) == use.end() && def.find(mc.right) == def.end()) {
					use.insert(mc.right);
				}
			}
			break;
		case ASSIGNOP:
			if (symList[funcName].find(mc.left) != symList[funcName].end()
				&& (symList[funcName][mc.left].kind == 1 || symList[funcName][mc.left].kind == 2) && mc.left[0] != '#') {  //�ֲ�����
				if (use.find(mc.left) == use.end() && def.find(mc.left) == def.end()) {
					use.insert(mc.left);
				}
			}
			if (symList[funcName].find(mc.result) != symList[funcName].end()
				&& (symList[funcName][mc.result].kind == 1 || symList[funcName][mc.result].kind == 2) && mc.result[0] != '#') {  //�ֲ�����
				if (use.find(mc.result) == use.end() && def.find(mc.result) == def.end()) {
					def.insert(mc.result);
				}
			}
			break;
		case GETARRAY:  //mc.result << " = " << mc.left << "[" << mc.right << "]
			if (symList[funcName].find(mc.right) != symList[funcName].end()
				&& (symList[funcName][mc.right].kind == 1 || symList[funcName][mc.right].kind == 2) && mc.right[0] != '#') {  //�ֲ�����
				if (use.find(mc.right) == use.end() && def.find(mc.right) == def.end()) {
					use.insert(mc.right);
				}
			}
			if (symList[funcName].find(mc.result) != symList[funcName].end()
				&& (symList[funcName][mc.result].kind == 1 || symList[funcName][mc.result].kind == 2) && mc.result[0] != '#') {  //�ֲ�����
				if (use.find(mc.result) == use.end() && def.find(mc.result) == def.end()) {
					def.insert(mc.result);
				}
			}
			break;
		case PUSH:
		case RET:
		case INLINERET:
			if (symList[funcName].find(mc.result) != symList[funcName].end()
				&& (symList[funcName][mc.result].kind == 1 || symList[funcName][mc.result].kind == 2) && mc.result[0] != '#') {  //�ֲ�����
				if (use.find(mc.result) == use.end() && def.find(mc.result) == def.end()) {
					use.insert(mc.result);
				}
			}
			break;
		case RETVALUE:
		case SCAN:
			if (symList[funcName].find(mc.result) != symList[funcName].end()
				&& (symList[funcName][mc.result].kind == 1 || symList[funcName][mc.result].kind == 2) && mc.result[0] != '#') {  //�ֲ�����
				if (use.find(mc.result) == use.end() && def.find(mc.result) == def.end()) {
					def.insert(mc.result);
				}
			}
			break;
		case PRINT:
			if (mc.left == "1" || mc.left == "2") {
				if (symList[funcName].find(mc.result) != symList[funcName].end()
					&& (symList[funcName][mc.result].kind == 1 || symList[funcName][mc.result].kind == 2) && mc.result[0] != '#') {  //�ֲ�����
					if (use.find(mc.result) == use.end() && def.find(mc.result) == def.end()) {
						use.insert(mc.result);
					}
				}
			}
			break;
		case PUTARRAY:  //mc.result << "[" << mc.left << "]" << " = " << mc.right
			if (symList[funcName].find(mc.left) != symList[funcName].end()
				&& (symList[funcName][mc.left].kind == 1 || symList[funcName][mc.left].kind == 2) && mc.left[0] != '#') {  //�ֲ�����
				if (use.find(mc.left) == use.end() && def.find(mc.left) == def.end()) {
					use.insert(mc.left);
				}
			}
			if (symList[funcName].find(mc.right) != symList[funcName].end()
				&& (symList[funcName][mc.right].kind == 1 || symList[funcName][mc.right].kind == 2) && mc.right[0] != '#') {  //�ֲ�����
				if (use.find(mc.right) == use.end() && def.find(mc.right) == def.end()) {
					use.insert(mc.right);
				}
			}
			break;
		case CONST:
		case ARRAY:
		case VAR:
		case PARAM:
		case FUNC:
		case EXIT:
		case GOTO:
		case BZ:
		case BNZ:
		case CALL:
		case LABEL:
			break;
		default:
			break;
		}
	}
	for (set<string>::iterator it = use.begin(); it != use.end(); it++) {
		bl.useInsert((*it));
	}
	for (set<string>::iterator it = def.begin(); it != def.end(); it++) {
		bl.defInsert((*it));
	}
}


vector<string> unionVe(vector<string> v1, vector<string> v2) {
	vector<string> res;
	res.insert(res.end(), v1.begin(), v1.end());
	res.insert(res.end(), v2.begin(), v2.end());
	set<string> s(res.begin(), res.end());
	res.assign(s.begin(), s.end());
	return res;
}

vector<string> diffVe(vector<string> v1, vector<string> v2) {
	for (int i = 0; i < v2.size(); i++) {
		vector<string>::iterator iter = find(v1.begin(), v1.end(), v2[i]);
		if (iter != v1.end()) {
			v1.erase(iter);
		}
	}
	return v1;
}

void calInOut() {
	for (map<string, vector<Block> >::iterator it = funcBlockTable.begin(); it != funcBlockTable.end(); it++) {
		string funcName = (*it).first;
		vector<Block> blVe = (*it).second;
		while (true) {
			int cnt = 0;
			for (int i = blVe.size() - 1; i >= 0; i--) {
				vector<string> tOut, tIn, t;
				if (blVe[i].nextBlock1 != -1) {
					tOut = unionVe(tOut, blVe[blVe[i].nextBlock1].in);
				}
				if (blVe[i].nextBlock2 != -1) {
					tOut = unionVe(tOut, blVe[blVe[i].nextBlock2].in);
				}
				blVe[i].out = tOut;
				t = diffVe(tOut, blVe[i].def);
				
				tIn = unionVe(t, blVe[i].use);

				t = diffVe(tIn, blVe[i].in);
				if (t.size() != 0) {
					blVe[i].in = tIn;
				}
				else {
					cnt++;
				}
			}
			if (cnt == blVe.size()) {
				break;
			}
		}
		(*it).second = blVe;
	}
}

void showFuncBlock() {
	for (map<string, vector<Block> >::iterator it = funcBlockTable.begin(); it != funcBlockTable.end(); it++) {
		string funcName = (*it).first;
		vector<Block> blockVe = (*it).second;
		cout << funcName << ":\n";
		for (int i = 0; i < blockVe.size(); i++) {
			blockVe[i].output();
		}
	}
}