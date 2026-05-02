#include "mnx/scripting/PythonBindingsExport.h"

#if defined(MNX_HAS_PYBIND11)
#include <pybind11/pybind11.h>

namespace mnx::scripting {

void bindExport(pybind11::module_& module) {
    module.def(
        "export_api_summary",
        []() {
            return "Export automation is exposed through Part.save_project() and Part.export_stl().";
        }
    );
}

} // namespace mnx::scripting
#endif
