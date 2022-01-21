const int test_const_int1 = 1,test_const_int2 = 22;
int test_var_int1,test_var_int2;
int test_var_char1;
int var_a;
int var_b;
int var_c;
int array_a[2];
int i;
void main(){
printf("18182683");
var_b=1;
var_a=-1;
var_c=1;
array_a[1]=12;
var_a = var_b + var_b;
var_a = var_b / var_b;
var_c = var_a * var_b;

while(var_b<var_c){
    var_c = var_c -1;
}



var_c=100;
var_b=1;

if(var_c != var_b)
    printf("c != b,",var_b);

for (test_var_int1 = -2;test_var_int1 <= 0; test_var_int1 = test_var_int1 +1){
    i=i+1;
    printf ("test_var_int1 <=0");
}
i=0;
for (test_var_int2 = 3; test_var_int2 >= 0; test_var_int2 =test_var_int2 - 1){
    i=i+1;
    printf ("test_var_int2 >=0");
}
printf(test_var_int2);
}