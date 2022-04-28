#include <algorithm>
#include <cstdio>
typedef long long ll;
#define tr t[root]
#define lson t[root << 1]
#define rson t[root << 1 | 1]
using namespace std;
const int maxn = 1e6 + 5;
ll n, m;
ll a[maxn];
//区间修改，区间查询
struct segment_tree {
    ll l, r;                       //标记为该节点统计的区间
    ll val;                        //统计的结果值
    ll add;                        //add的延迟标记
} t[maxn << 2];                    //4倍空间
void build(ll root, ll l, ll r) {  //建树，开始调用build(1,1,n)
    tr.l = l, tr.r = r;            //将根所在的位置标上
    if (l == r) {                  //递归基，区间无法再划分
        tr.val = a[l];             //统计值
        return;                    //回退
    }
    int mid = (l + r) >> 1;            //划分，对半分
    build(root << 1, l, mid);          //建左子树
    build(root << 1 | 1, mid + 1, r);  //建右子树
    tr.val = (lson.val + rson.val);    //统计父亲节点的答案
}
void spread(ll root) {
    if (tr.add) {                                    //若打了标记
        lson.val += tr.add * (lson.r - lson.l + 1);  //更新左子节点
        rson.val += tr.add * (rson.r - rson.l + 1);  //更新右子节点
        lson.add += tr.add;                          //下传延时标记至左子节点
        rson.add += tr.add;                          //下传延时标记至右子节点
        tr.add = 0;                                  //清空父亲的延时标记
    }
}
void update(ll root, ll l, ll r, ll x) {
    if (l <= tr.l && tr.r <= r) {         //递归基，无需划分问题的时候
        tr.val += x * (tr.r - tr.l + 1);  //直接在和上添加答案
        tr.add += x;                      //同时打上标记
        return;                           //返回
    }
    spread(root);  //下传延时标记
    int mid = (tr.l + tr.r) / 2;
    if (l <= mid) update(root << 1, l, r, x);     //左边小于中点，与左子节点有关
    if (r > mid) update(root << 1 | 1, l, r, x);  //右边比中点大，与右子节点有关
    tr.val = lson.val + rson.val;                 //更新父节点
}

ll query(ll root, ll l, ll r) {
    if (l <= tr.l && tr.r <= r)    //递归基，无需划分
        return tr.val;             //直接回退
    spread(root);                  //下传延时标记
    int mid = (tr.l + tr.r) >> 1;  //取中点
    ll ans = 0;
    if (l <= mid) ans += query(root << 1, l, r);     //左端点小于中点，说明左子树贡献了答案
    if (r > mid) ans += query(root << 1 | 1, l, r);  //右端点大于中点，说明右子树贡献了答案
    return ans;                                      //返回答案
}

int main() {
    scanf("%lld %lld", &n, &m);
    for (int i = 1; i <= n; i++)
        scanf("%lld", &a[i]);
    build(1, 1, n);
    while (m--) {
        ll op, l, r, x;
        scanf("%lld", &op);
        if (op == 1) {
            scanf("%lld %lld %lld", &l, &r, &x);
            update(1, l, r, x);
        }
        if (op == 2) {
            scanf("%lld %lld", &l, &r);
            printf("%lld\n", query(1, l, r));
        }
    }
    return 0;
}