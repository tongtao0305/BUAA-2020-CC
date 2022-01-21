const int MIN_NUM = 1;
const int MAX_NUM = 10;
const int x1 = +100, x2 = -100;
int y1;
int y2, arr1[5], arr2[3][3];
int x3 = 100;
int arr3[5] = {1,2,3,4,5};
int arr4[3][3] = {{1,2,3},{4,5,6},{7,8,9}};

int gets1(int n1, int n2) {
	const int x4 = 2;
	y1 = - (n1 - n2) * x4 + 100;
	return (y1);
}

void gets2() {
	int x5 = 2;
	y1 = + x3 / x5;
	y2 = y1;
	y2 = 100;
	arr1[0] = arr3[0];
	arr2[0][0] = arr4[0][0]; 
	return ;
}

int sum(int num) {
	int i = 0;
	int s = 0;
	if (num < MIN_NUM)
		return (-1);
	else if (num > MAX_NUM)
		return (0);
	for (i = 1; i <= num; i = i + 1) {
		s = s + i;
	}
	return (s);
}

void main() {
	int n;
	int s = 0;
	int i = 0;
	scanf(n);
	s = sum(n);
	printf("16061021");
	printf(s);
	printf(gets1(20,10));
	gets2();
	printf("hello world",2*3);
	printf(2*3);
	for (i = 5; i >= 1; i = i-1)
		printf(i);
}
