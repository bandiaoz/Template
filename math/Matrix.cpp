#pragma once
#include <iostream>
#include <vector>
#include <cassert>

template <typename T>
struct Matrix {
    std::vector<std::vector<T>> a;
    
    Matrix() {}
    Matrix(int n, int m, T v = T()) { init(n, m, v); }
    Matrix(std::vector<std::vector<T>> a) { init(a); }
    void init(int n, int m, T v) {
        a.assign(n, std::vector<T>(m, v));
    }
    void init(std::vector<std::vector<T>> &a) {
        this->a = a;
    }

    std::vector<T> &operator[](int i) { return a[i]; }
    const std::vector<T> &operator[](int i) const { return a[i]; }
    /**
     * 支持 for (auto row : matrix) 遍历
    */
    auto begin() { return a.begin(); }
    auto end() { return a.end(); }
    auto begin() const { return a.begin(); }
    auto end() const { return a.end(); }
    std::pair<int, int> size() const { return {a.size(), a[0].size()}; }
    Matrix operator+(const Matrix &rhs) const {
        auto [n, m] = this->size();
        Matrix res(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                res[i][j] = a[i][j] + rhs[i][j];
            }
        }
        return res;
    }
    Matrix operator-(const Matrix &rhs) const {
        auto [n, m] = this->size();
        Matrix res(n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                res[i][j] = a[i][j] - rhs[i][j];
            }
        }
        return res;
    }
    Matrix operator*(const Matrix &rhs) const {
        auto [n, m] = this->size();
        int k = rhs.size().second;
        Matrix res(n, k);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k; j++) {
                for (int l = 0; l < m; l++) {
                    res[i][j] += a[i][l] * rhs[l][j];
                }
            }
        }
        return res;
    }
    static Matrix identity(int n) {
        Matrix res(n, n);
        for (int i = 0; i < n; i++) {
            res[i][i] = 1;
        }
        return res;
    }
    Matrix identity() {
        auto [n, m] = this->size();
        assert(n == m);
        return identity(n);
    }
    /**
     * @brief 方阵行列式 calculate the determinant of the matrix
     * @link https://judge.yosupo.jp/problem/matrix_det
     * @return the determinant of the matrix mod prime 
     */
    Z det() {
        auto [n, m] = size();
        assert(n == m);
        Z res = 1;
        for (int i = 0; i < n; i++) {
            int sel = -1;
            for (int j = i; j < n && sel == -1; j++) {
                if (a[j][i] != 0) {
                    sel = j;
                }
            }
            if (sel == -1) return 0;
            std::swap(a[i], a[sel]);
            if (sel != i) res = -res;
            res *= a[i][i];
            auto inv = a[i][i].inv();
            for (int j = i; j < n; j++) {
                a[i][j] *= inv;
            }
            for (int j = i + 1; j < n; j++) {
                for (int k = i + 1; k < n; k++) {
                    a[j][k] += -a[i][k] * a[j][i];
                }
            }
        }
        return res;
    }
    void print(std::ostream &os) const {
        auto [n, m] = size();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                os << a[i][j] << " \n"[j == m - 1];
            }
        }
    }
};