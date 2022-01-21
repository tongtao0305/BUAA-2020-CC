	const int a = 1, a1 = +1, a2 = -1;
	const char ch_a = '_';
	const char ch_b = '*', ch_c = 'A';
	int text_a, text_b, text_c;
	char ch_1,ch_2,ch_3;
	int b = 1024;
	int arry_a[3];
	int arry_1[10] = {1,1,1,1,1,1,1,1,1,1};
	void  print_num(){
		const int stu_num = 18182651;
		printf(stu_num);
		return;
	}
	void print_char(char p1, int p2){
		printf("print_char");
	}
	int num(int p1, int p2, int p3){
		int r;
		printf("num");
		r = p1;
		return(p1);
	}
	char word(char p1, char p2, char p3) {
    	char r;
    	printf("word");
    	r = p3;
    	return (r);
	}
	
	void main(){
		printf("18182651");
		ch_1 = ch_a;
		ch_2 = ch_b;
		arry_a[0] = a;
		arry_a[1] = a1;
		arry_a[2] = a2; 
		printf("ch_1",ch_1);
		printf(arry_a[2]);
		printf(arry_1[9]);
		
		print_num();
		print_char(ch_1,b);
		text_a = num(a,a1,a2);
		ch_3 = word(ch_a,ch_b,ch_c);
		
		printf(text_a);
		printf(ch_3);
		
			
	}




