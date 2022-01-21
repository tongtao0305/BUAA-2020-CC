const int fixed_num1 = 1,fixed_num2 = 2,fixed_num3 = 0;
const char fixed_char1 = 'a',fixed_char2 = 'A';
const char fixed_char3 = 'Z';

int variable_num1,variable_NumArray1[5];
int variable_num2;
char variable_char1 = '0';
char variable_char2 = '+';
char variable_char3 = 'z';
char variable_CharArray1[5] = {'h','e','l','l','o'};

int AbsFunc(int x) {
	if (x > 0) {
		return (x);
	}
	else {
		return (-x);
	}
}

int MaxFunc(int x,int y,int z) {
	int w = -999999;
	if (x > w) {
		w = x;
	}
	if (y > w) {
		w = y;
	}
	if (z > w) {
		w = z;
	}
	return (w);
}

void PrintFunc() {
	printf("18373039");
	return ;
}

void main() {
	const int fixed_num4 = 4;
	const int fixed_num5 = 5,fixed_num6 = 6;
	const int fixed_num7 = 7,fixed_num8 = 10,fixed_num9 = 100;
	int variable_NumArray2[4] = {1,10,100,1000};
	int variable_num3,variable_num4,i;
	int variable_num5,variable_num6,variable_num7;
	PrintFunc();  
	variable_num3 = 0;
	while (variable_num3 < 5) {
		scanf(variable_num4);
		variable_NumArray1[variable_num3] = variable_num4;
		variable_num3 = variable_num3 + 1;
	}
	for (i = 4;i >= 0;i = i - 1) {
		printf(AbsFunc(variable_NumArray1[i]));
	}
	variable_num4 = MaxFunc(fixed_num1,fixed_num2,fixed_num3);
	printf(variable_num4);
	for (i = 0;i < 4;i = i + 1) {
		switch(variable_NumArray2[i]) {
			case 1 : {
				printf("Good!");
			}
			default : {
				
			}
		}
	}
	variable_num4 = fixed_char1 + fixed_char3 - fixed_char2;
	printf(variable_num4);
	printf("Good luck");
	return ;
}