const int a=-1;const int b = +2;
int c=20;int d=5;int rst;int rst2;
int add(int x,int y){
	int sum;
	sum=x+y;
	return(sum);
}
void min(int x,int y){
	rst=x-y;
	return;
}
void mul(int x,int y){
	rst2=x*y;
}
char sameChar(char ch){
	return(ch);
}
char aChar(){
	char ch='a';
	return(ch);
}
void main(){
	const char c1='+';
	char c2='/';
	char c3='a';
	char c4='2';
	int sum;
	int vector[2]={1,2};
	int matrix[2][2]={{3,4},{5,6}};
	printf("18182672");
	sum=add(c,d);
	min(c,d);
	mul(c,d);
	printf(sum);
	printf(rst);
	printf(rst2);
	printf(sameChar('b'));
	printf(aChar());
	printf(7-(c/d));
	if(c==d){
		printf("c==d");
	}else{
		printf("c!=d");
	}
	if(c!=d){}
	printf(vector[1]);
	printf(matrix[1][1]);
	;
}