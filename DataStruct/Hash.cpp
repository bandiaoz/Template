#include <bits/stdc++.h>
#include <bits/extc++.h>

using namespace __gnu_pbds;

/**
 * @cite https://codeforces.com/blog/entry/62393
 * @example std::unordered_map<int64_t, int, custom_hash> safe_map;
            gp_hash_table<int64_t, int, custom_hash> safe_hash_table;
*/
struct custom_hash {
    static uint64_t splitmix64(uint64_t x) {
        // http://xorshift.di.unimi.it/splitmix64.c
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
};

/**
 * @cite https://codeforces.com/blog/entry/62393?#comment-464874
 * @example std::unordered_map<std::pair<int64_t, int64_t>, int, custom_hash_pair> safe_map_pair;
            gp_hash_table<std::pair<int64_t, int64_t>, int, custom_hash_pair> safe_hash_table_pair;
*/
struct custom_hash_pair {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(std::pair<uint64_t,uint64_t> x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x.first + FIXED_RANDOM) ^ (splitmix64(x.second + FIXED_RANDOM) >> 1);
	}
};