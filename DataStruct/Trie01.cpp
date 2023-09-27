// 01 Trie find maximal xor sum
template <typename T, int B = 30>
class Trie01 {
    std::vector<std::array<int, 2>> ch_;
    std::vector<int> cnt;
    int emptyNode() {
        ch_.push_back(std::array<int, 2>{-1, -1});
        cnt.push_back(0);
        return ch_.size() - 1;
    }

   public:
    Trie01() { emptyNode(); }
    void insert(T x, int add = 1) {
        for (int i = B, p = 0; i >= 0; --i) {
            T c = x >> i & 1;
            if (ch_[p][c] == -1) {
                ch_[p][c] = emptyNode();
            }
            p = ch_[p][c];
            cnt[p] += add;
        }
    }
    void erase(T x) { insert(x, -1); }
    T getMax(T x) {
        T res = 0;
        for (int i = B, p = 0; i >= 0; --i) {
            T c = x >> i & 1;
            if (int child = ch_[p][c ^ 1]; child != -1 && cnt[child] > 0) {
                p = child;
                res |= T(1) << i;
            } else {
                p = ch_[p][c];
            }
        }
        return res;
    }
    T getMin(T x) {
        T res = 0;
        for (int i = B, p = 0; i >= 0; --i) {
            T c = x >> i & 1;
            if (int child = ch_[p][c]; child != -1 && cnt[child] > 0) {
                p = child;
            } else {
                p = ch_[p][c ^ 1];
                res |= T(1) << i;
            }
        }
        return res;
    }
};