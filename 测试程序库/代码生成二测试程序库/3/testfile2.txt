const int a = 1;
int b;

int intFunc1() {
	printf("in intFunc1()");
	return (1);
}

char scanfFunc() {
	char a;
	scanf(a);
	return (a);
}

int scanfIntFunc() {
	int a;
	scanf(a);
	return (a);
}
void main() {
	char a;
	int b;
	int i = 10000;
	printf("18231039");
	a = scanfFunc();
	b = scanfIntFunc();
	printf(a);
	printf(b);
	intFunc1();
	printf("+-*/");
	while (i != 0) {
		printf(i);
		i = i / 10;
	}
}
