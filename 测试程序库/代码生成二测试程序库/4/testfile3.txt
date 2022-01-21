const int global_c_a = 1;
const int global_c_b = 2;
int global_v_c;
char global_v_d;

void func_a(){
    global_v_c = global_c_a + global_c_b;
    return;
}
void func_b(){
    global_v_c = +4;
}
void func_c(){
}
char func_d(){
    return ('a');
}
int func_e(int func_v_a){
    return (func_v_a);
}
int func_f(int func_v_a, int func_v_b){
    return (func_v_a + func_v_b);
}
void main(){
    int a , B;
    a = 1;
    printf("18373674");
    func_a();
    if (global_v_c == 3) printf("test_unrfunc:",1);
    else printf("test_unrfunc:",0);
    func_b();
    if (global_v_c == 4) printf("test_unrfunc:",1);
    else printf("test_unrfunc:",0);
    global_v_d = func_d();
    printf("test_rfunc:",global_v_d);
    global_v_c = func_e(global_c_a);
    if (global_v_c == 1) printf("test_rfunc:",1);
    else printf("test_rfunc:",0);
    global_v_c = func_f(global_c_a, global_c_b);
    if (global_v_c == 3) printf("test_rfunc:",1);
    else printf("test_rfunc:",0);
    switch(global_c_a){
        case 0: printf("test_switch:",0);
        default: printf("test_switch:",1);
    }
    switch(global_c_a){
        case 0: printf("test_switch:",0);
        case 1: printf("test_switch:",1);
        default: printf("test_switch:",0);
    }
    global_v_c = 1;
    while (global_v_c<=10) global_v_c = global_v_c+1;
    if (global_v_c==11) printf("test_while:",1);
    else printf("test_while:",0);
    printf(a);
}