#include <bits/stdc++.h>

#include "src/Math/Modular/DynamicModInt32.h"

void test_Fermat_10000019() {
    // 验证费马定理
    std::cout << "test_Fermat_10000019\n";
    using mint = OY::DynamicModInt32<0>;
    mint::set_mod(10000019, true);
    mint prod(1);
    for (auto i = 1; i != mint::mod() - 1; i++) {
        prod *= i;
    }
    std::cout << mint::mod() - 2 << "! mod " << mint::mod() << " = " << prod << std::endl;
}

void test_small_modint() {
    // 验证区间内随机数的和
    // 模数取个小点的偶数
    std::cout << "test_small_modint\n";
    using mint = OY::DynamicModInt32<1>;
    mint::set_mod(16, false);
    std::mt19937_64 rand;
    long long sum_ll = 0;
    mint sum_mint;
    uint64_t start = rand() % (1ull << 40), len = 10000001;
    for (uint64_t a = 0; a < len; a++) {
        sum_ll += start + a;
        sum_mint += start + a;
    }
    std::cout << "sum from " << start << " to " << start + len - 1 << " = " << sum_ll << std::endl;
    std::cout << "sum from " << start << " to " << start + len - 1 << " mod " << mint::mod() << " = " << sum_mint << std::endl;
}

void test_big_modint() {
    // 计算区间内随机数的乘积
    // 模数取个大点的质数
    // 验证就算了，可以用 python 验证
    std::cout << "test_big_modint\n";
    using mint = OY::DynamicModInt32<2>;
    mint::set_mod(1000000007, true);
    std::mt19937_64 rand;
    mint prod = 1;
    uint64_t start = rand(), len = 10000001;
    for (uint64_t a = 0; a < len; a++)
        prod *= start + a;
    std::cout << "prod from " << start << " to " << start + len - 1 << " mod " << mint::mod() << " = " << prod << std::endl;
}

int main() {
    test_Fermat_10000019();
    std::cout << "\n";
    test_small_modint();
    std::cout << "\n";
    test_big_modint();
    std::cout << "\n";

    return 0;
}