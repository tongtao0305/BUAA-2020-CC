const int MyConstInt = 1;
const char MyConstChar = 'A';

void printMyStudentId() {
    printf("18373105");
    return;
}

int max(int a, int b) {
    int c;
    if (a >= b) {
        c = a;
    } else {
        c = b;
    }
    return (c);
}

void main(){
    char s[3] = {'a', 'b', 'c'};
    int array1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int array2[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    int sum;
    int i, j, k;
    int x, y, z;

    printf("18373105");

    i = 1;
    sum = 0;
    while (i <= 10) {
        sum = sum + i;
        i = i + 1;
    }
    for (i = 11; i <= 20; i = i + 1) {
        sum = sum + i;
    }
    for (i = 30; i > 20; i = i - 1) {
        sum = sum + i;
    }
    printf(sum);

    i = 1;
    j = 10;
    sum = 0;
    if (i < j) sum = sum + 1;
    if (i < j)
        sum = sum + 1;
    else
        sum = sum - 1;
    printf(sum);

    i = 1;
    j = 10;
    sum = max(i, j);
    printf(sum);

    printMyStudentId();

    i = 1;
    array1[0] = 0;
    array2[0][0] = 1;
    sum = i + array1[0] + array2[0][0];
    printf(sum);

    sum = 0;
    scanf(sum);
    printf("sum=", sum);
    printf(sum);

    i = 1;
    j = 10;
    switch (i) {
        case 1:
            sum = 1;
        default:
            sum = 0;
    }
    switch (j) {
        case 10:
            sum = sum + 10 * 10;
        case 20:
            sum = sum + 20 * 20;
        default:
            sum = 0;
    }
    printf(sum);

    ;

    printf("testfile2 ended!");
}