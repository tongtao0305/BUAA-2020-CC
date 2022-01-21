int func_switch_ch() {
    char ch;
    scanf(ch);
    switch (ch)
    {
        case 'm':
            printf("Got m");
        default:{
            printf("Got else");
        }
    }
    return (0);
}

int func_switch_int() {
    int tmp;
    scanf(tmp);
    switch (tmp)
    {
        case 1:
            printf("iGot 1");
        case 2:
            ;
        case 3: {}
        default:{
            printf("iGot else");
        }
    }
    return (0);
}

void main() {
    printf("18373050");
    func_switch_ch();
    func_switch_ch();
    func_switch_int();
    func_switch_int();
    func_switch_int();
    func_switch_int();
    printf("--------------------");

    printf("ylsyyds! yls 3x speed!");
    printf("lls,yyds! xdd,yyds! gtc,yyds!");
    printf("Cjbbbbbbbbbbbbbbbbbbb! --- The master encourager!");
    printf("GoodLuck, may the encourager be with you!(");

    return;
}