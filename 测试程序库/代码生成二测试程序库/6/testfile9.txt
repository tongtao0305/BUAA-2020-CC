const int test_const_int1 = 1,test_const_int2 = 22;
const char test_const_char1 = 'a',test_const_char2 = 'b',test_const_char3 = '*',test_const_char4 = '+',test_const_char5 = '1';
const int postive = +1, negative = -1, bit = 333;
int test_var_int1,test_var_int2;
int test_var_char1;
int var_a;
int var_b;
int var_c=1;
int var_1;
int i;
int array_a[2],array_c[33];
int array_d[2]={1,2};
int array_b[2][2]={{1,1},{1,2}};
char  charr(char var_y){
    return ('A');
}
int intt(int t,int tt){
    const int aa = 1;
    return(11);
}
int inttt(int t){
    return(t+1);
}
 void inttttt(){
    return;
}
void voiddd(){

}

void main(){
    int b = 1;
    char BB = '1';
printf("18182683");
var_b=+1;
var_a=-1;
var_c=1;
array_a[1]=1;
array_b[1][1]=1;
var_a = -var_b + var_b + var_a;
var_a = -var_b / var_b;
var_c = var_a * var_b;

while(var_b<var_c){
    var_c = (var_c) -1;
}

intt(var_b,var_a);
voiddd();
inttt(var_a);

scanf(test_var_int2);
i=0;
for (test_var_int1 = -2;test_var_int1 <= 0; test_var_int1 = test_var_int1 +1){
    i=i+1;
    printf ("test_var_int1 <=0");
}
i=0;
for (test_var_int2 = 4; test_var_int2 >= 0; test_var_int2 =test_var_int2 - 1){
    i=i+1;
    printf ("test_var_int2 >=0");
}
 

switch(test_var_int1){
        case 1: test_var_int1 = test_var_int1 *2;
        case 2:test_var_int2 = test_var_int2 /2;
        default:test_var_int2 = test_var_int2 /2;
    }
switch(test_var_int1){
        case 1: test_var_int1 = test_var_int1 *2;
        default:test_var_int2 = test_var_int2 /2;
    }
if(var_b <= var_a){

}
else
    test_var_int1 = var_b;
if(var_b >= var_a){

}
else
    test_var_int1 = var_b;
var_c = inttt(var_c);
var_c = array_a[1]+array_b[1][1];
var_c=100;
var_b=1;

if(var_c == var_b)
    test_var_int1 = var_b;
if(var_c >var_b)
    test_var_int1 = var_b;
if(var_c != var_b)
    printf("c != b");
test_var_int1=1;
}