const int kCon1 = 1024;
const int kCon2 = +18, kCon3 = -5;
const char kCon4 = 'K';

int global_var_uninitialized, global_1darray_uninitialized[6];
int global_var_initialized = 1234;
int global_1darray_initialized[4] = {1, 2, 4, 4};
int global_2darray_initialized[2][2] = {{5, 6}, {7, 8}};

void TestNoReturn(int n) {
    global_var_uninitialized = n;
}

int Fib(int x) {
    if (x == 0) return (1);
    if (x == 1) return (1);
    return (+Fib(x - 1) + Fib(x - 2));
}

char TestChar() {
    const char inner_char = 'c';
    return (inner_char);
}

int TestMulDiv(int a, int b, int c) {
    return (a * (b / c));
}

void main() {
    const int mainCon = 9;
    char c = 'a';
    int n, i;
    printf("18373109");
    TestNoReturn(144);
    printf(global_var_uninitialized);
    scanf(n);
    printf("Fib: ", Fib(n));
    printf(TestChar());
    printf(kCon4);
    printf(TestMulDiv(kCon1, kCon2, mainCon));
    for (i = 0; i < -kCon3 - 2; i = i + 1) {
        if (global_1darray_initialized[i] == i + 1) printf(global_var_initialized);
        else printf(global_2darray_initialized[0][0]);
    }
    scanf(n);
    global_1darray_uninitialized[3] = n;
    printf(global_1darray_uninitialized[3]);
    return;
}
