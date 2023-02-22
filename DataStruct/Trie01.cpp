// 01 Trie find maximal xor sum
template <typename T, int B = 30>
class Trie01 {
    using Node = array<int, 2>;
    vector<Node> ch_;
    Node emptyNode() {
        Node tp;
        tp.fill(-1);
        return tp;
    }
    void addNode(int fa, int c) {
        ch_[fa][c] = ch_.size();
        ch_.emplace_back(emptyNode());
        // cnt.push_back(0);
    }

   public:
    Trie01() : { 
        ch_.push_back(emptyNode()); 
        // cnt.push_back(0);
    }
    void insert(T x) {
        for (int i = B, p = 0; i >= 0; --i) {
            int c = x >> i & 1;
            if (ch_[p][c] == -1) addNode(p, c);
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