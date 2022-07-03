#include <bits/stdc++.h>
using namespace std;

//已知六条边计算四面体体积
double culV(double p, double q, double r, double n, double m, double l)
{
    double tmp1 = (p * p + q * q - n * n) / 2;
    double tmp2 = (p * p + r * r - m * m) / 2;
    double tmp3 = (q * q + r * r - l * l) / 2;

    double M1 = q * q * r * r - tmp3 * tmp3;
    double M2 = r * r * tmp1 - tmp2 * tmp3;
    double M3 = tmp1 * tmp3 - q * q * tmp2;

    double V = sqrt(p * p * M1 - tmp1 * M2 + tmp2 * M3);
    V /= 6.0;
    printf("%.4lf\n", V);
}
