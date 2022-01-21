const int _ = +1, aA = 10, a1 = -10;
int b[5],b2[5][5];
char c[3] = { 'a','/','-' };
char c2[2][2] = { {'+','*'},{'a','1'} };
int d;
char e = '_';

int Has_ReturnAndOnePara_Func(int a) {
	int temp = 1;
	int temp2;
	int i = 0;
	{
		temp2 = 0;
		b[0] = 0;
		b2[4][4] = 0;
		if (temp2 == 0)
			;
		if (temp2 > 0)
			;
		else
			;
		while (temp2 != 1) {
			temp2 = temp;
			printf("wdnmd", temp + temp2);
			printf("wdnmd2");
		}
		for (i = 0; i < 3; i = i + 1) {
			switch (i) {
			case 0:
				printf("case0");
			default:
				;
			}
		}
		for (i = 3; i >=0; i = i - 1) {
			switch (i) {
			case 0:
				printf("againCase0");
			case 1:
				printf("case1");
			default:
				;
			}
		}
	}
	return (0);
}

void No_Return_No_Para() {
	{; }
}

void Has_Void_Return() {
	printf("this is Has_Void_Return");
	return;
}

char Has_ReturnAndTwoPara_Func(int a, int b) {
	return ('a');
}

void main() {
	int sum;
	char ans;
	scanf(sum);
	printf("18373494");
	sum = Has_ReturnAndOnePara_Func(0);
	ans = Has_ReturnAndTwoPara_Func(0, 0);
	No_Return_No_Para();
	Has_Void_Return();
	sum = aA;
	sum = +aA;
	sum = -b[0];
	sum = b2[0][0];
	sum = (sum);
	sum = 0;
	ans = 'c';
	sum = aA * a1;
	sum = aA + aA;
	printf(sum);
	printf("wdnmd3");
	printf("wdnmd4");
	{

	}
}