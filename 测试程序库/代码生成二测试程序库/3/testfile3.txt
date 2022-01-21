const char f = '_';
int a = 1;
int count(int abc) {
	int i = 10;
	if (abc > i){
		abc = abc + i;
	}
	if (abc < i){
		abc = abc - i;
	}
	if (abc != i){
		i = 0;
	}
	return (abc);
}
void main(){
    int abc;
    scanf(abc);
    printf("18231133");
    printf(abc);
    printf(count(abc));
    printf(f);
    printf(a);
    printf("a");
    printf("b");
    printf("c");
    printf("d");
    printf("e");
}