#include <bits/stdc++.h>
#include "src/Graph/connectivity/Tarjan_scc.h"

/*
[P1407 [国家集训队] 稳定婚姻](https://www.luogu.com.cn/problem/P1407)
[status](https://www.luogu.com.cn/record/203232832)
*/
/**
 * 有 n 对夫妻和 m 对情侣关系，如果一对夫妻感情不和，任然可以通过其他情侣关系重新组成 n 对夫妻，那么这对夫妻就是不安全的。
 * 问每一对夫妻是否安全。
 * 
 * 夫妻关系由女生向男生连边，情侣关系由男生向女生连边
 * 断开一个夫妻关系（girl -> boy），男生通过情侣关系重新找女生（boy -> girl），如果能通过一个环回到这个女生，那么可以重新稳定，这个夫妻是不安全的。
 * 也就是如果这对夫妻在同一个强连通分量中，那么这个夫妻关系是不安全的。
 */

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    OY::SCC::Graph graph(n * 2);
    std::map<std::string, int> girl, boy;
    for (int i = 0; i < n; i++) {
        std::string s, t;
        std::cin >> s >> t;
        girl[s] = i;
        boy[t] = i + n;
        graph.add_edge(girl[s], boy[t]);
    }

    int m;
    std::cin >> m;
    for (int i = 0; i < m; i++) {
        std::string s, t;
        std::cin >> s >> t;
        graph.add_edge(boy[t], girl[s]);
    }

    auto scc = graph.calc();
    for (int i = 0; i < n; i++) {
        if (scc.query(i) == scc.query(i + n)) {
            std::cout << "Unsafe\n";
        } else {
            std::cout << "Safe\n";
        }
    }

    return 0;
}