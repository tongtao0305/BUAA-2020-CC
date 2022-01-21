int BigIntarr[2][100];
int BigIntLength[2];
int Result[200];

int min(int a, int b) {
	if (a > b) {
		return(b);
	}
	else {
		return(a);
	}
}

int max(int a, int b) {
	if (a > b) {
		return(a);
	}
	else {
		return(b);
	}
}

int compare() {
	int i = 0;
	if (BigIntLength[0] > BigIntLength[1]) {
		return(1);
	}
	if (BigIntLength[0] < BigIntLength[1]) {
		return (-1);
	}
	while (i < BigIntLength[0]) {
		if (BigIntarr[0][i] > BigIntarr[1][i]) {
			return(1);
		}
		if (BigIntarr[0][i] < BigIntarr[1][i]){
			return (-1);
		}
		i = i + 1;
	}
	return(0);
}

int subtract() {
	int a[100];
	int b[100];
	int c[100];
	int i, j, k;
	int flag;
	int l1, l2;
	l1 = BigIntLength[0];
	l2 = BigIntLength[1];
	k = min(l1, l2);
	j = max(l1, l2);
	if (compare() >= 0) {
		flag = 0;
		i = 0;
		while (i < l1) {
			a[i] = BigIntarr[0][l1 - 1 - i];
			i = i + 1;
		}
		i = 0;
		while (i < l2) {
			b[i] = BigIntarr[1][l2 - 1 - i];
			i = i + 1;
		}
	}
	else {
		flag = 1;
		i = 0;
		while (i < l2) {
			a[i] = BigIntarr[1][l2 - 1 - i];
			i = i + 1;
		}
		i = 0;
		while (i < l1) {
			b[i] = BigIntarr[0][l1 - 1 - i];
			i = i + 1;
		}
	}
	i = 0;
	while (i < k) {
		if (a[i] >= b[i]) {
			c[i] = a[i] - b[i];
		}
		else {
			c[i] = a[i] + 10 - b[i];
			a[i + 1] = a[i + 1] - 1;
		}
		i = i + 1;
	}
	while (i < j) {
		if (a[i] >= 0) { c[i] = a[i]; }
		else {
			c[i] = a[i] + 10;
			a[i + 1] = a[i + 1] - 1;
		}
		i = i + 1;
	}
	while (c[i - 1] == 0) i = i - 1;
	j = 0;
	if (flag == 1)
		printf("-");
	k = i - 1;
	while (k > -1) {
		Result[j] = c[k];
		k = k - 1;
		j = j + 1;
	}
	return(j);
}

void main()
{
	int n;
	int i = 0;

	int tmp;
	scanf(n);
	BigIntLength[0] = n;
	while(i<n){
		scanf(tmp);
		BigIntarr[0][i] = tmp;
		i = i + 1;
	}

	scanf(n);
	BigIntLength[1] = n;
	i = 0;
	while(i<n){
		scanf(tmp);
		BigIntarr[1][i] = tmp;
		i = i + 1;
	}

	i = 0;
	n = subtract();
	while (i < n) {
        printf(Result[i]);
        i = i + 1;
	}
	return;
}