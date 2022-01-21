const int a=0 ,b=3;
const int c=5;
int d1[4],d2[4];
char s1[2][4];
int d3[4]={+1,+2,+3,+4};
char s2[2][4]={{'a','b','c','d'},{'0','1','2','3'}};
void main(){
	const int k=-10;
	int i,j;
	printf("18373165");
	d1[0]=0;
	s1[0][0]='+';
	s1[0][1]='*';
	for(i=0;i<=3;i=i+1){
		j=0;
		s1[0][i]='A';
		while(j<=3){
			if(j>i){
				if(i+j<d3[3])
				printf(s2[0][j]);
				else
				printf(s2[1][j]);
			}
			j=j+1;
		}
	if(i>0)
	printf(i+k);
	else{}
	}
	for(i=0;i<=2;i=i+1);
}