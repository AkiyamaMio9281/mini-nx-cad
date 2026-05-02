#pragma once

#if defined(MNX_HAS_PYBIND11)
namespace pybind11 {
class module_;
}

namespace mnx::scripting {
void bindFeature(pybind11::module_& module);
}
#endif
