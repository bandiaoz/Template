// 01 Trie find maximal xor sum
template <typename T, int B = 30>
class Trie01 {
    vector<vector<int>> ch_;
    // vector<int> cnt;
    int emptyNode() {
        ch_.push_back(vector<int>(2, -1));
        // cnt.push_back(0);
        return ch_.size() - 1;
    }

   public:
    Trie01() : { emptyNode(); }
    void insert(T x) {
        for (int i = B, p = 0; i >= 0; --i) {
            int c = x >> i & 1;
            if (ch_[p][c] == -1) {
                ch_[p][c] = emptyNode();
            }
            p = ch_[p][c];
            // cnt[p]++;
        }
    }
    T getMax(T x) {
        T res = 0;
        for (int i = B, p = 0; i >= 0; --i) {
            int c = x >> i & 1;
            if (ch_[p][c ^ 1] != -1) {
                p = ch_[p][c ^ 1];
                res |= 1 << i;
            } else {
                p = ch_[p][c];
            }
        }
        return res;
    }
    T getMin(T x) {
        T res = 0;
        for (int i = B, p = 0; i >= 0; --i) {
            int c = x >> i & 1;
            if (ch_[p][c] != -1) {
                p = ch_[p][c];
            } else {
                p = ch_[p][c ^ 1];
                res |= 1 << i;
            }
        }
        return res;
    }
};