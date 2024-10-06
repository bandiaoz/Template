#include <iostream>
#include <cmath>
#include "src/DataStruct/Segtree/AssignZkwTree.h"

void test() {
    std::string s[] = {"apple", "banana", "peal", "orange", "banana"};
    // 一颗不维护信息聚合的树
    OY::AssignZkw<std::string> S(s, s + 5);
    std::cout << S << std::endl;
    S.modify(1, 3, "app");
    std::cout << S << std::endl;
    S.modify(2, 4, "bank");
    std::cout << S << std::endl;
    std::cout << std::endl;
}

void test_sum() {
    int arr[] = {1, 100, 1000, 10, 10000};
    // 一颗维护信息聚合的树
    // 对于数字类型的信息，而且还是求和信息
    // 因为通过乘法，可以很快算出翻倍的值，所以可以用如下特化
    OY::AssignSumZkw<int> S(5, [&](int i) { return arr[i]; });
    std::cout << S << std::endl;
    S.modify(1, 3, 20);
    S.modify(2, 4, 5);
    std::cout << S << std::endl;
    std::cout << "sum(S[0~2]) = " << S.query(0, 2) << std::endl;
    std::cout << "sum(S[1~3]) = " << S.query(1, 3) << std::endl;
    std::cout << "sum(S[2~4]) = " << S.query(2, 4) << std::endl;
    std::cout << std::endl;
}

void test_fast_pow() {
    int arr[] = {1, 2, 1, 1, 3, 1, 2};
    // 假设维护一个数字区间，维护区间乘积
#if CPP_STANDARD >= 202002L
    auto op = [](int x, int y) { return x * y; };
    auto pow = [](int x, int n) { return std::pow(x, n); };
#else
    struct {
        int operator()(int x, int y) const { return x * y; }
    } op;
    struct {
        int operator()(int x, int n) const { return std::pow(x, n); }
    } pow;
#endif
    auto S = OY::make_fast_pow_AssignZkwTree<int, 1>(7, op, pow, [&](int i) { return arr[i]; });
    std::cout << S << std::endl;
    S.modify(1, 3, 3);
    std::cout << S << std::endl;
    S.modify(2, 5, 0.1);
    std::cout << S << std::endl;
    std::cout << "prod(S2[0~2]) = " << S.query(0, 2) << std::endl;
    std::cout << "prod(S2[1~3]) = " << S.query(1, 3) << std::endl;
    std::cout << "prod(S2[2~4]) = " << S.query(2, 4) << std::endl;
    S.modify(2, 5, 2);
    std::cout << S << std::endl;
    std::cout << "prod_all = " << S.query_all() << std::endl;
    std::cout << std::endl;
}

void test_slow_pow() {
    int64_t arr[] = {12, 2, 1, 3, 2, 0, 10};
    // 假设维护一个长整数区间，维护区间乘积
    // 由于长整数的 pow 可能有精度问题，所以只能采用 Lazy 树
#if CPP_STANDARD >= 202002L
    auto op = [](int64_t x, int64_t y) { return x * y; };
#else
    struct {
        double operator()(int64_t x, int64_t y) const { return x * y; }
    } op;
#endif
    auto S = OY::make_lazy_AssignZkwTree<int64_t, 1>(7, op, [&](int i) { return arr[i]; });
    std::cout << S << std::endl;
    S.modify(1, 3, 3);
    std::cout << S << std::endl;
    S.modify(2, 5, 1);
    std::cout << S << std::endl;
    std::cout << "prod(S2[0~2]) = " << S.query(0, 2) << std::endl;
    std::cout << "prod(S2[1~3]) = " << S.query(1, 3) << std::endl;
    std::cout << "prod(S2[2~4]) = " << S.query(2, 4) << std::endl;
    S.modify(2, 5, 2);
    std::cout << S << std::endl;
    std::cout << "prod_all = " << S.query_all() << std::endl;
    std::cout << std::endl;
}

int main() {
    test();
    test_sum();
    test_fast_pow();
    test_slow_pow();
}
/*
#输出如下
[apple, banana, peal, orange, banana]
[apple, app, app, app, banana]
[apple, app, bank, bank, bank]

[1, 100, 1000, 10, 10000]
[1, 20, 5, 5, 5]
sum(S[0~2]) = 26
sum(S[1~3]) = 30
sum(S[2~4]) = 15

[1, 2, 1, 1, 3, 1, 2]
[1, 3, 3, 3, 3, 1, 2]
[1, 3, 0, 0, 0, 0, 2]
prod(S2[0~2]) = 0
prod(S2[1~3]) = 0
prod(S2[2~4]) = 0
[1, 3, 2, 2, 2, 2, 2]
prod_all = 96

[12, 2, 1, 3, 2, 0, 10]
[12, 3, 3, 3, 2, 0, 10]
[12, 3, 1, 1, 1, 1, 10]
prod(S2[0~2]) = 36
prod(S2[1~3]) = 3
prod(S2[2~4]) = 1
[12, 3, 2, 2, 2, 2, 10]
prod_all = 5760

*/