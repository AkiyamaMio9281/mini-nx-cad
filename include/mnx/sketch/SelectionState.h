#pragma once

#include "mnx/sketch/GeometryRef.h"

#include <vector>

namespace mnx {

class SelectionState {
public:
    [[nodiscard]] bool empty() const {
        return selectedRefs_.empty();
    }

    [[nodiscard]] const std::vector<GeometryRef>& refs() const {
        return selectedRefs_;
    }

    [[nodiscard]] bool contains(const GeometryRef& ref) const;
    void select(const GeometryRef& ref);
    void deselect(const GeometryRef& ref);
    void clear();

private:
    std::vector<GeometryRef> selectedRefs_;
};

} // namespace mnx
