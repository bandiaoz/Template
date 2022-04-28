// 01 Trie find maximal xor sum
template <typename T, int B = 30>
class Trie01 {
    using Node = array<int, 2>;
    vector<Node> ch_;
    void addNode(int fa, int c) {
        ch_[fa][c] = ch_.size();
        ch_.emplace_back(Node());
    }

   public:
    Trie01() : ch_(1) {}
    void insert(T x) {
        for (int i = B, p = 0; i >= 0; --i) {
            int c = x >> i & 1;
            if (ch_[p][c] == 0) addNode(p, c);
            p = ch_[p][c];
        }
    }
    T getMax(T x) {
        T res = 0;
        for (int i = B, p = 0; i >= 0; --i) {
            int c = x >> i & 1;
            if (ch_[p][c ^ 1]) {
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
            if (ch_[p][c]) {
                p = ch_[p][c];
            } else {
                p = ch_[p][c ^ 1];
                res |= 1 << i;
            }
        }
        return res;
    }
};