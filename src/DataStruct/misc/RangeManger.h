#ifndef __OY_RANGEMANAGER__
#define __OY_RANGEMANAGER__

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>

/**
 * @brief 区间管理
 * @example OY::RangeManager<int> ranges;
 * @note 注意区间合并，例如 [1, 11) 和 [11, 30) 会合并为 [1, 30)
 */
namespace OY {
    template <typename Tp>
    class RangeManager {
        struct Ignore {
            template <typename... Args>
            void operator()(Args...) const {}
        };
        using table_type = RangeManager<Tp>;
        using iterator = typename std::map<Tp, Tp>::iterator;
        using const_iterator = typename std::map<Tp, Tp>::const_iterator;
        Tp m_length = 0;
        std::map<Tp, Tp> m_segs;
        static Tp length_of(const std::pair<Tp, Tp> &range) { return range.second - range.first + 1; }
    public:
        /**
         * @brief 插入开区间
         */
        template <typename AddCallback = Ignore, typename RemoveCallback = Ignore>
        void add_range(std::pair<Tp, Tp> range, AddCallback &&add_call = AddCallback(), RemoveCallback &&remove_call = RemoveCallback()) {
            range.second--;
            auto it = upper_bound(range.first);
            if (it != begin()) {
                if ((--it)->second + 1 >= range.first) {
                    if (it->second >= range.second) return;
                    remove_call(range.first = it->first, it->second + 1), m_length -= length_of(*it);
                    it = m_segs.erase(it);
                } else
                    ++it;
            }
            for (; it != end() && it->first <= range.second + 1; it = m_segs.erase(it)) {
                if (it->second > range.second) range.second = it->second;
                remove_call(it->first, it->second + 1), m_length -= length_of(*it);
            }
            add_call(range.first, range.second + 1), m_length += length_of(*m_segs.emplace(range.first, range.second).first);
        }
        /**
         * @brief 移除开区间
         */
        template <typename AddCallback = Ignore, typename RemoveCallback = Ignore>
        void remove_range(std::pair<Tp, Tp> range, AddCallback &&add_call = AddCallback(), RemoveCallback &&remove_call = RemoveCallback()) {
            range.second--;
            std::vector<std::pair<Tp, Tp>> deleted, inserted;
            iterator it = m_segs.upper_bound(range.first);
            if (it != begin()) {
                if ((--it)->second >= range.first) {
                    remove_call(it->first, it->second + 1), m_length -= it->second - it->first + 1;
                    if (it->second >= range.second && it->second > range.second) {
                        add_call(range.second + 1, it->second + 1), m_length += it->second - range.second;
                        m_segs.emplace(range.second + 1, it->second);
                        if (it->first < range.first) {
                            it->second = range.first - 1;
                            add_call(it->first, it->second + 1), m_length += it->second - it->first + 1;
                            ++it;
                        } else
                            m_segs.erase(it);
                        return;
                    }
                    if (it->first < range.first) {
                        it->second = range.first - 1;
                        add_call(it->first, it->second + 1), m_length += it->second - it->first + 1;
                        ++it;
                    } else
                        it = m_segs.erase(it);
                } else
                    ++it;
            }
            for (; it != end() && it->first <= range.second; it = m_segs.erase(it)) {
                remove_call(it->first, it->second + 1), m_length -= it->second - it->first + 1;
                if (it->second > range.second) m_segs.emplace(range.second + 1, it->second), add_call(range.second + 1, it->second + 1), m_length += it->second - range.second;
            }
        }
        /**
         * @brief 查询区间是否出现过
         * @return 返回最先出现的区间的迭代器，若不存在则返回 `end()`
         */
        const_iterator any_of(const std::pair<Tp, Tp> &range) const {
            const_iterator it = upper_bound(range.second - 1);
            if (it == begin()) return end();
            if ((--it)->second < range.first) return end();
            return it;
        }
        /**
         * @brief 查询区间是否全部出现过
         * @return 返回包含 `range`，若不存在则返回 `end()`
         */
        const_iterator all_of(const std::pair<Tp, Tp> &range) const {
            const_iterator it = upper_bound(range.second - 1);
            if (it == begin()) return end();
            if ((--it)->first > range.first || it->second < range.second - 1) return end();
            return it;
        }
        /**
         * @brief 返回区间总长度
         */
        Tp length() const { return m_length; }
        /**
         * @brief 清空所有区间
         */
        void clear() { m_segs.clear(), m_length = 0; }
        /**
         * @brief 返回区间数量
         */
        auto size() const -> decltype(m_segs.size()) { return m_segs.size(); }
        auto begin() const -> decltype(m_segs.begin()) { return m_segs.begin(); }
        auto end() const -> decltype(m_segs.end()) { return m_segs.end(); }
        /**
         * @brief 获取左端点大于等于 `key` 的第一个区间
         */
        auto lower_bound(const Tp &key) const -> decltype(m_segs.lower_bound(key)) { return m_segs.lower_bound(key); }
        /**
         * @brief 获取左端点大于 `key` 的第一个区间
         */
        auto upper_bound(const Tp &key) const -> decltype(m_segs.upper_bound(key)) { return m_segs.upper_bound(key); }
        table_type &operator&=(const RangeManager<Tp> &rhs) {
            if (m_segs.empty() || rhs.m_segs.empty())
                clear();
            else {
                if (m_segs.begin()->first < rhs.m_segs.begin()->first) remove_range({m_segs.begin()->first, rhs.m_segs.begin()->first});
                auto it = rhs.m_segs.begin(), end = rhs.m_segs.end();
                while (!m_segs.empty()) {
                    auto it2 = std::next(it);
                    if (it2 == end) {
                        auto max = std::prev(m_segs.end())->second;
                        if (max > it->second) remove_range({it->second + 1, max + 1});
                        break;
                    }
                    remove_range({it->second + 1, it2->first});
                    it = it2;
                }
            }
            return *this;
        }
        table_type &operator|=(const RangeManager<Tp> &rhs) {
            for (auto &&it : rhs) {
                it.second++;
                add_range(it);
            }
            return *this;
        }
    };
    template <typename Ostream, typename Tp>
    Ostream &operator<<(Ostream &out, const RangeManager<Tp> &x) {
        out << '[';
        for (auto it = x.begin(); it != x.end(); ++it) {
            if (it != x.begin()) out << ", ";
            out << "[" << it->first << ", " << it->second + 1 << ")";
        }
        return out << ']';
    }
}

#endif