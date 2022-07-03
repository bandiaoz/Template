#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

#define rep(i, a, n) for (int i = a; i <= n; ++i)
#define per(i, a, n) for (int i = n; i >= a; --i)

const int maxn = 5e4 + 5;
inline double sq(double x) { return x * x; }
namespace KDTree {
const int DIM = 10;
int K;  //维数
struct Point {
    int x[DIM];
    double distance(const Point &A) const {
        double ans = 0;
        rep(i, 1, K) ans += sq(x[i] - A.x[i]);
        return ans;
    }
    void input() { rep(i, 1, K) scanf("%d", &x[i]); }
    void output() { rep(i, 1, K) printf("%d%c", x[i], " \n"[i == K]); }
};
struct qnode {
    Point p;
    double dis;
    qnode() {}
    qnode(Point _p, double _dis) : p(_p), dis(_dis) {}
    bool operator<(const qnode &A) const { return dis < A.dis; }
};
priority_queue<qnode> q;
struct cmpx {
    int div;
    cmpx(const int &_div) : div(_div) {}
    bool operator()(const Point &A, const Point &B) {
        for (int i = 1; i <= K; ++i)
            if (A.x[div % K + i] != B.x[div % K + i])
                return A.x[div % K + i] < B.x[div % K + i];
        return true;
    }
};
bool cmp(const Point &A, const Point &B, int div) {
    cmpx cp = cmpx(div);
    return cp(A, B);
}
struct Node {
    Point e;
    Node *lc, *rc;
    int div;
} pool[maxn], *tail, *root;
void init() { tail = pool; }
Node *build(Point *a, int l, int r, int div) {
    if (l >= r) return NULL;
    Node *p = tail++;
    p->div = div;
    int mid = (l + r) >> 1;
    nth_element(a + l, a + mid, a + r, cmpx(div));
    p->e = a[mid];
    p->lc = build(a, l, mid, div % K + 1);
    p->rc = build(a, mid + 1, r, div % K + 1);
    return p;
}
void search(Point p, Node *x, int div, int m) {
    if (!x) return;
    if (cmp(p, x->e, div)) {
        search(p, x->lc, div % K + 1, m);
        if (q.size() < m) {
            q.push(qnode(x->e, p.distance(x->e)));
            search(p, x->rc, div % K + 1, m);
        } else {
            if (p.distance(x->e) < q.top().dis) {
                q.pop();
                q.push(qnode(x->e, p.distance(x->e)));
            }
            if (sq(x->e.x[div] - p.x[div]) < q.top().dis)
                search(p, x->rc, div % K + 1, m);
        }
    } else {
        search(p, x->rc, div % K + 1, m);
        if (q.size() < m) {
            q.push(qnode(x->e, p.distance(x->e)));
            search(p, x->lc, div % K + 1, m);
        } else {
            if (p.distance(x->e) < q.top().dis) {
                q.pop();
                q.push(qnode(x->e, p.distance(x->e)));
            }
            if (sq(x->e.x[div] - p.x[div]) < q.top().dis)
                search(p, x->lc, div % K + 1, m);
        }
    }
}
void search(Point p, int m) {
    while (!q.empty()) q.pop();
    search(p, root, 1, m);
}
};  // namespace KDTree
KDTree::Point p[maxn];
int main() {
    int n, k;
    while (scanf("%d%d", &n, &k) == 2) {
        KDTree::K = k;
        rep(i, 0, n - 1) p[i].input();
        KDTree::init();
        KDTree::root = KDTree::build(p, 0, n, 1);
        int q;
        scanf("%d", &q);
        KDTree::Point o;
        while (q--) {
            o.input();
            int m;
            scanf("%d", &m);
            KDTree::search(o, m);
            printf("the closest %d points are:\n", m);
            int cnt = 0;
            while (!KDTree::q.empty()) {
                p[cnt++] = KDTree::q.top().p;
                KDTree::q.pop();
            }
            for (int i = 0; i < m; ++i) p[m - 1 - i].output();
        }
    }
}