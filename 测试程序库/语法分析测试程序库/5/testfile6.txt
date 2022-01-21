const int int_a = 0;
const int int_b = -1, int_c = 1, int_d = +1;
const int int_e = 123, int_f = -123;
const char char_a = 'a';
const char char_b = 'b', char_c = 'c';

int int_m;
int int_n = 5;
int int_o = -1;
int s;
int ID = 18182646;

int int_G[10];
int int_H[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int int_I[10];
int int_J[2][2] = {{1, 1}, {2, 2}};
int int_K[2][2], int_L[10][10];

char char_D[2][2] = {{'c', 'd'}, {'e', 'f'}};
char char_E[2][2], char_F[10][10];
char char_G[10] = {'a', 'b', '+', '*', '5', 'F', 'g', 'h', 'i', 'j'};

int Parameter_1(int x) {
    const int y = 1;
    int z = 2;
    return (x + y + z);
}

char Parameter_2(char c) {
    return (c);
}

void Pig_Ma() {
    printf(ID);
}

void main() {
    int flag;
    int temp = 0;
    temp = temp * 1;
    temp = temp / 1;
    scanf(flag);
    if (flag < int_a) {
        flag = 1;
    } else if (flag <= int_a) {
        flag = 2;
    } else if (flag != int_a) {
        flag = 3;
    } else if (flag == int_a) {
        flag = 4;
    } else if (flag > int_a) {
        flag = 5;
    } else if (flag >= int_a) {
        flag = 6;
    } else {
        flag = 0;
    }

    for (flag = 0; flag < 4; flag = flag + 1) {
        int_G[0] = 1;
    }
    for (flag = 4; flag > 0; flag = flag - 1) {
        int_G[0] = 1;
    }

    switch (ID) {
        case 18182646: temp = temp + 1;
        case 0: temp = temp - 1;
        default: temp = 0;
    }

    switch (temp) {
        case 1: temp = temp - 1;
        default: temp = 0;
    }

    while (int_o != 0) {
        int_o = int_o - int_o;
    }

    Pig_Ma();
    printf(Parameter_1(1));
    printf(Parameter_1(1) + 1);
    printf(ID);
    printf(int_f);
    printf(int_f);
    printf(int_f);
    printf(int_f);
    printf(int_f);
    printf(int_f);

}
