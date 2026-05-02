#include "mnx/scripting/PythonModule.h"

#if defined(MNX_HAS_PYBIND11)
#include "mnx/scripting/PythonBindingsExport.h"
#include "mnx/scripting/PythonBindingsFeature.h"
#include "mnx/scripting/PythonBindingsPart.h"
#include "mnx/scripting/PythonBindingsSketch.h"

#include <pybind11/pybind11.h>

namespace mnx::scripting {

void bindMiniNxModule(pybind11::module_& module) {
    module.doc() = "Mini NX Python automation API";
    bindPart(module);
    bindSketch(module);
    bindFeature(module);
    bindExport(module);
}

} // namespace mnx::scripting

PYBIND11_MODULE(mini_nx, module) {
    mnx::scripting::bindMiniNxModule(module);
}
#endif
