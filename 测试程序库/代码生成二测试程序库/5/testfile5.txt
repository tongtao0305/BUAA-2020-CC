const int num = 10;
const char first = 'f', last = 'm';
char cool = 'a';
char cool2 = '+';
char cool3 = '*';
char cool4 = '6';
int num_1 = +1;
int nUM2 = -222;
int num3 = 3;
int arr1[2] = {5,1};
int arr2[2][2] = {{1,2},{3,4}};

int function1 () {
    return (3);
}

char function2 (int a) {
    if (a == 1) {
        return ('b');
    }
    return ('a');
}

void g1 () {
    printf("18377290");
    return;
}

void g2 (int a, int b) {
    if (a < b) {
        printf("a < b");
    }
    if (a <= b) {
        printf("a <= b");
    }
    if (a != b) {
        printf("a != b");
    }
}

void main() {
    int i = 1;
    int aaa[2] = {1,1};
    char ch_ = 'c';
    aaa[0] = 5;
    while (i < 5) {
        i = i + 1;
    }
    for (i = 0; i < 3; i = i + 1) {
        ;
    }
    for (i = 10; i > 5; i = i - 3) {
        ;
    }
    g1();
    g2(aaa[1],function1 ());
    ch_ = function2(2);
    printf("Sammy was low");
    printf("Just watching the show");
    printf("Over and over again");
    printf("Knew it was time");
    printf("Hed made up his mind");
    printf("To leave his dead life behind");
}
