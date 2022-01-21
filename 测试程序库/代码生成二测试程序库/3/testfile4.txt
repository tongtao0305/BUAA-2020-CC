const int c1 = +11, c = -22;
int var1 = 1;
int var2 = -2;

void main() {
	char ch;
	printf("18373677");
	while (var1 < 7) {
		printf(var1);
		var1 = var1 + 2;
	}
	for (var2 = 0; var2 <= 9; var2 = var2 + 3) {
		if (var2 > 3) {
			var1 = var1 + c1;
		}
	}
	printf(var1);
	printf("const c is ", c);
	printf("var1 plus c is ", var1 + c);
	ch = '_';
	printf(ch);
	printf(c + ch);
	printf("__________end__________");
}