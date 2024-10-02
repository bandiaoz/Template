#pragma once

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <numeric>

/**
 * @brief 高斯消元
 * @example OY::GaussJordanElimination<double, 100, 100> GE(unknown, equation);
 * @param `MAX_UNKNOWN` 未知数的最大数量，默认为100
 * @param `MAX_EQUATION` 方程的最大数量，默认为100
 * @param `unknown` 未知数的数量
 * @param `equation` 方程的数量
 * @note GaussJordanXorElimination 用于异或方程组，使用 std::bitset 加速
 */
namespace OY {
    template <typename Tp>
    struct GaussJordanGetBigger {
        bool operator()(Tp x, Tp y) const { return (x >= 0 ? x : -x) > (y >= 0 ? y : -y); }
    };
    template <typename Tp>
    struct GaussJordanIsZero {
        bool operator()(Tp x) const { return x == Tp(0); }
    };
    template <typename Tp, uint32_t MAX_UNKNOWN = 100, uint32_t MAX_EQUATION = 100>
    class GaussJordanElimination {
        uint32_t m_unknown, m_equation, m_index[MAX_UNKNOWN], m_base_cnt;
        Tp m_coefs[MAX_EQUATION][MAX_UNKNOWN + 1], m_x[MAX_UNKNOWN];
    public:
        GaussJordanElimination(uint32_t unknown, uint32_t equation) : m_unknown(unknown), m_equation(equation), m_index{}, m_base_cnt(0), m_coefs{}, m_x{} {}
        Tp &coef(uint32_t equation_ID, uint32_t unknown_ID) { return m_coefs[equation_ID][unknown_ID]; }
        Tp coef(uint32_t equation_ID, uint32_t unknown_ID) const { return m_coefs[equation_ID][unknown_ID]; }
        Tp &result(uint32_t equation_ID) { return m_coefs[equation_ID][m_unknown]; }
        Tp result(uint32_t equation_ID) const { return m_coefs[equation_ID][m_unknown]; }
        /**
         * @brief 设置第 `equation_ID` 个方程，`equation` 的长度为未知数数量加一
         */
        void set_equation(uint32_t equation_ID, std::initializer_list<Tp> equation) { std::copy(equation.begin(), equation.end(), m_coefs[equation_ID]); }
        template <typename GetBigger = GaussJordanGetBigger<Tp>, typename IsZero = GaussJordanIsZero<Tp>>
        /**
         * @brief 计算方程组，返回是否有解
         * @param `get_bigger` 比较两个数的大小，用于选取主元
         * @param `is_zero` 判断一个数是否为零
         */
        bool calc(GetBigger get_bigger = GetBigger(), IsZero is_zero = IsZero()) {
            for (uint32_t i = 0; i != m_unknown; i++) {
                uint32_t index = m_base_cnt;
                for (uint32_t j = m_base_cnt + 1; j < m_equation; j++)
                    if (get_bigger(m_coefs[j][i], m_coefs[index][i])) index = j;
                if (!is_zero(m_coefs[index][i])) {
                    if (index != m_base_cnt)
                        for (uint32_t j = 0; j <= m_unknown; j++) std::swap(m_coefs[index][j], m_coefs[m_base_cnt][j]);
                    Tp inv = Tp(1) / m_coefs[m_base_cnt][i];
                    for (uint32_t j = i; j <= m_unknown; j++) m_coefs[m_base_cnt][j] *= inv;
                    for (uint32_t j = 0; j != m_equation; j++)
                        if (j != m_base_cnt && m_coefs[j][i])
                            for (uint32_t k = i + 1; k <= m_unknown; k++) m_coefs[j][k] -= m_coefs[j][i] * m_coefs[m_base_cnt][k];
                    m_index[m_base_cnt++] = i;
                } else
                    m_x[i] = 0;
            }
            for (uint32_t i = m_base_cnt; i < m_equation; i++)
                if (!is_zero(m_coefs[i][m_unknown])) return false;
            for (uint32_t i = 0; i < m_base_cnt; i++) m_x[m_index[i]] = m_coefs[i][m_unknown];
            return true;
        }
        bool has_multi_solution() const { return m_base_cnt < m_unknown; }
        Tp get_solution(uint32_t unknown_ID) const { return m_x[unknown_ID]; }
    };
    template <uint32_t MAX_UNKNOWN = 1000, uint32_t MAX_EQUATION = 1000>
    class GaussJordanXorElimination {
        uint32_t m_unknown, m_equation, m_base_cnt, m_index[MAX_UNKNOWN];
        std::bitset<MAX_UNKNOWN + 1> m_coefs[MAX_EQUATION], m_x;
    public:
        GaussJordanXorElimination(uint32_t unknown, uint32_t equation) : m_unknown(unknown), m_equation(equation), m_base_cnt(0) {}
        typename std::bitset<MAX_UNKNOWN + 1>::reference coef(uint32_t equation_ID, uint32_t unknown_ID) { return m_coefs[equation_ID][unknown_ID]; }
        bool coef(uint32_t equation_ID, uint32_t unknown_ID) const { return m_coefs[equation_ID][unknown_ID]; }
        typename std::bitset<MAX_UNKNOWN + 1>::reference result(uint32_t equation_ID) { return m_coefs[equation_ID][m_unknown]; }
        bool result(uint32_t equation_ID) const { return m_coefs[equation_ID][m_unknown]; }
        void set_equation(uint32_t equation_ID, const std::bitset<MAX_UNKNOWN + 1> &equation) { m_coefs[equation_ID] = equation; }
        bool calc() {
            for (uint32_t i = 0; i < m_unknown; i++) {
                uint32_t index = m_base_cnt;
                while (index < m_equation && !m_coefs[index][i]) index++;
                if (index < m_equation) {
                    if (index != m_base_cnt) std::swap(m_coefs[index], m_coefs[m_base_cnt]);
                    for (uint32_t j = 0; j < m_equation; j++)
                        if (j != m_base_cnt && m_coefs[j][i]) m_coefs[j] ^= m_coefs[m_base_cnt];
                    m_index[m_base_cnt++] = i;
                } else
                    m_x[i] = false;
            }
            for (uint32_t i = m_base_cnt; i < m_equation; i++)
                if (m_coefs[i][m_unknown]) return false;
            for (uint32_t i = 0; i < m_base_cnt; i++) m_x[m_index[i]] = m_coefs[i][m_unknown];
            return true;
        }
        bool has_multi_solution() const { return m_base_cnt < m_unknown; }
        bool get_solution(uint32_t unknown_ID) const { return m_x[unknown_ID]; }
    };
}