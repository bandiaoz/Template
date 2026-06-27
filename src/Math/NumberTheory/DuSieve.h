#ifndef __AA_DUSIEVE__
#define __AA_DUSIEVE__

#include <cstdint>
#include <map>
#include <vector>

/**
 * @brief 杜教筛，求数论函数的前缀和
 * @details 设算术函数 $$f,g$$，令 $$h=f\ast g$$。若能快速计算
 
 *          $$H(n)=\sum_{i\le n}h(i)$$ 与 $$G(n)=\sum_{i\le n}g(i)$$，并给出 $$g(1)$$，

 *          则可用本模板求 $$S(n)=\sum_{i\le n}f(i)$$。
 * @note 递推：

 *          $$g(1)S(n) = \sum_{i = 1}^n h(i) - \sum_{i = 2}^n g(i)S\left(\left\lfloor \frac{n}{i} \right\rfloor\right)$$，

 * @example $$f = \varphi$$，$$g = 1$$，则有 $$h = f \ast g = \varphi \ast 1 = \mathrm{id}$$
 *          auto f = [&](uint32_t n) -> int64_t { return sieve.get_Euler_Phi(n); };
 *          auto getH = [](uint32_t n) -> int64_t { return 1LL * n * (n + 1) / 2; };
 *          auto getG = [](uint32_t n) -> int64_t { return n; };
 *          AA::DU::WeightedPrefix solver(getH, getG, 1LL);
 *          solver.build_small_by_f(LIMIT, f);
 *          auto ans = solver.query(N);
 * @note 可选：注入小范围（约 $$n^{2/3}$$ 范围） $$S(i)$$ 前缀（手动或通过提供 $$f(i)$$ 计算），以减少记忆化查询次数；
 *       单次查询复杂度通常约为 $$O(n^{2/3})$$（取决于 $$H,G$$ 的计算复杂度）。
 */
namespace AA {
    namespace DU {
        using size_type = uint32_t;
        template <typename SumTp, typename GetHPrefix, typename GetGPrefix>
        struct WeightedPrefix {
            GetHPrefix m_getH;
            GetGPrefix m_getG;
            SumTp m_g1 = (SumTp)1;
            size_type m_limit = 0;
            bool m_has_small = false;
            std::vector<SumTp> m_small;
            std::map<uint64_t, SumTp> m_cache;

            WeightedPrefix(GetHPrefix getH = GetHPrefix(), GetGPrefix getG = GetGPrefix(), SumTp g1 = (SumTp)1, size_type small_limit = 0) : m_getH(getH), m_getG(getG), m_g1(g1) {
                if (small_limit) reserve_small(small_limit);
            }
            /**
             * @brief 设置 $$h$$ 的前缀计算器
             */
            void set_getH(const GetHPrefix &getH) { m_getH = getH; clear_cache(); }
            /**
             * @brief 设置 $$g$$ 的前缀计算器
             */
            void set_getG(const GetGPrefix &getG) { m_getG = getG; clear_cache(); }
            /**
             * @brief 设置 $$g(1)$$
             */
            void set_g1(SumTp g1) { m_g1 = g1; clear_cache(); }

            /**
             * @brief 预留小范围 $$S(i)$$ 表空间
             * @note 调用后需填充
             */
            void reserve_small(size_type limit) {
                m_limit = limit;
                m_small.assign(limit + 1, 0);
                m_has_small = false;
            }
            /**
             * @brief 通过提供 $$f(i)$$ 构造小范围前缀：$$S(i)=\sum_{j\le i} f(j)$$
             */
            template <typename GetF>
            void build_small_by_f(size_type limit, GetF &&get_f) {
                reserve_small(limit);
                for (size_type i = 1; i <= limit; i++) m_small[i] = m_small[i - 1] + (SumTp)get_f(i);
                m_has_small = true;
                clear_cache();
            }
            /**
             * @brief 直接注入小范围 $$S(i)$$ 前缀
             * @note 需满足 prefix[0]==0
             */
            void assign_small_prefix(const std::vector<SumTp> &prefix) {
                m_limit = (size_type)prefix.size() - 1;
                m_small = prefix;
                m_has_small = true;
                clear_cache();
            }
            void clear_small() { m_small.clear(); m_limit = 0; m_has_small = false; }
            void clear_cache() { m_cache.clear(); }

            /**
             * @brief 查询 $$S(n)$$，整除分块 + 记忆化
             */
            SumTp query(uint64_t n) {
                if (m_has_small && n <= m_limit) return m_small[(size_type)n];
                auto it = m_cache.find(n);
                if (it != m_cache.end()) return it->second;
                SumTp res = m_getH(n);
                uint64_t l = 2;
                while (l <= n) {
                    uint64_t q = n / l;
                    uint64_t r = n / q;
                    SumTp wsum = m_getG(r) - (l > 1 ? m_getG(l - 1) : (SumTp)0);
                    res -= wsum * query(q);
                    l = r + 1;
                }
                res /= m_g1;
                return m_cache[n] = res;
            }
        };
    }
}

#endif
