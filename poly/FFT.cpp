#include <bits/stdc++.h>
using namespace std;
#define PI acos(-1.0)
const int maxn = 5e5 + 5;
const int INF = 0x3f3f3f3f;
const int MOD = 1e9 + 7;
struct Complex {//复数结构体
    double r, i;
    Complex(double _r = 0.0, double _i = 0.0) { r = _r, i = _i; }
    Complex operator+(const Complex &b) { return Complex(r + b.r, i + b.i); }
    Complex operator-(const Complex &b) { return Complex(r - b.r, i - b.i); }
    Complex operator*(const Complex &b) { return Complex(r * b.r - i * b.i, r * b.i + i * b.r); }
};
/*
 * 进行FFT和IFFT前的反转变换。
 * 位置i和 （i二进制反转后位置）互换
 * len必须去2的幂
 */
/*
 做FFT
 len必须为2^k形式，
 on==1时是DFT，on==-1时是IDFT
 */
int rev[maxn];
void FFT(Complex y[], int len, int on) {
    int bit = 0;
    while ((1 << bit) < len)
        bit++;
    for (int i = 0; i <= len - 1; i++) {  //对每一位y处理出递归结束后的位置,然后进行交换
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (bit - 1));
        if (i < rev[i]) swap(y[i], y[rev[i]]);  //不加这条if会交换两次（就是没交换）
    }
    for (int h = 2; h <= len; h <<= 1) {                           //h为合并后的区间长度
        Complex wn(cos(-on * 2 * PI / h), sin(-on * 2 * PI / h));  //单位根
        for (int j = 0; j < len; j += h) {                         //遍历每个区间的开端
            Complex w(1, 0);
            for (int k = j; k < j + h / 2; k++) {  //对小区间进行计算
                Complex u = y[k];
                Complex t = w * y[k + h / 2];
                y[k] = u + t;          //前半区间
                y[k + h / 2] = u - t;  //后半区间
                w = w * wn;
            }
        }
    }
    if (on == -1)
        for (int i = 0; i < len; i++)
            y[i].r /= len;
}

char s1[maxn], s2[maxn];
int ans[maxn];
Complex a[maxn], b[maxn];
int main() {
    int i, len1, len2, len;
    while (~scanf("%s%s", s1, s2)) {
        len1 = strlen(s1);
        len2 = strlen(s2);
        len = 1;
        while (len < (len1 << 1) || len < (len2 << 1))
            len <<= 1;
        for (i = 0; i < len1; i++)
            a[i] = Complex(s1[len1 - i - 1] - '0', 0);
        for (; i < len; i++)
            a[i] = Complex(0, 0);
        for (i = 0; i < len2; i++)
            b[i] = Complex(s2[len2 - i - 1] - '0', 0);
        for (; i < len; i++)
            b[i] = Complex(0, 0);
        FFT(a, len, 1);
        FFT(b, len, 1);
        for (i = 0; i < len; i++)
            a[i] = a[i] * b[i];
        FFT(a, len, -1);
        for (i = 0; i < len; i++)
            ans[i] = (int)(a[i].r + 0.5);
        len = len1 + len2 - 1;
        for (i = 0; i < len; i++) {
            ans[i + 1] += ans[i] / 10;
            ans[i] %= 10;
        }
        for (i = len; ans[i] <= 0 && i > 0; i--)
            ;
        for (; i >= 0; i--)
            printf("%d", ans[i]);
        putchar('\n');
    }
}