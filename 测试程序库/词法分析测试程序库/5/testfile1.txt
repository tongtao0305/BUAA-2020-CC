

const int lqw = +917, hjf = -924;
const char A = 'a';



int bArray[6];
int cArrays[10][100];



int d = 1;
int eArray[3] = {6,7,8};
int fArray[2][5] = {{0,1,2,3,4},{9,8,7,6,5}};



int func1(){
    const int f_a = 999;
    int f_b = 666;
    while(f_b < f_a) {
        f_b = f_b + 2;
    }
    return (f_b);
}
char func2(int p){
    printf(-p);
    return ('a');
}

int func3(char p1, int p2, char p3){
    if (+p1 + p2 + p3 <= 100) {
        printf("p1 + p2 + p3 = ", p1 + p2 + p3);
    }
    return (p1 + p2 + p3);
}

void main()
{
    int i;
    printf("17005014");
    printf(d);
    printf(A);

    for(i = 0; i < 6; i = i + 1) {
        bArray[i] = i;
        printf(bArray[i]);
    }

    for (d = 0; d < 3; d = d + 1) {
        printf(eArray[d]);
    }
    for(d = 0; d > -3; d = d - 1) {

    }
    printf(d + hjf);
    if (d >= 0) {

    }
    else fArray[1][4] = 10;

    for (d = 0; d < 2; d = d + 1) {
        for (i = 0 ; i < 5; i = i + 1)
        printf(fArray[d][i]);
    }

    printf(func1() + func2(668) + func3('2', -80, '1') * lqw);
}
