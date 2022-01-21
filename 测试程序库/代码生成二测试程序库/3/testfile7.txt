int test1(int a, int b)
{
    if (a >= b)
        a = b;
    if (a != b)
        a = a + 1;
    if (a == b)
        ;
    return(a);
}

char test2(char c)
{
    return (c);
}

void main()
{
    int i = 1;
    int a1 = 18;
    int a2 = 19;
    int d;
    printf("18182643");
    for (i=1;i<3;i=i+1)
        printf("+test");
    for (i=1;i>=0;i=i-1)
        printf("-test");
    d = test1(a1, a2);
    printf(d);
    printf("18182643");
    printf("18182643");
    printf("18182643");
    printf("18182643");
}