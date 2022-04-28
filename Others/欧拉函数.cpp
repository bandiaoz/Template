//我们首先应该要知道欧拉函数的通项公式
//φ(n)=n*(1-1/p1)*(1-1/p2)*(1-1/p3)*(1-1/p4)…..(1-1/pn)，其中pi为n的质因数
//求n以内与n互质的数的个数
long long eular(long long n)
{
    long long ans = n;
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            ans -= ans / i;    //等价于通项，把n乘进去
            while (n % i == 0) //确保下一个i是n的素因数
                n /= i;
        }
    }
    if (n > 1)
        ans -= ans / n; //最后可能还剩下一个素因数没有除
    return ans;
}