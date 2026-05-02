find_package(Qt6 COMPONENTS Widgets OpenGLWidgets QUIET)
find_package(OpenGL QUIET)
find_package(GTest QUIET)
find_package(pybind11 QUIET)

if(NOT Qt6_FOUND)
    message(WARNING "Qt6 Widgets was not found. The cad_desktop app target will be skipped.")
endif()

if(Qt6_FOUND AND NOT OpenGL_FOUND)
    message(WARNING "OpenGL was not found. The Qt rendering target may fail to link.")
endif()

if(NOT GTest_FOUND)
    message(STATUS "GTest was not found. Phase 0 smoke tests will use a minimal CTest fallback.")
endif()

if(NOT pybind11_FOUND)
    message(STATUS "pybind11 was not found. The mini_nx Python extension target will be skipped.")
endif()
