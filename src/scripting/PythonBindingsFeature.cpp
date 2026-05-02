#include "mnx/scripting/PythonBindingsFeature.h"

#if defined(MNX_HAS_PYBIND11)
#include <pybind11/pybind11.h>

namespace mnx::scripting {

void bindFeature(pybind11::module_& module) {
    module.def(
        "feature_api_summary",
        []() {
            return "Feature automation is exposed through Part.extrude() and Part.revolve().";
        }
    );
}

} // namespace mnx::scripting
#endif
