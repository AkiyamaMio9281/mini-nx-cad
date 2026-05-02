#pragma once

#include <compare>
#include <cstdint>
#include <functional>

namespace mnx {

template <typename Tag>
class BasicId {
public:
    using ValueType = std::uint64_t;

    constexpr BasicId() = default;
    explicit constexpr BasicId(ValueType value) : value_(value) {}

    [[nodiscard]] static constexpr BasicId invalid() {
        return BasicId();
    }

    [[nodiscard]] constexpr bool isValid() const {
        return value_ != kInvalidValue;
    }

    [[nodiscard]] constexpr ValueType value() const {
        return value_;
    }

    friend constexpr auto operator<=>(BasicId, BasicId) = default;

private:
    static constexpr ValueType kInvalidValue = 0;

    ValueType value_ = kInvalidValue;
};

} // namespace mnx

namespace std {

template <typename Tag>
struct hash<mnx::BasicId<Tag>> {
    size_t operator()(const mnx::BasicId<Tag>& id) const noexcept {
        return hash<typename mnx::BasicId<Tag>::ValueType> {}(id.value());
    }
};

} // namespace std
