#pragma once

#include "../Others/Z.hpp"
#include <chrono>
#include <random>
#include <string>
#include <vector>

namespace Hashing {
constexpr uint64_t mod = 1'000'000'000'000'000'003ULL;
using hash_t = ModInt<int64_t, mod>;

const hash_t base = std::mt19937_64(std::chrono::steady_clock::now().time_since_epoch().count())() % mod;
static std::vector<hash_t> pow{1};

static void expand_pow(size_t index) {
    if (index < pow.size()) {
        return;
    }
    auto old_size = pow.size();
    pow.resize(index * 2 + 1);
    for (auto i = old_size; i <= index * 2; i++) {
        pow[i] = pow[i - 1] * base;
    }
}

struct HashResult {
    HashResult() {}
    HashResult(hash_t _h, size_t _size) : h(_h), len(_size) {}
    HashResult(const std::string& s) {
        for (auto c : s) {
            *this += c;
        }
    }
    HashResult& operator+=(const HashResult &rhs) {
        h = h * pow[rhs.len] + rhs.h;
        len += rhs.len;
        return *this;
    }
    HashResult& operator+=(char ch) {
        h = h * base + ch;
        len += 1;
        return *this;
    }
    friend HashResult operator+(HashResult lhs, HashResult rhs) { return lhs += rhs; }
    bool operator==(HashResult rhs) const { return len == rhs.len && h == rhs.h; }
    size_t size() const { return len; }
    uint64_t hash() const { return h.val; }

private:
    hash_t h = 0;
    size_t len = 0;
};

struct HashStr {
    HashStr() : pref(1) {}
    HashStr(const std::string& s) : pref(std::size(s) + 1) {
        expand_pow(s.size());
        for (size_t i = 0; i < s.size(); i++) {
            pref[i + 1] = pref[i] * base + s[i];
        }
    }
    void push_back(char c) {
        expand_pow(pref.size());
        pref.push_back(pref.back() * base + c);
    }
    void pop_back() {
        assert(pref.size() > 1);
        pref.pop_back();
    }
    HashStr& operator+=(const HashStr &rhs) {
        expand_pow(pref.size() + rhs.pref.size());
        for (size_t i = 0; i < rhs.pref.size(); i++) {
            pref.push_back(pref.back() * base + rhs.pref[i]);
        }
        return *this;
    }
    HashStr& operator+=(char ch) {
        expand_pow(pref.size());
        pref.push_back(pref.back() * base + ch);
        return *this;
    }
    HashStr operator+(const HashStr &rhs) const {
        HashStr res = *this;
        res += rhs;
        return res;
    }
    HashStr operator+(char ch) const {
        HashStr res = *this;
        res += ch;
        return res;
    }
    HashResult sub_hash(size_t pos, size_t n = -1) const {
        if (n == -1) {
            n = pref.size() - pos - 1;
        }
        assert(pos + n < std::size(pref));
        return {pref[pos + n] - pref[pos] * pow[n], n};
    }
    HashResult hash_result() const {
        return sub_hash(0, pref.size() - 1);
    }
    uint64_t hash() const { return pref.back().val; }
    size_t size() const { return pref.size() - 1; }

private:
    std::vector<hash_t> pref;
};

struct HashStrView {
    HashStrView(const HashStr &str) : s(str), _pos(0), _n(str.size()) {}
    HashStrView(const HashStr &str, size_t pos, size_t n) : s(str), _pos(pos), _n(n) {}

    HashResult sub_hash(size_t pos, size_t n) const { return s.sub_hash(_pos + pos, n); }
    HashResult hash_result() const { return sub_hash(0, _n); }
    HashStrView sub_str(size_t pos, size_t n) { return {s, _pos + pos, n}; }
    uint64_t hash() const { return hash_result().hash(); }
    bool operator==(const HashStrView& rhs) const { return hash_result() == rhs.hash_result(); }
private:
    const HashStr &s;
    size_t _pos, _n;
};
} // namespace Hashing 

int main() {
    std::string s = "abcabc";

    Hashing::HashStr hash(s);
    std::cout << hash.sub_hash(0, 3).hash() << std::endl;
    std::cout << hash.sub_hash(3, 3).hash() << std::endl;

    std::string t = "abc";
    Hashing::HashStr hash2(t);
    std::cout << hash2.hash() << std::endl;

    Hashing::HashStr hash3;
    for (auto c : t) {
        hash3 += c;
    }
    std::cout << hash3.hash() << std::endl;
}