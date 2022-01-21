#include "MidCode.h"

void insertMidCode(string result, string left, midOp op, string right) {
	struct midCode midCode = { result, left, op, right };
	midCodes.push_back(midCode);
}

void outputMidCodes() {
	for (auto midCode : midCodes)
		outputMidCode(midCode);
}

void outputMidCode(struct midCode mc) {
	switch (mc.op) {
	case PLUSOP:
		midCodeFile << mc.result << " = " << mc.left << " + " << mc.right << "\n";
		break;
	case MINUOP:
		midCodeFile << mc.result << " = " << mc.left << " - " << mc.right << "\n";
		break;
	case MULTOP:
		midCodeFile << mc.result << " = " << mc.left << " * " << mc.right << "\n";
		break;
	case DIVOP:
		midCodeFile << mc.result << " = " << mc.left << " / " << mc.right << "\n";
		break;
	case LSSOP:
		midCodeFile << mc.result << " = (" << mc.left << " < " << mc.right << ")\n";
		break;
	case LEQOP:
		midCodeFile << mc.result << " = (" << mc.left << " <= " << mc.right << ")\n";
		break;
	case GREOP:
		midCodeFile << mc.result << " = (" << mc.left << " > " << mc.right << ")\n";
		break;
	case GEQOP:
		midCodeFile << mc.result << " = (" << mc.left << " >= " << mc.right << ")\n";
		break;
	case EQLOP:
		midCodeFile << mc.result << " = (" << mc.left << " == " << mc.right << ")\n";
		break;
	case NEQOP:
		midCodeFile << mc.result << " = (" << mc.left << " != " << mc.right << ")\n";
		break;
	case ASSIGNOP:
		midCodeFile << mc.result << " = " << mc.left << "\n";
		break;
	case GOTO:
		midCodeFile << "GOTO " << mc.result << "\n";
		break;
	case BZ:
		midCodeFile << "BZ " << mc.result << "(" << mc.left << "=0)" << "\n";
		break;
	case BNZ:
		midCodeFile << "BNZ " << mc.result << "(" << mc.left << "=1)" << "\n";
		break;
	case PUSH:
		midCodeFile << "PUSH " << mc.result << "\n";
		break;
	case CALL:
		midCodeFile << "CALL " << mc.result << "\n";
		break;
	case RET:
		midCodeFile << "RET " << mc.result << "\n";
		break;
	case RETVALUE:
		midCodeFile << "RETVALUE " << mc.result << " = " << mc.left << "\n";
		break;
	case SCAN:
		midCodeFile << "SCAN " << mc.result << "\n";
		break;
	case PRINT:
		midCodeFile << "PRINT " << mc.result << " " << mc.left << "\n";
		break;
	case LABEL:
		midCodeFile << mc.result << ": \n";
		break;
		/*case CONST:
			midCodeFile << "CONST " << mc.result << " " << mc.left << " = " << mc.right << endl;
			break;
		case ARRAY:
			midCodeFile << "ARRAY " << mc.result << " " << mc.left << "[" << mc.right << "]" << endl;
			break;
		case VAR:
			midCodeFile << "VAR " << mc.result << " " << mc.left << endl;
			break;*/
	case FUNC:
		midCodeFile << "FUNC " << mc.result << " " << mc.left << "()" << endl;
		break;
	case PARAM:
		midCodeFile << "PARA " << mc.result << " " << mc.left << endl;
		break;
	case GETARRAY:
		midCodeFile << mc.result << " = " << mc.left << "[" << mc.right << "]\n";
		break;
	case PUTARRAY:
		midCodeFile << mc.result << "[" << mc.left << "]" << " = " << mc.right << "\n";
		break;
	case EXIT:
		midCodeFile << "EXIT\n";
		break;
	default:
		break;
	}
}