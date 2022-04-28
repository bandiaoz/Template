// #pragma GCC optimize(2)
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;
const int maxn = 1e6 + 10;

ll n, m;
ll a[maxn];
struct segtree {
    int lc, rc;  //记录左右子树所在的索引下标
    int dat;     //存储要统计的信息
} tr[maxn];      //开点
int root, tot;   //根节点与即时节点

int build()  //开新的节点
{
    tot++;                                      //开辟新空间
    tr[tot].lc = tr[tot].rc = tr[tot].dat = 0;  //初始化
    return tot;                                 //返回位置(指针)
}

void insert(int p, int l, int r, int val, int dat)  //添加新节点，节点管辖的是[l,r]，修改位置为val，加上dat
{
    if (l == r)  //递归基，l==r
    {
        tr[p].dat += dat;  //修改数据域
        return;            //回退
    }
    int mid = (l + r) >> 1;  //二分
    //分而治之
    if (val <= mid)  //进入[l,mid]
    {
        if (!tr[p].lc)
            tr[p].lc = build();              //未开辟则开辟新节点
        insert(tr[p].lc, l, mid, val, dat);  //递归下去继续插入
    } else                                   //[mid+1,r]
    {
        if (!tr[p].rc)
            tr[p].rc = build();                  //未开辟则开辟新节点
        insert(tr[p].rc, mid + 1, r, val, dat);  //递归下去继续插入
    }
    tr[p].dat = tr[tr[p].lc].dat + tr[tr[p].rc].dat;  //合并
}

ll query(int p, int l, int r, int ql, int qr) {
    if (ql <= l && qr >= r)  //递归基,查询区间包含了统计区间
    {
        return tr[p].dat;  //回退
    }
    ll ans = 0;              //统计答案
    int mid = (l + r) >> 1;  //划分
    if (ql <= mid)
        ans += query(tr[p].lc, l, mid, ql, qr);  //统计左子树
    if (qr > mid)
        ans += query(tr[p].rc, mid + 1, r, ql, qr);  //统计右子树
    return ans;                                      //返回答案
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int T;
    cin >> T;
    for (int cas = 1; cas <= T; cas++) {
        cout << "Case " << cas << ":" << endl;

        root = 0, tot = 0;
        cin >> n;
        root = build();
        for (int i = 1; i <= n; i++)
            cin >> a[i], insert(root, 1, n, i, a[i]);
        string s;
        while (cin >> s) {
            if (s == "End")
                break;
            else if (s == "Query") {
                int l, r;
                cin >> l >> r;
                cout << query(root, 1, n, l, r) << endl;
            } else if (s == "Add") {
                int x, v;
                cin >> x >> v;
                insert(root, 1, n, x, v);
            } else if (s == "Sub") {
                int x, v;
                cin >> x >> v;
                insert(root, 1, n, x, -v);
            }
        }
    }
}