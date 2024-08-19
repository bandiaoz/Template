#include <bits/stdc++.h>

/**
 * @brief 静态区间 kth
 * @link https://judge.yosupo.jp/problem/range_kth_smallest
 */
template <typename T>
class KthSmall {
private:
    struct wavelet_matrix {
        using size_type = std::uint32_t;
        struct bit_vector {
            static constexpr size_type wsize = 64;
            static size_type rank64(std::uint64_t x, size_type i) {
                return __builtin_popcountll(x & ((1ull << i) - 1));
            }
            #pragma pack(4)
            struct block_t {
                std::uint64_t bit;
                size_type sum;
            };
            #pragma pack()
            size_type n, zeros;
            std::vector<block_t> block;
            bit_vector(size_t _n = 0) : n(_n), block(n / wsize + 1) {}
            int operator[](size_type i) const {
                return block[i / wsize].bit >> i % wsize & 1;
            }
            void set(size_type i) {
                block[i / wsize].bit |= (std::uint64_t)1 << i % wsize;
            }
            void build() {
                for (size_type j = 0; j < n / wsize; ++j)
                    block[j + 1].sum = block[j].sum + __builtin_popcountll(block[j].bit);
                zeros = rank0(n);
            }
            size_type rank0(size_type i) const { return i - rank1(i); }
            size_type rank1(size_type i) const {
                auto&& e = block[i / wsize];
                return e.sum + rank64(e.bit, i % wsize);
            }
        };

        size_type n, lg;
        std::vector<T> a;
        std::vector<bit_vector> bv;
        wavelet_matrix(size_type _n = 0) : n(_n), a(n) {}
        wavelet_matrix(const std::vector<T>& _a) : n(_a.size()), a(_a) { build(); }
        T& operator[](size_type i) { return a[i]; }
        void build() {
            lg = std::__lg(std::max<T>(*std::max_element(std::begin(a), std::end(a)), 1)) + 1;
            bv.assign(lg, n);
            std::vector<T> cur = a, nxt(n);
            for (auto h = lg; h--;) {
                for (size_type i = 0; i < n; ++i)
                    if (cur[i] >> h & 1)
                        bv[h].set(i);
                bv[h].build();
                std::array<typename std::vector<T>::iterator, 2> it{std::begin(nxt), std::begin(nxt) + bv[h].zeros};
                for (size_type i = 0; i < n; ++i)
                    *it[bv[h][i]]++ = cur[i];
                std::swap(cur, nxt);
            }
        }
        T kth(size_type l, size_type r, size_type k) const {
            T res = 0;
            for (auto h = lg; h--;) {
                auto l0 = bv[h].rank0(l), r0 = bv[h].rank0(r);
                if (k < r0 - l0)
                    l = l0, r = r0;
                else {
                    k -= r0 - l0;
                    res |= (T)1 << h;
                    l += bv[h].zeros - l0;
                    r += bv[h].zeros - r0;
                }
            }
            return res;
        }
        size_type count(size_type l, size_type r, T ub) const {
            if (ub >= (T)1 << lg)
                return r - l;
            size_type res = 0;
            for (auto h = lg; h--;) {
                auto l0 = bv[h].rank0(l), r0 = bv[h].rank0(r);
                if (~ub >> h & 1)
                    l = l0, r = r0;
                else {
                    res += r0 - l0;
                    l += bv[h].zeros - l0;
                    r += bv[h].zeros - r0;
                }
            }
            return res;
        }
        size_type count(size_type l, size_type r, T lb, T ub) const {
            return count(l, r, ub) - count(l, r, lb);
        }
    };
    std::pair<std::vector<int>, std::vector<T>> zip(const std::vector<T>& a) {
        int n = a.size();
        std::vector<std::pair<T, int>> p(n);
        for (int i = 0; i < n; ++i)
            p[i] = {a[i], i};
        std::sort(std::begin(p), std::end(p));
        std::vector<int> na(n);
        std::vector<T> v;
        for (int k = 0, rnk = -1; k < n; ++k) {
            if (k == 0 or p[k - 1].first < p[k].first)
                v.push_back(p[k].first), ++rnk;
            na[p[k].second] = rnk;
        }
        return std::make_pair(na, v);
    }
    void valid(int l, int &r, int k) {
        int len = r - l;
        assert(0 <= l && l < r && r <= n);
        assert(0 <= k && k < len);
    }
    std::vector<T> vec;
    wavelet_matrix wm;
    int n;
public:
    KthSmall() {}
    KthSmall(const std::vector<T> &v) : n(v.size()) {
        auto [a, _vec] = zip(v);
        wm = wavelet_matrix(a);
        vec = _vec;
    }
    /**
     * @brief 返回区间 [l, r) 中第 k 小的数，k 从 0 开始
     * @note 也可以认为是区间 [l, r) 从小到大排序后下标为 k 的数
     */
    T small(int l, int r, int k) {
        valid(l, r, k);
        return vec[wm.kth(l, r, k)];
    }
    /**
     * @brief 返回区间 [l, r) 中第 k 大的数，k 从 0 开始
     * @note 也可以认为是区间 [l, r) 从大到小排序后下标为 k 的数
     */
    T large(int l, int r, int k) {
        valid(l, r, k);
        k = r - l - k - 1;
        return vec[wm.kth(l, r, k)];
    }
};