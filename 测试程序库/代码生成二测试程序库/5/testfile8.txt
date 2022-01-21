const int ZERO=0;
const int ONE=1,TEN=10;
const char o='0';
int test1=+1;
int test2=2;
int var_array[3]={-1,0,1};
int var_array2[1][1]={{1}};
char var_char[3];
char var_char2[2][2],var_char3[3][3];

int bigger_than_zero(int a) {
	const int zero_1 = 0;
	int b = 0;
	if (a > zero_1) return (1);
	return (0); 
}

char get_zero() {
	return ('0');
} 

void hello_world() {
	printf("hello world");
}

void compare_int(int a, int b) {
	return;
}

void main()
{
	int test3;
	char test4; 
	char test_print3='d'; 
	printf("18373075");
	printf(test1);
	hello_world(); 

	test3 = test1;
	test3 = +test1;
	test3 = -test1; 
	printf(test3);

	test3 = test1 + test2;
	test3 = test1 - test2; 
	printf(test3);

	test3 = test1*test2;
	test3 = test1/test2;
	printf(test3); 

	test3 = var_array[0];
	test3 = var_array2[0][0];
	test3 = (test1+test2);
	test3 = 3;
	printf(test3);
	test4 = '1';
	printf(test4);
	test3 = bigger_than_zero(test1);
	printf(test3);

	compare_int(test1,test2);

	test_print3='+';
	test_print3='-';
	test_print3='*';
	test_print3='/';
	test_print3='d';
	printf("abc",test_print3);
	
	return ;
}



