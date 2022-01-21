	const int ass=1;
	int chinese,math,eglish;
	char level;
	int n;
	int s;
	int add[2][3]={{1,2,3},{4,5,6}};
	int i=1;

int fenshu(int a,int b,int c){
	return (a*5+b*3+c*2);
}
void main(){

	scanf(n);
	;
	printf("18182653");
	while(i<=n){
		scanf(chinese);
		scanf(math);
		scanf(eglish);
		s=fenshu(chinese,math,eglish);
		if(s<60){
				if(s==0)
				{
					level='0';
			}
				else{
				   level='D';
				}
		}
		else{
			if(s<80){
				level='C';
			}
			else{
				if(s<90)
				level='B';
				else
				level='A';
			}
		}
		
		switch (level){
			case 'A' :{ printf("the level is ");}
			case 'B' :{ printf("the level is ");}
			case 'C' :{ printf("the level is ");}
			case 'D' :{ printf("the level is ");}
			case '0' :{ printf("the level is ");}
			default: { }
			}
		
		switch (level){
			case 'D' :{ printf("it's ,the number is ",i);}
			default : printf("it's ,the number is ",i);
		}
		i=i+1;
	}
	printf("end");
	
}