#include <bits/stdc++.h>

template <typename T>
class MinMax {
    std::multiset<T> min, max;
    int64_t min_sum, max_sum;
    MinMax() : min_sum(0), max_sum(0) {}

    void insert(std::multiset<T>& st, int64_t& sum, T x) {
        st.insert(x);
        sum += x;
    }
    void erase(std::multiset<T>& st, int64_t& sum, T x) {
        assert(st.find(x) != st.end());
        st.erase(st.find(x));
        sum -= x;
    }
    void maintain() {
        // min.size = max.size or max.size + 1
        while (min.size() > max.size() + 1) {
            T d = *min.rbegin();
            insert(max, max_sum, d);
            erase(min, min_sum, d);
        }
        while (min.size() < max.size()) {
            T d = *max.begin();
            insert(min, min_sum, d);
            erase(max, max_sum, d);
        }
        while (min.size() > 0 && max.size() > 0 && *min.rbegin() > *max.begin()) {
            T a = *min.rbegin(), b = *max.begin();
            erase(min, min_sum, a);
            erase(max, max_sum, b);
            insert(min, min_sum, b);
            insert(max, max_sum, a);
        }
    }

public:
    void insert(T x) {
        insert(min, min_sum, x);
        maintain();
    }
    void erase(T x) {
        if (min.find(x) != min.end()) {
            erase(min, min_sum, x);
        } else if (max.find(x) != max.end()) {
            erase(max, max_sum, x);
        } else {
            assert(false);
        }
        maintain();
    }
    T get_mid() {
        assert(min.size() > 0);
        return *min.rbegin();
    }
};