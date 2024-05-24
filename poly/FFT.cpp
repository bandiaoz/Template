#include <bits/stdc++.h>

using cd = std::complex<double>;

static void _fft(std::vector<cd> &a, bool invert) {
    int n = a.size();
    // permute the array to do in-place calculation
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * M_PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) {
        for (auto &x : a) x /= n;
    }
}

/**
 * @brief Convolution of two polynomials
 * @param a: coefficients of the first polynomial *from low to high*
 * @param b: coefficients of the second polynomial *from low to high*
 * @return coefficients of the convolution
 * @link https://www.luogu.com.cn/problem/P3803
 */
static std::vector<int> convolve(const std::vector<int> &a, const std::vector<int> &b) {
    std::vector<cd> fa(a.begin(), a.end());
    std::vector<cd> fb(b.begin(), b.end());
    int n = 1 << (std::__lg(a.size() + b.size() - 1) + 1);
    fa.resize(n);
    fb.resize(n);

    _fft(fa, false);
    _fft(fb, false);
    for (int i = 0; i < n; i++) {
        fa[i] *= fb[i];
    }
    _fft(fa, true);

    std::vector<int> result(n);
    for (int i = 0; i < n; i++) {
        result[i] = std::round(fa[i].real());
    }
    return result;
}