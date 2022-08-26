#include<bits/stdc++.h>
#define rep(i, a, n) for (int i = a; i <= n; ++i)
#define per(i, a, n) for (int i = n; i >= a; --i)
using namespace std;
typedef long long ll;
const int maxn = 1e7 + 5;
int n, a[maxn];
int ls[maxn], rs[maxn];
int top = 0;
// stack<int> st;
int st[maxn];
// ls代表笛卡尔树每个节点的左孩子，rs代表笛卡尔树每个节点的右孩子
// 按照满足二叉搜索树的权值排序，插入在右链
// 栈顶元素为当前元素的左孩子
// 当前元素为栈顶元素的右孩子
int main() {
    int n;
    scanf("%d", &n);
    rep(i, 1, n) { 
        scanf("%d", &a[i]);
        // while (st.size() && a[st.top()] > a[i]) ls[i] = st.top(), st.pop();
        // if (st.size()) rs[st.top()] = i;
        // st.push(i);
        while (top && a[st[top]] > a[i]) ls[i] = st[top--]; 
        if (top) rs[st[top]] = i; 
        st[++top] = i;
    }
    ll lans = 0, rans = 0;
    rep(i, 1, n) {
        lans ^= 1LL * i * (ls[i] + 1);
        rans ^= 1LL * i * (rs[i] + 1);
    }
    printf("%lld %lld\n", lans, rans);
    return 0;
}