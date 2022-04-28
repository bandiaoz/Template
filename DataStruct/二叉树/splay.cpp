#include <algorithm>
#include <cstdio>
#include <iostream>
using namespace std;

const int MAXL = 100005;
const int INF = 2147480000;

class Splay  //存储规则：小左大右，重复节点记录
{
#define root e[0].ch[1]  //该树的根节点
   private:
    class node {
       public:
        int v, father;  //节点值，父级节点
        int ch[2];      //左孩子=0，右孩子=1
        int sum;        //自己+自己下级有多少节点。在根节点为1。
        int recy;       //记录自己被重复了几次
    };
    node e[MAXL];  //Splay树主体

    int n, points;  //使用存储数,元素数
    void update(int x) {
        e[x].sum = e[e[x].ch[0]].sum + e[e[x].ch[1]].sum + e[x].recy;
    }
    int identify(int x) {
        return e[e[x].father].ch[0] == x ? 0 : 1;
    }
    void connect(int x, int f, int son)  //连接函数。用法：connect(son,father,1/0)
    {
        e[x].father = f;
        e[f].ch[son] = x;
    }  //作用：使得x的father=f，f的son=x.
    void rotate(int x) {
        int y = e[x].father;
        int mroot = e[y].father;
        int mrootson = identify(y);
        int yson = identify(x);
        int B = e[x].ch[yson ^ 1];
        connect(B, y, yson);
        connect(y, x, (yson ^ 1));
        connect(x, mroot, mrootson);
        update(y);
        update(x);
    }
    void splay(int at, int to)  //将at位置的节点移动到to位置
    {
        to = e[to].father;
        while (e[at].father != to) {
            int up = e[at].father;
            if (e[up].father == to)
                rotate(at);
            else if (identify(up) == identify(at)) {
                rotate(up);
                rotate(at);
            } else {
                rotate(at);
                rotate(at);
            }
        }
    }
    int crepoint(int v, int father) {
        n++;
        e[n].v = v;
        e[n].father = father;
        e[n].sum = e[n].recy = 1;
        return n;
    }
    void destroy(int x)  //pop后摧毁节点
    {
        e[x].v = e[x].ch[0] = e[x].ch[1] = e[x].sum = e[x].father = e[x].recy = 0;
        if (x == n)
            n--;  //最大限度优化
    }

   public:
    int getroot() { return root; }
    int find(int v)  //用于外部的find调用
    {
        int now = root;
        while (true) {
            if (e[now].v == v) {
                splay(now, root);
                return now;
            }
            int next = v < e[now].v ? 0 : 1;
            if (!e[now].ch[next])
                return 0;
            now = e[now].ch[next];
        }
    }
    int build(int v)  //内部调用的插入函数，没有splay
    {
        points++;
        if (n == 0)  //特判无点状态
        {
            root = 1;
            crepoint(v, 0);
        } else {
            int now = root;
            while (true)  //向下找到一个空节点
            {
                e[now].sum++;  //自己的下级肯定增加了一个节点
                if (v == e[now].v) {
                    e[now].recy++;
                    return now;
                }
                int next = v < e[now].v ? 0 : 1;
                if (!e[now].ch[next]) {
                    crepoint(v, now);
                    e[now].ch[next] = n;
                    return n;
                }
                now = e[now].ch[next];
            }
        }
        return 0;
    }
    void push(int v)  //插入元素时，先添加节点，再进行伸展
    {
        int add = build(v);
        splay(add, root);
    }
    void pop(int v)  //删除节点
    {
        int deal = find(v);
        if (!deal)
            return;
        points--;
        if (e[deal].recy > 1) {
            e[deal].recy--;
            e[deal].sum--;
            return;
        }
        if (!e[deal].ch[0]) {
            root = e[deal].ch[1];
            e[root].father = 0;
        } else {
            int lef = e[deal].ch[0];
            while (e[lef].ch[1])
                lef = e[lef].ch[1];
            splay(lef, e[deal].ch[0]);
            int rig = e[deal].ch[1];
            connect(rig, lef, 1);
            connect(lef, 0, 1);
            update(lef);
        }
        destroy(deal);
    }
    int rank(int v)  //获取值为v的元素在这棵树里是第几小
    {
        int ans = 0, now = root;
        while (true) {
            if (e[now].v == v)
                return ans + e[e[now].ch[0]].sum + 1;
            if (now == 0)
                return 0;
            if (v < e[now].v)
                now = e[now].ch[0];
            else {
                ans = ans + e[e[now].ch[0]].sum + e[now].recy;
                now = e[now].ch[1];
            }
        }
        if (now)
            splay(now, root);
        return 0;
    }
    int atrank(int x)  //获取第x小的元素的值
    {
        if (x > points)
            return -INF;
        int now = root;
        while (true) {
            int minused = e[now].sum - e[e[now].ch[1]].sum;
            if (x > e[e[now].ch[0]].sum && x <= minused)
                break;
            if (x < minused)
                now = e[now].ch[0];
            else {
                x = x - minused;
                now = e[now].ch[1];
            }
        }
        splay(now, root);
        return e[now].v;
    }
    int upper(int v)  //寻找该值对应的一个最近的上界值
    {
        int now = root;
        int result = INF;
        while (now) {
            if (e[now].v > v && e[now].v < result)
                result = e[now].v;
            if (v < e[now].v)
                now = e[now].ch[0];
            else
                now = e[now].ch[1];
        }
        return result;
    }
    int lower(int v)  //寻找该值对应的一个最近的下界值
    {
        int now = root;
        int result = -INF;
        while (now) {
            if (e[now].v < v && e[now].v > result)
                result = e[now].v;
            if (v > e[now].v)
                now = e[now].ch[1];
            else
                now = e[now].ch[0];
        }
        return result;
    }
#undef root
};
Splay F;

int main() {
    
    return 0;
}