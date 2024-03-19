#include <bits/stdc++.h>

class MinMax {
public:
    std::multiset<int> min, max;
    int64_t min_sum, max_sum;
    MinMax() : min_sum(0), max_sum(0) {}

    void maintain() {
        // min.size = max.size or max.size + 1
        while (min.size() > max.size() + 1) {
            int d = *min.rbegin();
            max.insert(d);
            min.erase(min.find(d));
            min_sum -= d;
            max_sum += d;
        }
        while (min.size() < max.size()) {
            int d = *max.begin();
            min.insert(d);
            max.erase(max.find(d));
            max_sum -= d;
            min_sum += d;
        }
        while (min.size() > 0 && max.size() > 0 && *min.rbegin() > *max.begin()) {
            int a = *min.rbegin(), b = *max.begin();
            min.erase(min.find(a));
            max.erase(max.find(b));
            min.insert(b);
            max.insert(a);
            min_sum += b - a;
            max_sum += a - b;
        }
    }
    void insert(int x) {
        min.insert(x);
        min_sum += x;
        maintain();
    }
    void erase(int x) {
        if (min.find(x) != min.end()) {
            min_sum -= x;
            min.erase(min.find(x));
        } else if (max.find(x) != max.end()) {
            max_sum -= x;
            max.erase(max.find(x));
        } else {
            assert(false);
        }
        maintain();
    }
    int get_mid() {
        assert(min.size() > 0);
        return *min.rbegin();
    }
};