#include <bits/stdc++.h>
using namespace std;

char a[50], b[50];
typedef struct TreeNode *BinTree;
struct TreeNode
{
    char data;
    BinTree right, left;
};

BinTree CreatNode()
{
    BinTree p = (BinTree)malloc(sizeof(TreeNode));
    p->left = NULL;
    p->right = NULL;
    return p;
}

BinTree ReTree(int a1, int a2, int b1, int b2)
{
    BinTree head = CreatNode();
    head->data = a[a1];
    for (int i = b1; i <= b2; i++)
    {
        if (b[i] == a[a1])
        {
            if (i != b1)
                head->left = ReTree(a1 + 1, a1 + i - b1, b1, i - 1);
            if (i != b2)
                head->right = ReTree(a1 + i - b1 + 1, a2, i + 1, b2);
            break;
        }
    }
    return head;
}

int getHight(BinTree tree) //求高度
{
    if (tree == NULL)
        return 0;
    return max(getHight(tree->left), getHight(tree->right)) + 1;
}

int main()
{
    int n;
    cin >> n;
    cin >> a >> b;
    BinTree BT = ReTree(0, n - 1, 0, n - 1);
    cout << getHight(BT) << endl;
}