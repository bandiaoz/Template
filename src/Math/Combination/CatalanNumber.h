#pragma once

#include <algorithm>
#include <cstdint>

/**
 * @brief 卡特兰数
 * @param Tp 任意自取模数类
 * @param MAX_RANGE 最大打标范围
 * @param range 实际打标范围
 * @example OY::CatalanNumber<Z, MAX_RANGE> catalan(range);
 *          catalan.query(3); or catalan[3];
 */
namespace OY {
    template <typename Tp, uint32_t MAX_RANGE>
    struct CatalanNumber {
        using mod_type = typename Tp::mod_type;
        Tp m_inverse[MAX_RANGE + 2], m_val[MAX_RANGE + 1];
        CatalanNumber(uint32_t range = MAX_RANGE) {
            m_inverse[1] = m_val[0] = Tp::raw(1);
            const mod_type P = Tp::mod();
            for (uint32_t i = 1, j = 2; i <= range; i++, j = j + 4 >= P ? j + 4 - P : j + 4) {
                mod_type q = P / (i + 1), r = P - q * (i + 1);
                m_inverse[i + 1] = m_inverse[r] * Tp::raw(q ? P - q : 0);
                m_val[i] = m_val[i - 1] * m_inverse[i + 1] * Tp::raw(j);
            }
        }
        Tp query(uint32_t x) const { return m_val[x]; }
        const Tp &operator[](uint32_t x) const { return m_val[x]; }
    };
}