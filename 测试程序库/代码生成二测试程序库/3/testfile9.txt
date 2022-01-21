
int n = 10;

int good (int a )  {
    int x,y;
    x = a / 10;
    y = a * 10;
    return (x + y + 10);
 }

void main ( )  {
    int x [10 ];
    int temp = 0;
    int i;
    for  (i=0;i<10;i = i + 1 )  {
        scanf(temp);
	x[i] = temp;
     }
    printf ("18231208");
    for  (i=1;i<10;i = i + 1 )  {
        temp = good (x [i ] );
        printf (temp );
     }
    return ;
 }