const int i = 1, ia = -1, ib = +1;
const char c1 = '+';
const char c2 = '*', c3 = 'a';
const char c4 = '1';

int arr1[2];
int arr2[2][2];
int i1;
char c5 = 'Z';
char c6, c7;
int i2 = 3;
int arr3[2] = {1, 2};
int arr4[2][2] = {{1, 2}, {1, 2}};

void fun1()
{
    const int i3 = 5;
    int i4;
    int i5;
    i4 = i3;

    for (i4 = 0; i4 < 5; i4 = i4 + 1)
    {
        if (i4 * 2 == 0)
            printf(i4);
        else
        {
            printf("string1");
            printf("string2", i3);
        }
        if (i4 <= 0)
        {
            scanf(i5);
        }
    }
    for (i4 = 5; i4 >= 0; i4 = i4 - 1)
    {
        i5 = i5 + 1 - 2;
    }
}

int fun2(int aa)
{
    int t;
    t = aa;
    while (t != 0)
    {
        if (t > 0)
        {
            t = +t - 1 * 1;
        } else
        {
            t = -t + 1 * 1;
        }
    }

    return (t);
}

void fun3(int in1, int in2)
{
    int zero;
    char ch1;
    zero = 0;
    ch1 = '_';
    switch(in1)
    {
    case 1:;
    default:;
    }

    switch(in2)
    {
    case 1:;
    case 2:;
    default:;
    }
    fun1();
    zero = fun2(in1);
}

void fun4(){}

void main()
{
    int arr3[3] = {1, 2, 3};
    int arr4[2][3] = {{1, 2, 3}, {2, 4, 6}};
    arr3[0] = arr4[0][1];
    arr4[0][2] = arr3[1];

    printf("18377418");
    fun3(1, 3);

    return;
}