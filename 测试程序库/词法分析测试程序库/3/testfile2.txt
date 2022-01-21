char alpha;
char string[20];
char out[20];

void read(int i){
	scanf(alpha);
	string[i]=alpha;
	return;
}
int print(int i){
	int j; 
	printf(string[i]);
	j = i-1;
	return (j);
}

void test1(){
}

void swap(int i){
	out[i]=string[19-i];
}

char test2(char i,int j){
	return (i);
}

void main(){
	int i=0;
	int step=1;
	int j;
	printf("16061178");
	printf("hello,world",i);
	for(i=0;i<20;i=i+1){
		read(i);
	}
	for(j=19;j>=0;j=j-1){
		swap(j);
	}
	i = i-step;
	while(i>=0){
		j = print(i);
		i = j;
	}
	
	test1();
	test2(alpha,j);
	;
}
