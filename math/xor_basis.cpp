/**
 * @brief 线性基
 * @cite https://github.com/nealwu/competitive-programming/blob/master/bits/xor_basis.cc
 * @link https://www.luogu.com.cn/problem/P3812
 */
template<typename T, int BITS = 30>
struct Xor_basis {
    // A list of basis values sorted in decreasing order, where each value has a unique highest bit.
    std::vector<T> basis;

    T min_value(T start) const {
        if (basis.size() == BITS) return 0;
        for (int i = 0; i < basis.size(); i++) {
            start = std::min(start, start ^ basis[i]);
        }
        return start;
    }

    T max_value(T start = 0) const {
        if (basis.size() == BITS) return (T(1) << BITS) - 1;
        for (int i = 0; i < basis.size(); i++) {
            start = std::max(start, start ^ basis[i]);
        }
        return start;
    }

    bool add(T x) {
        x = min_value(x);
        if (x == 0) return false;

        basis.push_back(x); // not guaranteed that x is a power of 2.
        int k = basis.size() - 1;

        // Insertion sort.
        while (k > 0 && basis[k] > basis[k - 1]) {
            std::swap(basis[k], basis[k - 1]);
            k--;
        }

        // Remove the highest bit of x from other basis elements.
        // TODO: this can be removed for speed if desired.
        for (int i = k - 1; i >= 0; i--) {
            basis[i] = std::min(basis[i], basis[i] ^ x);
        }

        return true;
    }

    void merge(const Xor_basis<T> &other) {
        for (int i = 0; i < other.basis.size() && basis.size() < BITS; i++) {
            add(other.basis[i]);
        }
    }
};