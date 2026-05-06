cmake_minimum_required(VERSION 3.24)

project(
    MiniNX
    VERSION 0.1.0
    DESCRIPTION "Mini NX modular CAD prototype"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(MNX_BUILD_DESKTOP "Build the Qt desktop application" ON)
option(MNX_BUILD_TESTS "Build Mini NX tests" ON)
option(MNX_ENABLE_WARNINGS "Enable compiler warnings for Mini NX targets" ON)
option(MNX_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(MNX_ENABLE_ASAN "Enable AddressSanitizer in supported Debug builds" OFF)
option(MNX_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer in supported Debug builds" OFF)
option(MNX_ENABLE_CLANG_TIDY "Enable clang-tidy when available" OFF)

include(Dependencies)
include(Warnings)
include(Sanitizers)
include(StaticAnalysis)

add_subdirectory(src)

if(MNX_BUILD_DESKTOP)
    add_subdirectory(apps/cad_desktop)
endif()

if(MNX_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
