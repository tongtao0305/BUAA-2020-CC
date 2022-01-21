const int zero=-0, time=10, GGG=+1;
const char _c1='a', _c2='_';
const char hhh='h';
int n, m, x[10];
char chars[10];

char c, nnn;
int as;
char aa;
int inc(int a){
	return(a+1);
}
void func_test(){
	printf("666");
}
void print_test(int a){
	const int qqq=1;
	const char ccc='c';
	printf("Hello World! ");
	printf(a+1);
	printf(" The value is ", a);
}
int cal_test(int a, int b){
	int tmp;
	tmp = +4/n+m;
	return((a+b-_c1*_c2)/4+zero+inc(a));
}

void while_test(int a){
	const int con=1;
	int i;
	while(a!=2){
		printf(a);
		printf(" ");
		a=a-1;
	}
	for(i=0; i<0; i=i+1)
		printf(i);
	for(i=con; i>0; i=i-1)
		printf(i);

}
int assign_test(int a, int b){
	int c;
	c=a;
	c=a+1;
	c=(a+b)*2;
	c=a-b/2;
	x[0]=b+1;
	x[1+2]=-b+1;
	return(x[1]+c);
}
int recursive_test(int a){
	if(a==0)
		return(1);
	if(a==1)
		return(1);
	return(recursive_test(a-1)+recursive_test(a-2));
}
void try1(){
	return;
}
char try2(){
	return('a');
}

void main()
{
	int w1;
	char w2;

	printf("15061081");
	chars[1] = '+';
	chars[2] = '*';
	chars[3] = '6';

	scanf(w1);
	scanf(w2);
	scanf(n);
	scanf(m);
	scanf(c);
	printf(w1);
	printf(w2);
	inc(n);
	{func_test();}
	print_test(n);
	x[0] = 10;
	x[1] = 15;
	printf(cal_test(n, m+1));
	while_test(n);
	assign_test(n,m);
	printf(recursive_test(n));
}