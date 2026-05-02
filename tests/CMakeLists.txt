add_executable(mnx_phase0_smoke_tests phase0_smoke_tests.cpp)

if(GTest_FOUND)
    target_compile_definitions(mnx_phase0_smoke_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_phase0_smoke_tests PRIVATE GTest::gtest_main mnx::core)
    include(GoogleTest)
    gtest_discover_tests(mnx_phase0_smoke_tests)
else()
    target_link_libraries(mnx_phase0_smoke_tests PRIVATE mnx::core)
    add_test(NAME mnx_phase0_smoke_tests COMMAND mnx_phase0_smoke_tests)
endif()

mnx_enable_warnings(mnx_phase0_smoke_tests)
mnx_enable_sanitizers(mnx_phase0_smoke_tests)
mnx_enable_static_analysis(mnx_phase0_smoke_tests)

set(MNX_CORE_TEST_SOURCES
    core/CoreTestMain.cpp
    core/IdTests.cpp
    core/Vec2Tests.cpp
    core/Vec3Tests.cpp
    core/MatrixTests.cpp
    core/Transform2DTests.cpp
    core/Transform3DTests.cpp
    core/AABB2Tests.cpp
    core/AABB3Tests.cpp
    core/ToleranceTests.cpp
    core/StatusResultTests.cpp
)

add_executable(mnx_core_tests ${MNX_CORE_TEST_SOURCES})
target_include_directories(mnx_core_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core")
target_link_libraries(mnx_core_tests PRIVATE mnx::core)

if(GTest_FOUND)
    target_compile_definitions(mnx_core_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_core_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_core_tests)
else()
    add_test(NAME mnx_core_tests COMMAND mnx_core_tests)
endif()

mnx_enable_warnings(mnx_core_tests)
mnx_enable_sanitizers(mnx_core_tests)
mnx_enable_static_analysis(mnx_core_tests)

set(MNX_GEOMETRY_TEST_SOURCES
    geometry/GeometryTestMain.cpp
    geometry/PointTests.cpp
    geometry/Line2DTests.cpp
    geometry/Segment2DTests.cpp
    geometry/Circle2DTests.cpp
    geometry/Arc2DTests.cpp
    geometry/Polyline2DTests.cpp
    geometry/CurveTests.cpp
    geometry/PlaneSurfaceTests.cpp
    geometry/ProjectionTests.cpp
    geometry/Bounds2DTests.cpp
    geometry/IntersectionTests.cpp
)

add_executable(mnx_geometry_tests ${MNX_GEOMETRY_TEST_SOURCES})
target_include_directories(
    mnx_geometry_tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core" "${CMAKE_CURRENT_SOURCE_DIR}/geometry"
)
target_link_libraries(mnx_geometry_tests PRIVATE mnx::geometry)

if(GTest_FOUND)
    target_compile_definitions(mnx_geometry_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_geometry_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_geometry_tests)
else()
    add_test(NAME mnx_geometry_tests COMMAND mnx_geometry_tests)
endif()

mnx_enable_warnings(mnx_geometry_tests)
mnx_enable_sanitizers(mnx_geometry_tests)
mnx_enable_static_analysis(mnx_geometry_tests)

set(MNX_SKETCH_TEST_SOURCES
    sketch/SketchTestMain.cpp
    sketch/SketchEntityTests.cpp
    sketch/GeometryRefTests.cpp
    sketch/SketchTests.cpp
    sketch/SketchPlaneTests.cpp
)

add_executable(mnx_sketch_tests ${MNX_SKETCH_TEST_SOURCES})
target_include_directories(
    mnx_sketch_tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core" "${CMAKE_CURRENT_SOURCE_DIR}/sketch"
)
target_link_libraries(mnx_sketch_tests PRIVATE mnx::sketch)

if(GTest_FOUND)
    target_compile_definitions(mnx_sketch_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_sketch_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_sketch_tests)
else()
    add_test(NAME mnx_sketch_tests COMMAND mnx_sketch_tests)
endif()

mnx_enable_warnings(mnx_sketch_tests)
mnx_enable_sanitizers(mnx_sketch_tests)
mnx_enable_static_analysis(mnx_sketch_tests)

set(MNX_CONSTRAINT_TEST_SOURCES
    constraints/ConstraintTestMain.cpp
    constraints/ConstraintValidationTests.cpp
    constraints/ConstraintMetadataTests.cpp
)

add_executable(mnx_constraint_tests ${MNX_CONSTRAINT_TEST_SOURCES})
target_include_directories(
    mnx_constraint_tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core" "${CMAKE_CURRENT_SOURCE_DIR}/constraints"
)
target_link_libraries(mnx_constraint_tests PRIVATE mnx::constraints)

if(GTest_FOUND)
    target_compile_definitions(mnx_constraint_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_constraint_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_constraint_tests)
else()
    add_test(NAME mnx_constraint_tests COMMAND mnx_constraint_tests)
endif()

mnx_enable_warnings(mnx_constraint_tests)
mnx_enable_sanitizers(mnx_constraint_tests)
mnx_enable_static_analysis(mnx_constraint_tests)

set(MNX_SOLVER_TEST_SOURCES
    solver/SolverTestMain.cpp
    solver/SketchSolverTests.cpp
)

add_executable(mnx_solver_tests ${MNX_SOLVER_TEST_SOURCES})
target_include_directories(
    mnx_solver_tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core" "${CMAKE_CURRENT_SOURCE_DIR}/solver"
)
target_link_libraries(mnx_solver_tests PRIVATE mnx::solver)

if(GTest_FOUND)
    target_compile_definitions(mnx_solver_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_solver_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_solver_tests)
else()
    add_test(NAME mnx_solver_tests COMMAND mnx_solver_tests)
endif()

mnx_enable_warnings(mnx_solver_tests)
mnx_enable_sanitizers(mnx_solver_tests)
mnx_enable_static_analysis(mnx_solver_tests)

set(MNX_COMMAND_TEST_SOURCES
    commands/CommandTestMain.cpp
    commands/CommandTests.cpp
)

add_executable(mnx_command_tests ${MNX_COMMAND_TEST_SOURCES})
target_include_directories(
    mnx_command_tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core" "${CMAKE_CURRENT_SOURCE_DIR}/commands"
)
target_link_libraries(mnx_command_tests PRIVATE mnx::commands)

if(GTest_FOUND)
    target_compile_definitions(mnx_command_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_command_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_command_tests)
else()
    add_test(NAME mnx_command_tests COMMAND mnx_command_tests)
endif()

mnx_enable_warnings(mnx_command_tests)
mnx_enable_sanitizers(mnx_command_tests)
mnx_enable_static_analysis(mnx_command_tests)

set(MNX_MODELING_TEST_SOURCES
    modeling/ModelingTestMain.cpp
    modeling/FeatureGraphTests.cpp
    modeling/ExtrudeFeatureTests.cpp
    modeling/RevolveFeatureTests.cpp
)

add_executable(mnx_modeling_tests ${MNX_MODELING_TEST_SOURCES})
target_include_directories(
    mnx_modeling_tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core" "${CMAKE_CURRENT_SOURCE_DIR}/modeling"
)
target_link_libraries(mnx_modeling_tests PRIVATE mnx::modeling)

if(GTest_FOUND)
    target_compile_definitions(mnx_modeling_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_modeling_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_modeling_tests)
else()
    add_test(NAME mnx_modeling_tests COMMAND mnx_modeling_tests)
endif()

mnx_enable_warnings(mnx_modeling_tests)
mnx_enable_sanitizers(mnx_modeling_tests)
mnx_enable_static_analysis(mnx_modeling_tests)

set(MNX_TOPOLOGY_TEST_SOURCES
    topology/TopologyTestMain.cpp
    topology/TopologyTests.cpp
)

add_executable(mnx_topology_tests ${MNX_TOPOLOGY_TEST_SOURCES})
target_include_directories(
    mnx_topology_tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core" "${CMAKE_CURRENT_SOURCE_DIR}/topology"
)
target_link_libraries(mnx_topology_tests PRIVATE mnx::modeling mnx::topology)

if(GTest_FOUND)
    target_compile_definitions(mnx_topology_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_topology_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_topology_tests)
else()
    add_test(NAME mnx_topology_tests COMMAND mnx_topology_tests)
endif()

mnx_enable_warnings(mnx_topology_tests)
mnx_enable_sanitizers(mnx_topology_tests)
mnx_enable_static_analysis(mnx_topology_tests)

set(MNX_RENDERING_TEST_SOURCES
    rendering/RenderingTestMain.cpp
    rendering/RenderingTests.cpp
)

add_executable(mnx_rendering_tests ${MNX_RENDERING_TEST_SOURCES})
target_include_directories(
    mnx_rendering_tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core" "${CMAKE_CURRENT_SOURCE_DIR}/rendering"
)
target_link_libraries(mnx_rendering_tests PRIVATE mnx::rendering)

if(GTest_FOUND)
    target_compile_definitions(mnx_rendering_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_rendering_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_rendering_tests)
else()
    add_test(NAME mnx_rendering_tests COMMAND mnx_rendering_tests)
endif()

mnx_enable_warnings(mnx_rendering_tests)
mnx_enable_sanitizers(mnx_rendering_tests)
mnx_enable_static_analysis(mnx_rendering_tests)

set(MNX_PERSISTENCE_TEST_SOURCES
    persistence/PersistenceTestMain.cpp
    persistence/PersistenceTests.cpp
)

add_executable(mnx_persistence_tests ${MNX_PERSISTENCE_TEST_SOURCES})
target_include_directories(
    mnx_persistence_tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core" "${CMAKE_CURRENT_SOURCE_DIR}/persistence"
)
target_link_libraries(mnx_persistence_tests PRIVATE mnx::persistence)

if(GTest_FOUND)
    target_compile_definitions(mnx_persistence_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_persistence_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_persistence_tests)
else()
    add_test(NAME mnx_persistence_tests COMMAND mnx_persistence_tests)
endif()

mnx_enable_warnings(mnx_persistence_tests)
mnx_enable_sanitizers(mnx_persistence_tests)
mnx_enable_static_analysis(mnx_persistence_tests)

set(MNX_ASSEMBLY_TEST_SOURCES
    assembly/AssemblyTestMain.cpp
    assembly/AssemblyTests.cpp
)

add_executable(mnx_assembly_tests ${MNX_ASSEMBLY_TEST_SOURCES})
target_include_directories(
    mnx_assembly_tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core" "${CMAKE_CURRENT_SOURCE_DIR}/assembly"
)
target_link_libraries(mnx_assembly_tests PRIVATE mnx::assembly mnx::persistence)

if(GTest_FOUND)
    target_compile_definitions(mnx_assembly_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_assembly_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_assembly_tests)
else()
    add_test(NAME mnx_assembly_tests COMMAND mnx_assembly_tests)
endif()

mnx_enable_warnings(mnx_assembly_tests)
mnx_enable_sanitizers(mnx_assembly_tests)
mnx_enable_static_analysis(mnx_assembly_tests)

set(MNX_SCRIPTING_TEST_SOURCES
    integration/ScriptingSmokeTestMain.cpp
    integration/ScriptingSmokeTests.cpp
)

add_executable(mnx_scripting_tests ${MNX_SCRIPTING_TEST_SOURCES})
target_include_directories(
    mnx_scripting_tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core" "${CMAKE_CURRENT_SOURCE_DIR}/integration"
)
target_link_libraries(mnx_scripting_tests PRIVATE mnx::scripting)

if(GTest_FOUND)
    target_compile_definitions(mnx_scripting_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_scripting_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_scripting_tests)
else()
    add_test(NAME mnx_scripting_tests COMMAND mnx_scripting_tests)
endif()

mnx_enable_warnings(mnx_scripting_tests)
mnx_enable_sanitizers(mnx_scripting_tests)
mnx_enable_static_analysis(mnx_scripting_tests)

set(MNX_SAMPLE_ASSET_TEST_SOURCES
    integration/SampleAssetTestMain.cpp
    integration/SampleAssetTests.cpp
)

add_executable(mnx_sample_asset_tests ${MNX_SAMPLE_ASSET_TEST_SOURCES})
target_include_directories(
    mnx_sample_asset_tests
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/core" "${CMAKE_CURRENT_SOURCE_DIR}/integration"
)
target_link_libraries(mnx_sample_asset_tests PRIVATE mnx::persistence mnx::modeling mnx::assembly)

if(GTest_FOUND)
    target_compile_definitions(mnx_sample_asset_tests PRIVATE MNX_HAS_GTEST=1)
    target_link_libraries(mnx_sample_asset_tests PRIVATE GTest::gtest_main)
    gtest_discover_tests(mnx_sample_asset_tests)
else()
    add_test(NAME mnx_sample_asset_tests COMMAND mnx_sample_asset_tests)
endif()

mnx_enable_warnings(mnx_sample_asset_tests)
mnx_enable_sanitizers(mnx_sample_asset_tests)
mnx_enable_static_analysis(mnx_sample_asset_tests)
