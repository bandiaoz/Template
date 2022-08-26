#include <cmath>
#include <iostream>
using namespace std;
#define eps 1e-9
const int maxn = 5e2 + 5;
double a[maxn][maxn];  //增广矩阵
double x[maxn];        //解集
int n;
int gauss() {
    for (int i = 1; i <= n; i++) {        //遍历每一行
        int max_r = i;                    //找到i+1--n中a[j][i]绝对值最大的一行
        for (int j = i + 1; j <= n; j++)  //遍历i+1至n去寻找
            if (fabs(a[j][i]) > fabs(a[max_r][i]))
                max_r = j;

        for (int k = 1; k <= n + 1; k++)  //将max_r与i交换
            swap(a[max_r][k], a[i][k]);
        if (fabs(a[i][i]) < eps)
            continue;        //若a[i][i]<0,则说明该行全为0,跳过
        double p = a[i][i];  //取出a[i][i]
        for (int j = 1; j <= n + 1; j++)
            a[i][j] /= p;                 //归一化
        for (int j = i + 1; j <= n; j++)  //消元，消掉其余的a[j][i]
        {
            if (i != j) {
                double tmp = a[j][i];
                for (int k = 1; k <= n + 1; k++)
                    a[j][k] -= a[i][k] * tmp;  //套公式a[j][k]=a[j][k]-(a[i][k]/a[k][k])*a[j][i];
            }
        }
    }
    int free_num = 0;  //自由元数量统计
    for (int i = 1; i <= n; i++) {
        int ans = 0;
        for (int j = 1; j <= n + 1; j++)     //统计一行中零元数量
            if (fabs(a[i][j]) < eps) ans++;  //注意精度修正
        if (ans == n && a[i][n + 1])
            return -1;  //若为无解情形
        if (ans == n + 1)
            free_num++;  //无数组解的情形
    }
    if (!free_num) {  //唯一解回带求解
        for (int i = n - 1; i >= 1; i--)
            for (int j = i + 1; j <= n; j++)
                a[i][n + 1] -= a[j][n + 1] * a[i][j];  //从下往上，从右往左
        for (int i = 1; i <= n; i++)
            x[i] = a[i][n + 1];
        return free_num;
    }
}

int main() {
    cin >> n;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n + 1; j++)
            cin >> a[i][j];
    int t = gauss();
    if (t == 0) {
        for (int i = 1; i <= n; i++) {
            if (fabs(x[i]) < eps)
                printf("0\n");
            else
                printf("%.2f\n", x[i]);
        }
    } else
        cout << "No Solution\n";
}