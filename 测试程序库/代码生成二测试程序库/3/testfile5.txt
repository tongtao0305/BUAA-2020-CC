const int N = 10;

int g[10][10] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

void main() {
    int i = 0;
    int j = -1;
    int cnt = 0;
    int NN = 100;
    int ok;
    printf("18373806");
    while (cnt < NN) {
        ok = 0;
        while (ok == 0) {
            if (j + 1 >= N) {
                ok = 1;
            } else if (g[i][j + 1] != 0) {
                ok = 1;
            }
            if (ok == 0) {
                j = j + 1;
                cnt = cnt + 1;
                g[i][j] = cnt;
            }
        }
        ok = 0;
        while (ok == 0) {
            if (i + 1 >= N) {
                ok = 1;
            } else if (g[i + 1][j] != 0) {
                ok = 1;
            }
            if (ok == 0) {
                i = i + 1;
                cnt = cnt + 1;
                g[i][j] = cnt;
            }
        }
        ok = 0;
        while (ok == 0) {
            if (j - 1 < 0) {
                ok = 1;
            } else if (g[i][j - 1] != 0) {
                ok = 1;
            }
            if (ok == 0) {
                j = j - 1;
                cnt = cnt + 1;
                g[i][j] = cnt;
            }
        }
        ok = 0;
        while (ok == 0) {
            if (i - 1 < 0) {
                ok = 1;
            } else if (g[i - 1][j] != 0) {
                ok = 1;
            }
            if (ok == 0) {
                i = i - 1;
                cnt = cnt + 1;
                g[i][j] = cnt;
            }
        }
    }
    printf("g12, g63, g24, g46, g75, g87, g98, g51");
    printf(g[1][2]);
    printf(g[6][3]);
    printf(g[2][4]);
    printf(g[4][6]);
    printf(g[7][5]);
    printf(g[8][7]);
    printf(g[9][8]);
    printf(g[5][1]);
}