#include "mnx/scripting/PythonBindingsSketch.h"

#if defined(MNX_HAS_PYBIND11)
#include <pybind11/pybind11.h>

namespace mnx::scripting {

void bindSketch(pybind11::module_& module) {
    module.def(
        "sketch_api_summary",
        []() {
            return "Sketch automation is exposed through Part.new_sketch(), Part.add_line(), and Part.add_circle().";
        }
    );
}

} // namespace mnx::scripting
#endif
