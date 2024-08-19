/*
最后修改:
20231127
测试环境:
gcc11.2,c++11
clang12.0,C++11
msvc14.2,C++14
*/
#ifndef __OY_STATICMATRIX__
#define __OY_STATICMATRIX__

#include <algorithm>
#include <cstdint>
#include <iostream>

/**
 * @brief 静态矩阵
 * @example OY::StaticMatrix<elem, 2, 3> a{{{1, 2, 3}, {4, 5, 6}}};
 */
namespace OY {
    template <typename Tp, uint32_t M, uint32_t N>
    struct StaticMatrix {
        Tp m_val[M][N];
        static constexpr uint32_t row() { return M; }
        static constexpr uint32_t column() { return N; }
        /**
         * @brief 返回一个所有元素全为 a 的矩阵
         */
        static StaticMatrix<Tp, M, N> raw(Tp a) {
            StaticMatrix<Tp, M, N> res;
            for (uint32_t i = 0; i != M; i++)
                for (uint32_t j = 0; j != N; j++) res[i][j] = a;
            return res;
        }
        static StaticMatrix<Tp, M, M> identity() {
            static_assert(M == N, "Row Not Equals to Column");
            StaticMatrix<Tp, M, N> res{};
            for (uint32_t i = 0; i != M; i++) res[i][i] = 1;
            return res;
        }
        Tp *operator[](uint32_t i) { return m_val[i]; }
        const Tp *operator[](uint32_t i) const { return m_val[i]; }
        Tp *begin() { return m_val[0]; }
        Tp *end() { return m_val[0] + M * N; }
        const Tp *begin() const { return m_val[0]; }
        const Tp *end() const { return m_val[0] + M * N; }
        StaticMatrix<Tp, M, N> &operator+=(Tp a) {
            for (uint32_t i = 0; i != M; i++)
                for (uint32_t j = 0; j != N; j++) m_val[i][j] += a;
            return *this;
        }
        StaticMatrix<Tp, M, N> &operator-=(Tp a) {
            for (uint32_t i = 0; i != M; i++)
                for (uint32_t j = 0; j != N; j++) m_val[i][j] -= a;
            return *this;
        }
        StaticMatrix<Tp, M, N> &operator*=(Tp a) {
            for (uint32_t i = 0; i != M; i++)
                for (uint32_t j = 0; j != N; j++) m_val[i][j] *= a;
            return *this;
        }
        StaticMatrix<Tp, M, N> &operator+=(const StaticMatrix<Tp, M, N> &rhs) {
            for (uint32_t i = 0; i != M; i++)
                for (uint32_t j = 0; j != N; j++) m_val[i][j] += rhs[i][j];
            return *this;
        }
        StaticMatrix<Tp, M, N> &operator-=(const StaticMatrix<Tp, M, N> &rhs) {
            for (uint32_t i = 0; i != M; i++)
                for (uint32_t j = 0; j != N; j++) m_val[i][j] -= rhs[i][j];
            return *this;
        }
        StaticMatrix<Tp, M, M> pow(uint64_t n) const {
            static_assert(M == N, "Row Not Equals to Column");
            auto res = identity(), a = *this;
            while (n) {
                if (n & 1) res = res * a;
                if (n >>= 1) a = a * a;
            }
            return res;
        }
        template <typename Fp>
        friend StaticMatrix<Tp, M, N> operator+(const StaticMatrix<Tp, M, N> &a, const Fp &b) { return StaticMatrix<Tp, M, N>(a) += b; }
        template <typename Fp>
        friend StaticMatrix<Tp, M, N> operator-(const StaticMatrix<Tp, M, N> &a, const Fp &b) { return StaticMatrix<Tp, M, N>(a) -= b; }
        friend StaticMatrix<Tp, M, N> operator*(const StaticMatrix<Tp, M, N> &a, const Tp &b) { return StaticMatrix<Tp, M, N>(a) *= b; }
        template <uint32_t L>
        friend StaticMatrix<Tp, M, L> operator*(const StaticMatrix<Tp, M, N> &a, const StaticMatrix<Tp, N, L> &b) {
            StaticMatrix<Tp, M, L> res{};
            for (uint32_t i = 0; i != M; i++)
                for (uint32_t j = 0; j != N; j++) {
                    Tp x = a.m_val[i][j];
                    for (uint32_t k = 0; k != L; k++) res[i][k] += x * b[j][k];
                }
            return res;
        }
        friend bool operator==(const StaticMatrix<Tp, M, N> &a, const StaticMatrix<Tp, M, N> &b) {
            for (uint32_t i = 0; i != M; i++)
                for (uint32_t j = 0; j != N; j++)
                    if (a[i][j] != b[i][j]) return false;
            return true;
        }
        friend bool operator!=(const StaticMatrix<Tp, M, N> &a, const StaticMatrix<Tp, M, N> &b) {
            for (uint32_t i = 0; i != M; i++)
                for (uint32_t j = 0; j != N; j++)
                    if (a[i][j] != b[i][j]) return true;
            return false;
        }
    };
    template <typename Tp, uint32_t M, uint32_t N>
    std::ostream &operator<<(std::ostream &os, const StaticMatrix<Tp, M, N> &a) {
        for (uint32_t i = 0; i != M; i++) {
            for (uint32_t j = 0; j != N; j++) os << a[i][j] << ' ';
            os << '\n';
        }
        return os;
    }
};

#endif