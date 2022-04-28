#include <cstring>
#include <iostream>
using namespace std;

class BigNum {
   private:
    int a[1000];
    int len;

   public:
    BigNum() {
        len = 1;
        memset(a, 0, sizeof(a));
    }
    BigNum(const int b);
    BigNum(char *s);
    BigNum(const BigNum &T);
    BigNum &operator=(const BigNum &n);

    friend istream &operator>>(istream &, BigNum &);
    friend ostream &operator<<(ostream &, BigNum &);

    BigNum operator+(const BigNum &T) const;
    BigNum operator-(const BigNum &T) const;
    BigNum operator*(const BigNum &T) const;
    BigNum operator/(const int &b) const;
    BigNum operator|(const BigNum &T) const;
    BigNum operator%(const BigNum &T) const;

    bool operator>(const BigNum &T) const;
    bool operator>(const int &t) const;
};

BigNum::BigNum(const int b) {
    len = 0;
    memset(a, 0, sizeof(a));
    int t = b;
    while (t) {
        int x = t % 10;
        a[len++] = x;
        t /= 10;
    }
}
BigNum::BigNum(char *s) {
    memset(a, 0, sizeof(a));
    int l = strlen(s);
    len = l;
    int cnt = 0;
    for (int i = l - 1; i >= 0; --i)
        a[cnt++] = s[i] - '0';
}
BigNum::BigNum(const BigNum &T) : len(T.len) {
    memset(a, 0, sizeof(a));
    for (int i = 0; i < len; ++i)
        a[i] = T.a[i];
}
BigNum &BigNum::operator=(const BigNum &n) {
    len = n.len;
    memset(a, 0, sizeof(a));
    for (int i = 0; i < len; ++i)
        a[i] = n.a[i];
    return *this;
}
istream &operator>>(istream &in, BigNum &b) {
    char ch[1000];
    in >> ch;
    int l = strlen(ch);
    int count = 0;
    for (int i = l - 1; i > 0; --i) {
        b.a[count++] = ch[i] - '0';
    }
    if (ch[0] == '-')
        b.a[count - 1] = 0 - b.a[count - 1];
    else
        b.a[count++] = ch[0] - '0';
    b.len = count;
    return in;
}
ostream &operator<<(ostream &out, BigNum &b) {
    for (int i = b.len - 1; i >= 0; --i)
        cout << b.a[i];
    return out;
}
BigNum BigNum::operator+(const BigNum &T) const {
    BigNum t(*this);
    int big;
    big = T.len > len ? T.len : len;
    for (int i = 0; i < big; ++i) {
        t.a[i] += T.a[i];
        if (t.a[i] >= 10) {
            t.a[i + 1]++;
            t.a[i] -= 10;
        }
    }
    if (t.a[big] != 0)
        t.len = big + 1;
    else
        t.len = big;
    return t;
}
BigNum BigNum::operator-(const BigNum &T) const {
    int big;
    bool flag;
    BigNum t1, t2;
    if (*this > T) {
        t1 = *this;
        t2 = T;
        flag = 0;
    } else {
        t1 = T;
        t2 = *this;
        flag = 1;
    }
    big = t1.len;
    for (int i = 0; i < big; ++i) {
        if (t1.a[i] < t2.a[i]) {
            int j = i + 1;
            while (t1.a[j] == 0)
                j++;
            t1.a[j--]--;
            while (j > i)
                t1.a[j--] += 9;
            t1.a[i] += 10 - t2.a[i];
        } else
            t1.a[i] -= t2.a[i];
    }
    t1.len = big;
    while (t1.a[t1.len - 1] == 0 && t1.len > 1) {
        t1.len--;
        big--;
    }
    if (flag)
        t1.a[big - 1] = 0 - t1.a[big - 1];
    return t1;
}
BigNum BigNum::operator*(const BigNum &T) const {
    BigNum ret;
    int up;
    int temp, temp1;
    int i, j;
    for (i = 0; i < len; ++i) {
        up = 0;
        for (j = 0; j < T.len; ++j) {
            temp = a[i] * T.a[j] + ret.a[i + j] + up;
            if (temp >= 10) {
                temp1 = temp % 10;
                up = temp / 10;
                ret.a[i + j] = temp1;
            } else {
                up = 0;
                ret.a[i + j] = temp;
            }
        }
        if (up != 0)
            ret.a[i + j] = up;
    }
    ret.len = i + j;
    while (ret.a[ret.len - 1] == 0 && ret.len > 1)
        ret.len--;
    return ret;
}
BigNum BigNum::operator/(const int &b) const {
    BigNum ret;
    int down = 0;
    for (int i = len - 1; i >= 0; --i) {
        ret.a[i] = (a[i] + down * 10) / b;
        down = a[i] + down * 10 - ret.a[i] * b;
    }
    ret.len = len;
    while (ret.a[ret.len - 1] == 0 && ret.len > 1)
        ret.len--;
    return ret;
}
BigNum BigNum::operator|(const BigNum &T) const {
    BigNum ans;
    BigNum a = *this, b = T;
    int len1 = len, len2 = T.len;
    int t = len1 - len2;
    BigNum x = 1;
    BigNum ten = 10;
    for (int i = 0; i < t; ++i) {
        b = b * ten;
        x = x * ten;
    }
    while (a > T || (!(a > T) && !(T > a))) {
        while (a > b || (!(a > b) && !(b > a))) {
            a = a - b;
            ans = ans + x;
        }
        b = b / 10;
        x = x / 10;
    }
    return ans;
}
BigNum BigNum::operator%(const BigNum &T) const {
    BigNum ans;
    BigNum a = *this, b = T;
    int len1 = len, len2 = T.len;
    int t = len1 - len2;
    BigNum x = 1;
    BigNum ten = 10;
    for (int i = 0; i < t; ++i) {
        b = b * ten;
        x = x * ten;
    }
    while (a > T || (!(a > T) && !(T > a))) {
        while (a > b || (!(a > b) && !(b > a))) {
            a = a - b;
            ans = ans + x;
        }
        b = b / 10;
        x = x / 10;
    }
    return a;
}
bool BigNum::operator>(const BigNum &T) const {
    int ln;
    if (len > T.len)
        return true;
    else if (len < T.len)
        return false;

    ln = len - 1;
    while (a[ln] == T.a[ln] && ln >= 0)
        ln--;
    if (ln >= 0 && a[ln] > T.a[ln])
        return true;
    else
        return false;
}
bool BigNum::operator>(const int &t) const {
    BigNum b(t);
    return *this > b;
}

int main() {

}
