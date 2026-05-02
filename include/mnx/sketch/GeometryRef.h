#pragma once

#include "mnx/core/ids/EntityId.h"

#include <compare>

namespace mnx {

enum class GeometryRefKind {
    WholeEntity,
    StartPoint,
    EndPoint,
    CenterPoint,
    Midpoint,
};

class GeometryRef {
public:
    constexpr GeometryRef() = default;
    constexpr GeometryRef(EntityId entityId, GeometryRefKind kind)
        : entityId_(entityId), kind_(kind) {}

    [[nodiscard]] static constexpr GeometryRef whole(EntityId entityId) {
        return {entityId, GeometryRefKind::WholeEntity};
    }

    [[nodiscard]] static constexpr GeometryRef startPoint(EntityId entityId) {
        return {entityId, GeometryRefKind::StartPoint};
    }

    [[nodiscard]] static constexpr GeometryRef endPoint(EntityId entityId) {
        return {entityId, GeometryRefKind::EndPoint};
    }

    [[nodiscard]] static constexpr GeometryRef centerPoint(EntityId entityId) {
        return {entityId, GeometryRefKind::CenterPoint};
    }

    [[nodiscard]] static constexpr GeometryRef midpoint(EntityId entityId) {
        return {entityId, GeometryRefKind::Midpoint};
    }

    [[nodiscard]] constexpr EntityId entityId() const {
        return entityId_;
    }

    [[nodiscard]] constexpr GeometryRefKind kind() const {
        return kind_;
    }

    [[nodiscard]] constexpr bool isValid() const {
        return entityId_.isValid();
    }

    friend constexpr auto operator<=>(GeometryRef, GeometryRef) = default;

private:
    EntityId entityId_;
    GeometryRefKind kind_ = GeometryRefKind::WholeEntity;
};

} // namespace mnx
