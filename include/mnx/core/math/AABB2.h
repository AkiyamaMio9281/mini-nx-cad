#pragma once

#include "mnx/core/math/Tolerance.h"
#include "mnx/core/math/Vec2.h"

#include <algorithm>
#include <limits>

namespace mnx {

class AABB2 {
public:
    constexpr AABB2() = default;
    constexpr AABB2(const Vec2& min, const Vec2& max) : min_(min), max_(max), valid_(true) {}

    [[nodiscard]] static constexpr AABB2 empty() {
        return AABB2();
    }

    [[nodiscard]] constexpr bool isValid() const {
        return valid_;
    }

    [[nodiscard]] constexpr const Vec2& min() const {
        return min_;
    }

    [[nodiscard]] constexpr const Vec2& max() const {
        return max_;
    }

    void expand(const Vec2& point) {
        if(!valid_) {
            min_ = point;
            max_ = point;
            valid_ = true;
            return;
        }

        min_ = {std::min(min_.x(), point.x()), std::min(min_.y(), point.y())};
        max_ = {std::max(max_.x(), point.x()), std::max(max_.y(), point.y())};
    }

    void expand(const AABB2& box) {
        if(!box.isValid()) {
            return;
        }
        expand(box.min());
        expand(box.max());
    }

    [[nodiscard]] bool contains(const Vec2& point, double tolerance = kDefaultTolerance) const {
        return valid_ && point.x() >= min_.x() - tolerance && point.x() <= max_.x() + tolerance
            && point.y() >= min_.y() - tolerance && point.y() <= max_.y() + tolerance;
    }

    [[nodiscard]] bool intersects(const AABB2& rhs, double tolerance = kDefaultTolerance) const {
        return valid_ && rhs.valid_ && min_.x() <= rhs.max_.x() + tolerance
            && max_.x() + tolerance >= rhs.min_.x() && min_.y() <= rhs.max_.y() + tolerance
            && max_.y() + tolerance >= rhs.min_.y();
    }

    [[nodiscard]] Vec2 center() const {
        return valid_ ? (min_ + max_) * 0.5 : Vec2 {};
    }

    [[nodiscard]] Vec2 size() const {
        return valid_ ? (max_ - min_) : Vec2 {};
    }

private:
    Vec2 min_ {std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()};
    Vec2 max_ {-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()};
    bool valid_ = false;
};

[[nodiscard]] inline AABB2 unite(const AABB2& lhs, const AABB2& rhs) {
    AABB2 result = lhs;
    result.expand(rhs);
    return result;
}

} // namespace mnx
