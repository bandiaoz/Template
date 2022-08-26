#include <math.h>
#include <stdio.h>
#include <string.h>

#include <algorithm>
#include <iostream>
using namespace std;

const int MAXN = 50;

int a[MAXN][MAXN];  //增广矩阵
int x[MAXN];        //解集
int free_x[MAXN];   //标记是否是不确定的变元

// 高斯消元法解方程组(Gauss-Jordan elimination).(-2表示有浮点数解，但无整数解，
//-1表示无解，0表示唯一解，大于0表示无穷解，并返回自由变元的个数)
//有equ个方程，var个变元。增广矩阵行数为equ,分别为0到equ-1,列数为var+1,分别为0到var.
int Gauss(int equ, int var) {
    int i, j, k;
    int max_r;  // 当前这列绝对值最大的行.
    int col;    //当前处理的列

    for (int i = 0; i <= var; i++) {
        x[i] = 0;
        free_x[i] = 1;
    }

    //转换为阶梯阵.
    col = 0;                                         // 当前处理的列
    for (k = 0; k < equ && col < var; k++, col++) {  // 枚举当前处理的行.
        // 找到该col列元素绝对值最大的那行与第k行交换.(为了在除法时减小误差)
        max_r = k;
        for (i = k + 1; i < equ; i++) {
            if (abs(a[i][col]) > abs(a[max_r][col]))
                max_r = i;
        }
        if (max_r != k) {  // 与第k行交换.
            for (j = k; j < var + 1; j++)
                swap(a[k][j], a[max_r][j]);
        }
        if (a[k][col] == 0) {  // 说明该col列第k行以下全是0了，则处理当前行的下一列.
            k--;
            continue;
        }
        for (i = k + 1; i < equ; i++) {  // 枚举要删去的行.
            if (a[i][col] != 0) {
                for (j = col; j < var + 1; j++)
                    a[i][j] ^= a[k][j];
            }
        }
    }
    // 1. 无解的情况: 化简的增广阵中存在(0, 0, ..., a)这样的行(a != 0).
    for (i = k; i < equ; i++) {  // 对于无穷解来说，如果要判断哪些是自由变元，那么初等行变换中的交换就会影响，则要记录交换.
        if (a[i][col] != 0)
            return -1;
    }
    return var - k;
}

int start[MAXN];
int en[MAXN];

int main() {
    //  freopen("in.txt","r",stdin);
    //  freopen("out.txt","w",stdout);
    int u, v;
    int T;
    int n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++)
            scanf("%d", &start[i]);
        for (int i = 0; i < n; i++)
            scanf("%d", &en[i]);
        memset(a, 0, sizeof(a));
        while (scanf("%d%d", &u, &v)) {
            if (u == 0 && v == 0)
                break;
            a[v - 1][u - 1] = 1;
        }
        for (int i = 0; i < n; i++)
            a[i][i] = 1;
        for (int i = 0; i < n; i++)
            a[i][n] = start[i] ^ en[i];
        int ans = Gauss(n, n);
        if (ans == -1)
            printf("Oh,it's impossible~!!\n");
        else
            printf("%d\n", 1 << ans);
    }
    return 0;
}