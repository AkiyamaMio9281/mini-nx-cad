#pragma once

#include "mnx/constraints/ConstraintType.h"
#include "mnx/core/ids/ConstraintId.h"
#include "mnx/core/utils/Status.h"
#include "mnx/sketch/GeometryRef.h"
#include "mnx/sketch/Sketch.h"

#include <span>
#include <string>
#include <vector>
#include <memory>

namespace mnx {

class Constraint {
public:
    Constraint(
        ConstraintId id,
        ConstraintType type,
        std::vector<GeometryRef> refs,
        bool driving,
        std::string label
    );
    virtual ~Constraint() = default;

    [[nodiscard]] ConstraintId id() const {
        return id_;
    }

    [[nodiscard]] ConstraintType type() const {
        return type_;
    }

    [[nodiscard]] std::span<const GeometryRef> refs() const {
        return refs_;
    }

    [[nodiscard]] bool isDriving() const {
        return driving_;
    }

    [[nodiscard]] const std::string& label() const {
        return label_;
    }

    void setLabel(std::string label);

    [[nodiscard]] virtual std::string debugDescription() const;
    [[nodiscard]] virtual Status validate(const Sketch& sketch) const;
    [[nodiscard]] virtual std::unique_ptr<Constraint> clone() const = 0;

protected:
    [[nodiscard]] Status validateRefCount(std::size_t expected) const;
    [[nodiscard]] Status validateRefExists(const Sketch& sketch, const GeometryRef& ref) const;
    [[nodiscard]] Status validateAllRefsExist(const Sketch& sketch) const;

private:
    ConstraintId id_;
    ConstraintType type_;
    std::vector<GeometryRef> refs_;
    bool driving_ = true;
    std::string label_;
};

[[nodiscard]] bool isPointLikeRef(GeometryRefKind kind);
[[nodiscard]] bool isEntityLikeRef(GeometryRefKind kind);

} // namespace mnx
