const char A_c='*';		
int A = 222;
int B=333;
int C=444;
int ans;
char B_c='a';
char C_c='b';
char D_c='c';

char check_1(char M_c){
	printf("check_1 M_c=",M_c);
	return (M_c);
}

int check_assign(int n,char N_c){
	char AA_c = '3';
	int AA[3]={0,0,0};
	int BB[2][3]={{1,1,1},{1,1,1}};
	printf("const A_c=",A_c);	

	AA_c=N_c;
	AA[1]=n;
	BB[1][1]=n+1;
	BB[0][0]=-BB[1][1];
	printf("BB[0][0]=",BB[0][0]);		

	printf("AA_c+AA[1]+BB[1][1]=",AA_c+AA[1]+BB[1][1]);
	return (AA_c+AA[1]+BB[1][1]);
}

void check_conditon(){
	int i=0;
	if(A==222)	i=i+1;
	if(A!=10+212)	i=i+2;
	if(A>=110*2+2)	i=i+5;
	if(A<=(50+61)*2)	i=i+10;
	if(A<222)	i=i+20;
	if(A>222)	i=i+50;
	printf("guanxi=",i);	
	
	if(i!=0)	i=0;
	if(A<0){
		i=-1;
		i=i-1;
	}else if(A==0){
		i=0;
	}else i=1;
	printf("tiaojian=",i);

	i=0;
	if(i==0){}
		else i=100;
	printf("kongyuju=",i);
}

void check_empty(){
	;
}

void main(){
	printf("18182601");
	B_c=check_1(C_c);
	printf("check_1=",B_c);

	ans=check_assign(A,B_c);
	printf("check_assign=",ans);

	check_conditon();
	check_empty();
	return;
}
