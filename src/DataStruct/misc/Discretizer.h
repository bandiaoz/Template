#ifndef __OY_DISCRETIZER__
#define __OY_DISCRETIZER__

#include <algorithm>
#include <array>
#include <numeric>
#include <tuple>
#include <vector>

/**
 * @brief 离散化数组
 * @example OY::Discretizer<Tp> D;
 * @note 继承自 `std::vector`， 重载了 `operator<<` ，可以插入元素、数组、二维数组、pair、array。
 */
namespace OY {
    template <typename Tp>
    struct Discretizer : std::vector<Tp> {
        using std::vector<Tp>::vector;
        Discretizer &operator<<(const Tp &item) {
            std::vector<Tp>::push_back(item);
            return *this;
        }
        Discretizer &operator<<(const std::vector<Tp> &items) {
            std::vector<Tp>::insert(std::end(*this), items.begin(), items.end());
            return *this;
        }
        Discretizer &operator<<(const std::vector<std::vector<Tp>> &items) {
            for (auto &each : items) *this << each;
            return *this;
        }
        Discretizer &operator<<(const std::pair<Tp, Tp> &items) { return *this << items.first << items.second; }
        template <size_t N>
        Discretizer &operator<<(const std::array<Tp, N> &items) {
            std::vector<Tp>::insert(std::end(*this), items.begin(), items.end());
            return *this;
        }
        void prepare() {
            std::sort(std::begin(*this), std::end(*this));
            std::vector<Tp>::resize(std::unique(std::begin(*this), std::end(*this)) - std::begin(*this));
        }
        /**
         * @brief 返回元素 `item` 在离散化数组中的排名
         */
        typename std::vector<Tp>::size_type rank(const Tp &item) { return std::distance(std::begin(*this), std::lower_bound(std::begin(*this), std::end(*this), item)); }
    };
}

#endif