#pragma once

#include "mnx/core/math/Tolerance.h"
#include "mnx/core/math/Vec3.h"

#include <algorithm>
#include <limits>

namespace mnx {

class AABB3 {
public:
    constexpr AABB3() = default;
    constexpr AABB3(const Vec3& min, const Vec3& max) : min_(min), max_(max), valid_(true) {}

    [[nodiscard]] static constexpr AABB3 empty() {
        return AABB3();
    }

    [[nodiscard]] constexpr bool isValid() const {
        return valid_;
    }

    [[nodiscard]] constexpr const Vec3& min() const {
        return min_;
    }

    [[nodiscard]] constexpr const Vec3& max() const {
        return max_;
    }

    void expand(const Vec3& point) {
        if(!valid_) {
            min_ = point;
            max_ = point;
            valid_ = true;
            return;
        }

        min_ = {std::min(min_.x(), point.x()), std::min(min_.y(), point.y()), std::min(min_.z(), point.z())};
        max_ = {std::max(max_.x(), point.x()), std::max(max_.y(), point.y()), std::max(max_.z(), point.z())};
    }

    void expand(const AABB3& box) {
        if(!box.isValid()) {
            return;
        }
        expand(box.min());
        expand(box.max());
    }

    [[nodiscard]] bool contains(const Vec3& point, double tolerance = kDefaultTolerance) const {
        return valid_ && point.x() >= min_.x() - tolerance && point.x() <= max_.x() + tolerance
            && point.y() >= min_.y() - tolerance && point.y() <= max_.y() + tolerance
            && point.z() >= min_.z() - tolerance && point.z() <= max_.z() + tolerance;
    }

    [[nodiscard]] bool intersects(const AABB3& rhs, double tolerance = kDefaultTolerance) const {
        return valid_ && rhs.valid_ && min_.x() <= rhs.max_.x() + tolerance
            && max_.x() + tolerance >= rhs.min_.x() && min_.y() <= rhs.max_.y() + tolerance
            && max_.y() + tolerance >= rhs.min_.y() && min_.z() <= rhs.max_.z() + tolerance
            && max_.z() + tolerance >= rhs.min_.z();
    }

    [[nodiscard]] Vec3 center() const {
        return valid_ ? (min_ + max_) * 0.5 : Vec3 {};
    }

    [[nodiscard]] Vec3 size() const {
        return valid_ ? (max_ - min_) : Vec3 {};
    }

private:
    Vec3 min_ {
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(),
    };
    Vec3 max_ {
        -std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
        -std::numeric_limits<double>::infinity(),
    };
    bool valid_ = false;
};

[[nodiscard]] inline AABB3 unite(const AABB3& lhs, const AABB3& rhs) {
    AABB3 result = lhs;
    result.expand(rhs);
    return result;
}

} // namespace mnx
