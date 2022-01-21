const char t='t';
char c(){
	return ('k');
}
void print(int input){
	printf(input);
}
void main(){
	int array_1[5];
	int array_2[2][2];
	int i=0;
	int j=0;
	int out=0;
	;
	for(i=0;i<=4;i=i+1){
		array_1[i]=i;
	}
	for(i=1;i>=0;i=i-1){
		for(j=0;j<2;j=j+1){
			array_2[i][j]=i*j;
		}
	}
	i=0;
	printf("18182681");
	while(i<5){
		out=array_1[i];
		print(out);
		i=i+1;
	}
	i=0;
	j=0;
	while(i<2){
		while(j<2){
			out=array_2[i][j];
			print(out);	
			j=j+1;
		}
		i=i+1;
		j=0;
	}
	i=i/2;
	i=i+'a';
}