#pragma once

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <numeric>
#include <vector>

/**
 * @brief Gauss-Jordan Bitwise XOR Elimination 高斯异或消元
 * @tparam MAX_UNKNOWN 表示未知数的最大数量
 * @tparam MAX_EQUATION 表示方程的最大数量
 * @param unknow 表示未知数的具体数量
 * @param equation 表示方程的具体数量
 * @example OY::GJBE::GaussJordanBitxorElimination<MAX_UNKNOWN, MAX_EQUATION> GE(unknown, equation);
 */
namespace OY {
    namespace GJBE {
        using size_type = uint32_t;
        template <size_type MAX_UNKNOWN, size_type MAX_EQUATION>
        class GaussJordanBitxorElimination {
        public:
            using bitset_type = std::bitset<MAX_UNKNOWN + 1>;
        private:
            size_type m_unknown, m_equation, m_ans_size, m_index[MAX_UNKNOWN];
            bitset_type m_coefs[MAX_EQUATION], m_x;
        public:
            GaussJordanBitxorElimination() = default;
            GaussJordanBitxorElimination(size_type unknown, size_type equation) { reset(unknown, equation); }
            void reset(size_type unknown, size_type equation) { m_unknown = unknown, m_equation = equation, m_ans_size = 0; }
            typename bitset_type::reference coef(size_type equation_ID, size_type unknown_ID) { return m_coefs[equation_ID][unknown_ID]; }
            bool coef(size_type equation_ID, size_type unknown_ID) const { return m_coefs[equation_ID][unknown_ID]; }
            typename bitset_type::reference result(size_type equation_ID) { return m_coefs[equation_ID][m_unknown]; }
            bool result(size_type equation_ID) const { return m_coefs[equation_ID][m_unknown]; }
            /**
             * @brief 设置第 `equation_ID` 个方程
             * @param equation_ID 表示方程的编号
             * @param equation 
             */
            void set_equation(size_type equation_ID, const bitset_type &equation) { m_coefs[equation_ID] = equation; }
            /**
             * @brief 计算方程组的解
             * @return 返回是否有解
             */
            bool calc() {
                m_x.reset();
                for (size_type i = 0; i != m_unknown; i++) {
                    size_type index = m_ans_size;
                    while (index != m_equation && !m_coefs[index][i]) index++;
                    if (index < m_equation) {
                        if (index != m_ans_size) std::swap(m_coefs[index], m_coefs[m_ans_size]);
                        for (size_type j = 0; j != m_equation; j++)
                            if (j != m_ans_size && m_coefs[j][i]) m_coefs[j] ^= m_coefs[m_ans_size];
                        m_index[m_ans_size++] = i;
                    }
                }
                for (size_type i = m_ans_size; i != m_equation; i++)
                    if (m_coefs[i][m_unknown]) return false;
                for (size_type i = 0; i != m_ans_size; i++) m_x[m_index[i]] = m_coefs[i][m_unknown];
                return true;
            }
            /**
             * @brief 返回方程组解的秩
             */
            size_type rank() const { return m_unknown - m_ans_size; }
            /**
             * @brief 枚举基向量
             * @param call 参数为 `bitset_type` 类型的函数对象
             */
            template <typename Callback>
            void enumerate_base(Callback &&call) {
                std::vector<bool> pos(m_unknown);
                for (size_type i = 0; i != m_ans_size; i++) pos[m_index[i]] = true;
                bitset_type x;
                for (size_type i = 0; i != m_unknown; i++) {
                    if (!pos[i]) {
                        x.reset(), x.set(i);
                        for (size_type j = 0; j != m_ans_size; j++) {
                            if (m_coefs[j][i]) x.set(m_index[j]);
                        }
                        call(x);
                    }
                }
            }
            /**
             * @brief 判断是否有多解
             */
            bool has_multi_solution() const { return m_ans_size < m_unknown; }
            /**
             * @brief 获取第 `unknown_ID` 个未知数的解
             */
            bool get_solution(size_type unknown_ID) const { return m_x[unknown_ID]; }
        };
    }
}