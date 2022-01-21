const int con1 = 1;
const char con2 = '1';
int var1 = 1;
int var2, vars1[1], vars2[2][1];
char var3 = '1';
char var4;

int fun1(int p) {
	return (-p);
}

void fun2(){
	var2 = var2 + var1 - var3 / 7 + ('_' + fun1((con2)));	
	vars2[0][0] = + var2 - var1 * con1;
	return ;
}  

void main() {
	int i = 0;
	int j = 8;
	int step = 1;
	int a = 1;
	int ii;
	printf("18373677");
	
	while (i < j) {
		for (ii = 0; ii <= i; ii = ii + 1) {
			if (ii - 4 > 0) {
				printf("ii is: ", ii);
			} 
		}
		if (i == 7) {
			printf("a is: ", a);
		} else {
			a = a + 1;
		}
		
		i = i + step;
	}
	
	for (i = j; i >= 0; i = i - 1) {
		a = a - 1;
	}
	
	if (a != 0) {
		var2 = 0;
	} else 
		;
	
	fun2();
	printf(var2);
	printf(fun1(vars2[0][0]));
}