#ifndef __OY_CIPOLLA__
#define __OY_CIPOLLA__

/**
 * @brief 二次剩余算法
 * @note 只解决模数为奇素数的情况
 */
namespace OY {
    /**
     * @brief 二次剩余算法
     * @tparam Tp 自取模数类型
     * @param a 需要求解的数
     * @return 二次剩余的根 $x$，满足 $x \times x \equiv a \pmod p$
     * @note 若不存在，返回 0，否则返回较小的根（一定存在两个根互为相反数，即 $x$ 和 $p-x$）
     */
    template <typename Tp, typename ModType = typename Tp::mod_type>
    ModType Cipolla(Tp a) {
        const ModType P = Tp::mod();
        auto one = Tp::raw(1);
        if (a.pow((P - 1) / 2) != one) return 0;
        auto b = one;
        while (true) {
            Tp c = b * b - a;
            if (c.pow((P - 1) / 2) != one) break;
            b += one;
        }
        Tp neg = b * b - a;
        struct node {
            Tp a, b;
        };
        auto mul = [&](const node &x, const node &y) {
            return node{x.a * y.a + x.b * y.b * neg, x.b * y.a + x.a * y.b};
        };
        auto pow = [&](node x, ModType n) {
            node res{one, Tp{}};
            while (n) {
                if (n & 1) res = mul(res, x);
                x = mul(x, x), n >>= 1;
            }
            return res;
        };
        auto ans = pow({b, one}, (P + 1) / 2).a.val();
        return ans * 2 > P ? P - ans : ans;
    }
}

#endif