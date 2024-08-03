#include <bits/stdc++.h>

/**
 * @brief 数位 dp
 * @link: https://leetcode.cn/problems/count-the-number-of-powerful-integers/description/
 */
class Solution {
   public:
    int64_t numberOfPowerfulInt(int64_t start, int64_t finish, int limit, std::string s) {
        std::string low = std::to_string(start);
        std::string high = std::to_string(finish);
        int n = high.size();
        low = std::string(n - low.size(), '0') + low;  // 补前导零，和 high 对齐
        int diff = n - s.size();

        std::vector<int64_t> memo(n, -1);
        auto dfs = [&](auto &&self, int i, bool limit_low, bool limit_high) -> int64_t {
            if (i == low.size()) {
                return 1;
            }

            if (!limit_low && !limit_high && memo[i] != -1) {
                return memo[i];
            }

            // 第 i 个数位可以从 lo 枚举到 hi
            // 如果对数位还有其它约束，应当只在下面的 for 循环做限制，不应修改
            int lo = limit_low ? low[i] - '0' : 0;
            int hi = limit_high ? high[i] - '0' : 9;

            int64_t res = 0;
            if (i < diff) {  // 枚举这个数位填什么
                for (int d = lo; d <= std::min(hi, limit); d++) {
                    res += self(self, i + 1, limit_low && d == lo, limit_high && d == hi);
                }
            } else {  // 这个数位只能填 s[i-diff]
                int d = s[i - diff] - '0';
                if (lo <= d && d <= std::min(hi, limit)) {
                    res = self(self, i + 1, limit_low && d == lo, limit_high && d == hi);
                }
            }

            if (!limit_low && !limit_high) {
                memo[i] = res;  // 记忆化 (i, false, false)
            }
            return res;
        };

        return dfs(dfs, 0, true, true);
    }
};