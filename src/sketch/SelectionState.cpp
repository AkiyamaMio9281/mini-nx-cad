#include "mnx/sketch/SelectionState.h"

#include <algorithm>

namespace mnx {

bool SelectionState::contains(const GeometryRef& ref) const {
    return std::find(selectedRefs_.begin(), selectedRefs_.end(), ref) != selectedRefs_.end();
}

void SelectionState::select(const GeometryRef& ref) {
    if(ref.isValid() && !contains(ref)) {
        selectedRefs_.push_back(ref);
    }
}

void SelectionState::deselect(const GeometryRef& ref) {
    selectedRefs_.erase(std::remove(selectedRefs_.begin(), selectedRefs_.end(), ref), selectedRefs_.end());
}

void SelectionState::clear() {
    selectedRefs_.clear();
}

} // namespace mnx
