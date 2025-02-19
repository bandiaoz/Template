#ifndef __OY_GLOBALHASHMAP__
#define __OY_GLOBALHASHMAP__

#include <algorithm>
#include <bitset>
#include <chrono>
#include <cstdint>
#include <numeric>
#include <vector>

/**
 * @brief 全局哈希表
 */
namespace OY {
    namespace GHASH {
        using size_type = uint32_t;
        template <size_type N>
        struct ModLevel : std::integral_constant<size_type, N % 2 == 0 && ~ModLevel<N / 2>::value ? ModLevel<N / 2>::value + 1 : -1> {};
        template <>
        struct ModLevel<1> : std::integral_constant<size_type, 0> {};
        template <size_type N, bool = ModLevel<N>::value != -1>
        struct Moder {
            static constexpr size_type L = 64 - ModLevel<N>::value;
            size_type operator()(uint64_t a) const { return a >> L; }
        };
        template <size_type N>
        struct Moder<N, false> {
            size_type operator()(uint64_t a) const { return a % N; }
        };
        struct HashCombine {
            uint64_t operator()(uint64_t a, uint64_t b) const { return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2)); }
            template <typename... Args>
            uint64_t operator()(uint64_t a, uint64_t b, Args... args) const { return operator()(operator()(a, b), args...); }
        };
        template <typename Tp>
        struct Hash {
            static uint64_t s_bias;
            uint64_t operator()(const Tp &x) const { return (x + s_bias) * 11995408973635179863ULL; }
        };
        template <typename Tp>
        uint64_t Hash<Tp>::s_bias = std::chrono::steady_clock::now().time_since_epoch().count();
        template <>
        struct Hash<std::string> {
            uint64_t operator()(const std::string &x) const { return std::hash<std::string>()(x); }
        };
        template <typename KeyType, typename MappedType>
        struct Node {
            KeyType m_key;
            MappedType m_mapped;
        };
        template <typename KeyType>
        struct Node<KeyType, void> {
            KeyType m_key;
        };
        template <bool Record>
        struct Recorder {};
        template <>
        struct Recorder<true> : std::vector<size_type> {};
        template <typename KeyType, typename MappedType, bool MakeRecord, size_type BUFFER>
        class TableBase {
        public:
            using node = Node<KeyType, MappedType>;
            struct pair {
                node *m_ptr;
                bool m_flag;
            };
        private:
            node m_pool[BUFFER];
            std::bitset<BUFFER> m_occupied;
            Recorder<MakeRecord> m_recs;
            size_type m_size;
        public:
            /**
             * @brief 预留空间
             */
            void reserve(size_type count) {
                if constexpr (MakeRecord) m_recs.reserve(count);
            }
            /**
             * @brief 返回哈希表中元素的个数
             */
            size_type size() const { return m_size; }
            /**
             * @brief 判断哈希表是否为空
             */
            bool empty() const { return !size(); }
            /**
             * @brief 清空哈希表
             */
            void clear() {
                static_assert(MakeRecord, "MakeRecord Must Be True");
                for (auto i : m_recs) m_occupied[i] = false;
                m_recs.clear(), m_size = 0;
            }
            /**
             * @brief 遍历哈希表
             */
            template <typename Callback>
            void do_for_each(Callback &&call) {
                static_assert(MakeRecord, "MakeRecord Must Be True");
                for (auto i : m_recs)
                    if (m_occupied[i]) call(m_pool + i);
            }
            /**
             * @brief 插入元素
             */
            pair insert(const KeyType &key) {
                size_type ha = Moder<BUFFER>()(Hash<KeyType>()(key)), i = ha;
                while (m_occupied[i]) {
                    if (key == m_pool[i].m_key) return {m_pool + i, false};
                    i = i != BUFFER - 1 ? i + 1 : 0;
                }
                m_pool[i].m_key = key, m_occupied[i] = true, m_size++;
                if constexpr (MakeRecord) m_recs.push_back(i);
                return {m_pool + i, true};
            }
            /**
             * @brief 查找元素
             */
            node *find(const KeyType &key) const {
                size_type ha = Moder<BUFFER>()(Hash<KeyType>()(key)), i = ha;
                while (m_occupied[i]) {
                    if (key == m_pool[i].m_key) return (node *)(m_pool + i);
                    i = i != BUFFER - 1 ? i + 1 : 0;
                }
                return nullptr;
            }
        };
        template <typename KeyType, bool MakeRecord, size_type BUFFER>
        struct UnorderedSet : TableBase<KeyType, void, MakeRecord, BUFFER> {};
        template <typename KeyType, typename MappedType, bool MakeRecord, size_type BUFFER>
        struct UnorderedMap : TableBase<KeyType, MappedType, MakeRecord, BUFFER> {
            using typename TableBase<KeyType, MappedType, MakeRecord, BUFFER>::pair;
            /**
             * @brief 插入否则赋值
             */
            pair insert_or_assign(const KeyType &key, const MappedType &mapped) {
                pair res = this->insert(key);
                res.m_ptr->m_mapped = mapped;
                return res;
            }
            /**
             * @brief 插入否则忽略
             */
            pair insert_or_ignore(const KeyType &key, const MappedType &mapped) {
                pair res = this->insert(key);
                if (res.m_flag) res.m_ptr->m_mapped = mapped;
                return res;
            }

            MappedType &operator[](const KeyType &key) {
                pair res = this->insert(key);
                if (res.m_flag) res.m_ptr->m_mapped = MappedType{};
                return res.m_ptr->m_mapped;
            }
            /**
             * @brief 获取映射值，若不存在则返回默认值
             */
            MappedType get(const KeyType &key, const MappedType &_default) const {
                auto res = this->find(key);
                return res ? res->m_mapped : _default;
            }
            /**
             * @brief 获取映射值，必须保证哈希表中存在该键值
             */
            const MappedType &get(const KeyType &key) const { return this->find(key)->m_mapped; }
        };
    }
}

#endif