const int MAXN = +10007;
char _ = '*';int par[10007];
int findpar(int p) {
	if (par[p] != p) {
		par[p] = findpar(par[p]);
	}
	return (par[p]);
}

char getcc() {return ('0');}

void hello() {
	int aaa;
	printf("HelloWorld", +114514);
	return;
}

void hellobye (int a1, int a2, int a3){}

void test() {const int mod = 107;char aaaa = '+'; char bbb = 'a';printf("a function with no return;");}

void main() {
	int n, m;int i;int z, x, y;char c;
	printf("18373142");
	scanf(n);
	scanf(m);
	hellobye(1, 2, 3);
	hello();
	while(0 < 0);
	while(0 >= 1);
	while(0 != 0);
	if (0 == 1);
	{}
	for (i = 1 / 1; i <= n; i = i + 1) {
		par[i] = i;
	}
	for (i = m * 1; i > 0; i = i - 1)
	{
		
		scanf(z); scanf(x); scanf(y);
		if (z == 1) {
			par[findpar(y)] = findpar(x);
		}
		else { 
			
			if (findpar(x) == findpar(y)) c = 'Y';
			else c = 'N';
			printf(c);
		}
	}
}