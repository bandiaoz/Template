#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;
const int K = 100005;
int nt[K], extand[K];
char S[K], T[K];
void Getnext(char *T, int *next) {
    int len = strlen(T), a = 0;
    next[0] = len;
    while (a < len - 1 && T[a] == T[a + 1])
        a++;
    next[1] = a;
    a = 1;
    for (int k = 2; k < len; k++) {
        int p = a + next[a] - 1, L = next[k - a];
        if ((k - 1) + L >= p) {
            int j = (p - k + 1) > 0 ? (p - k + 1) : 0;
            while (k + j < len && T[k + j] == T[j])
                j++;
            next[k] = j;
            a = k;
        } else
            next[k] = L;
    }
}
void GetExtand(char *S, char *T, int *next) {
    Getnext(T, next);
    int slen = strlen(S), tlen = strlen(T), a = 0;
    int MinLen = slen < tlen ? slen : tlen;
    while (a < MinLen && S[a] == T[a])
        a++;
    extand[0] = a;
    a = 0;
    for (int k = 1; k < slen; k++) {
        int p = a + extand[a] - 1, L = next[k - a];
        if ((k - 1) + L >= p) {
            int j = (p - k + 1) > 0 ? (p - k + 1) : 0;
            while (k + j < slen && j < tlen && S[k + j] == T[j])
                j++;
            extand[k] = j;
            a = k;
        } else
            extand[k] = L;
    }
}
int main() {
    while (scanf("%s%s", S, T) == 2) {
        GetExtand(S, T, nt);
        for (int i = 0; i < strlen(T); i++)
            printf("%d ", nt[i]);
        puts("");
        for (int i = 0; i < strlen(S); i++)
            printf("%d ", extand[i]);
        puts("");
    }
}