#ifndef __OY_CANTORSEQUENCE__
#define __OY_CANTORSEQUENCE__

#include <cstdint>
#include <vector>

/**
 * @brief 逆康托展开，根据排名反向求出元素的排列方式
 * @note 只支持无重集合的逆康托展开
 */
namespace OY {
    std::vector<uint32_t> get_Cantor_sequence(uint32_t n, uint64_t k) {
        uint64_t permutation = 1;
        std::vector<uint32_t> res(n);
        std::vector<bool> counter(n);
        for (uint32_t index = n - 1; ~index; index--) {
            res[index] = k / permutation % (n - index);
            permutation *= n - index;
        }
        if (k >= permutation) {
            res.clear();
            return res;
        }
        counter.flip();
        for (uint32_t &id : res)
            for (uint32_t i = 0; i < n; i++)
                if (counter[i] && !id--) {
                    counter[id = i] = false;
                    break;
                }
        return res;
    }
}

#endif