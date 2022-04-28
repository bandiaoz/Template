#include <bits/stdc++.h>
using namespace std;
const int kind = 26; //有多少种字符
const int N = 1005;
const int M = 2000005;
struct node
{
    node *next[kind];
    node *fail;
    int id; //病毒编号
    node()
    {
        for (int i = 0; i < kind; i++)
            next[i] = NULL;
        fail = NULL;
        id = 0;
    }
};
node *root;
int head, tail;
char source[M], s[M];
char vir[N][55];
int cnt[N];

void Insert(char *str, int id)
{
    node *p = root;
    int i = 0, index;
    while (str[i])
    {
        index = str[i] - 'A';
        if (p->next[index] == NULL)
            p->next[index] = new node();
        p = p->next[index];
        i++;
    }
    p->id = id;
}
void build(node *root)
{
    queue<node *> Q;
    root->fail = NULL;
    for (int i = 0; i < kind; ++i)
    {
        node *tmp = root->next[i];
        if (tmp != NULL)
        {
            tmp->fail = root;
            Q.push(tmp);
        }
    }
    while (!Q.empty())
    {
        node *u = Q.front();
        Q.pop();
        for (int i = 0; i < kind; ++i)
        {
            node *r = u->next[i];
            if (r == NULL)
            {
                continue;
            }
            Q.push(r);
            node *v = u->fail;
            while (v != root && v->next[i] == NULL)
                v = v->fail;
            r->fail = v->next[i];
        }
    }
}
void Query(char *str)
{
    int i = 0, index;
    node *p = root;
    while (str[i])
    {
        index = str[i] - 'A';
        while (p->next[index] == NULL && p != root)
            p = p->fail;
        p = p->next[index];
        if (p == NULL)
            p = root;
        node *temp = p;
        // while (temp != root && temp->id > 0) {
        //     cnt[temp->id]++;
        //     temp = temp->fail;
        // }
        i++;
    }
}
void Free(node *p)
{
    for (int i = 0; i < kind; ++i)
        if (p->next[i])
            Free(p->next[i]);
    delete p;
}
