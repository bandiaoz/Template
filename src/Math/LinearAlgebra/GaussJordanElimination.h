#ifndef __OY_GAUSSJORDANELIMINATION__
#define __OY_GAUSSJORDANELIMINATION__

#include <algorithm>
#include <cstdint>
#include <vector>

/**
 * @brief Gauss-Jordan Elimination 高斯消元
 * @tparam Tp 表示系数的数据类型，一般为 double 或者 modint 类型
 * @tparam MAX_UNKNOWN 表示未知数的最大数量
 * @tparam MAX_EQUATION 表示方程的最大数量
 * @param unknow 表示未知数的具体数量
 * @param equation 表示方程的具体数量
 * @example OY::GJE::GaussJordanElimination<Tp, MAX_UNKNOWN, MAX_EQUATION> GE(unknow, equation);
 */
namespace OY {
    namespace GJE {
        using size_type = uint32_t;
        template <typename Tp>
        struct GaussJordanGetBigger {
            bool operator()(Tp x, Tp y) const { return (x >= 0 ? x : -x) > (y >= 0 ? y : -y); }
        };
        template <typename Tp>
        struct GaussJordanIsZero {
            bool operator()(Tp x) const { return x == Tp(0); }
        };
        template <typename Tp, size_type MAX_UNKNOWN, size_type MAX_EQUATION>
        class GaussJordanElimination {
            size_type m_unknown, m_equation, m_index[MAX_UNKNOWN], m_ans_size;
            Tp m_coefs[MAX_EQUATION][MAX_UNKNOWN + 1], m_x[MAX_UNKNOWN];
        public:
            GaussJordanElimination() = default;
            GaussJordanElimination(size_type unknown, size_type equation) { reset(unknown, equation); }
            void reset(size_type unknown, size_type equation) { m_unknown = unknown, m_equation = equation, m_ans_size = 0; }
            Tp &coef(size_type equation_ID, size_type unknown_ID) { return m_coefs[equation_ID][unknown_ID]; }
            Tp coef(size_type equation_ID, size_type unknown_ID) const { return m_coefs[equation_ID][unknown_ID]; }
            Tp &result(size_type equation_ID) { return m_coefs[equation_ID][m_unknown]; }
            Tp result(size_type equation_ID) const { return m_coefs[equation_ID][m_unknown]; }
            /**
             * @brief 设置第 `equation_ID` 个方程
             * @param equation_ID 表示方程的编号
             * @param equation 表示 `{a_1, a_2, ..., a_n, b}` 的列表，其中 `a_i` 表示第 `i` 个未知数的系数，`b` 表示等式右边的值
             */
            void set_equation(size_type equation_ID, std::initializer_list<Tp> equation) { std::copy(equation.begin(), equation.end(), m_coefs[equation_ID]); }
            /**
             * @brief 计算方程组的解
             * @param get_bigger 表示判断绝对值更大的函数，一般不需要改写
             * @param is_zero 表示判断是否为 0 的函数，一般不需要改写
             * @return 返回是否有解
             */
            template <typename GetBigger = GaussJordanGetBigger<Tp>, typename IsZero = GaussJordanIsZero<Tp>>
            bool calc(GetBigger get_bigger = GetBigger(), IsZero is_zero = IsZero()) {
                std::fill_n(m_x, m_unknown, 0);
                for (size_type i = 0; i != m_unknown; i++) {
                    size_type index = m_ans_size;
                    for (size_type j = m_ans_size + 1; j < m_equation; j++)
                        if (get_bigger(m_coefs[j][i], m_coefs[index][i])) index = j;
                    if (!is_zero(m_coefs[index][i])) {
                        if (index != m_ans_size)
                            for (size_type j = 0; j <= m_unknown; j++) std::swap(m_coefs[index][j], m_coefs[m_ans_size][j]);
                        Tp inv = Tp(1) / m_coefs[m_ans_size][i];
                        for (size_type j = i; j <= m_unknown; j++) m_coefs[m_ans_size][j] *= inv;
                        for (size_type j = 0; j != m_equation; j++)
                            if (j != m_ans_size && m_coefs[j][i])
                                for (size_type k = i + 1; k <= m_unknown; k++) m_coefs[j][k] -= m_coefs[j][i] * m_coefs[m_ans_size][k];
                        m_index[m_ans_size++] = i;
                    }
                }
                for (size_type i = m_ans_size; i != m_equation; i++)
                    if (!is_zero(m_coefs[i][m_unknown])) return false;
                for (size_type i = 0; i < m_ans_size; i++) m_x[m_index[i]] = m_coefs[i][m_unknown];
                return true;
            }
            /**
             * @brief 返回方程组解的秩
             */
            size_type rank() const { return m_unknown - m_ans_size; }
            /**
             * @brief 枚举基向量
             * @param call 参数为一个 `std::vector<Tp>`，表示一个基向量
             */
            template <typename Callback>
            void enumerate_base(Callback &&call) {
                std::vector<bool> pos(m_unknown);
                std::vector<Tp> inv(m_ans_size), x(m_unknown);
                for (size_type i = 0; i != m_ans_size; i++) pos[m_index[i]] = true, inv[i] = -Tp(1) / m_coefs[i][m_index[i]];
                for (size_type i = 0; i != m_unknown; i++) {
                    if (!pos[i]) {
                        std::fill_n(x.data(), m_unknown, 0);
                        x[i] = 1;
                        for (size_type j = 0; j != m_ans_size; j++)
                            if (m_coefs[j][i]) x[m_index[j]] = m_coefs[j][i] * inv[j];
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
            Tp get_solution(size_type unknown_ID) const { return m_x[unknown_ID]; }
        };
    }
}

#endif