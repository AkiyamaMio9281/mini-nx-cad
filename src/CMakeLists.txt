add_library(
    mnx_core
    core/Mat3.cpp
    core/Mat4.cpp
    core/Transform2D.cpp
    core/Transform3D.cpp
)
add_library(mnx::core ALIAS mnx_core)
target_include_directories(
    mnx_core
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
mnx_enable_warnings(mnx_core)
mnx_enable_sanitizers(mnx_core)
mnx_enable_static_analysis(mnx_core)

add_library(
    mnx_geometry
    geometry/Line2D.cpp
    geometry/Segment2D.cpp
    geometry/Circle2D.cpp
    geometry/Arc2D.cpp
    geometry/Polyline2D.cpp
    geometry/Bezier2D.cpp
    geometry/BSpline2D.cpp
    geometry/Plane.cpp
    geometry/PlanarSurface.cpp
    geometry/algorithms/Intersections2D.cpp
    geometry/algorithms/Projections2D.cpp
    geometry/algorithms/Distances2D.cpp
    geometry/algorithms/Bounds2D.cpp
    geometry/algorithms/CurveSampling.cpp
)
add_library(mnx::geometry ALIAS mnx_geometry)
target_include_directories(
    mnx_geometry
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(mnx_geometry PUBLIC mnx::core)
mnx_enable_warnings(mnx_geometry)
mnx_enable_sanitizers(mnx_geometry)
mnx_enable_static_analysis(mnx_geometry)

add_library(
    mnx_sketch
    sketch/SketchEntity.cpp
    sketch/SelectionState.cpp
    sketch/SketchPoint.cpp
    sketch/SketchLine.cpp
    sketch/SketchCircle.cpp
    sketch/SketchArc.cpp
    sketch/SketchSpline.cpp
    sketch/SketchPlane.cpp
    sketch/Sketch.cpp
)
add_library(mnx::sketch ALIAS mnx_sketch)
target_include_directories(
    mnx_sketch
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(mnx_sketch PUBLIC mnx::core mnx::geometry)
mnx_enable_warnings(mnx_sketch)
mnx_enable_sanitizers(mnx_sketch)
mnx_enable_static_analysis(mnx_sketch)

if(Qt6_FOUND)
    add_library(
        mnx_sketch_gui
        sketch/gui/SketchView.cpp
        sketch/gui/SketchController.cpp
        sketch/gui/SketchRenderer.cpp
        sketch/gui/SelectionManager.cpp
        sketch/gui/SnapManager.cpp
        sketch/gui/SelectTool.cpp
        sketch/gui/LineTool.cpp
        sketch/gui/CircleTool.cpp
        sketch/gui/RectangleTool.cpp
    )
    add_library(mnx::sketch_gui ALIAS mnx_sketch_gui)
    target_include_directories(
        mnx_sketch_gui
        PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
    target_link_libraries(mnx_sketch_gui PUBLIC mnx::sketch Qt6::Widgets)
    mnx_enable_warnings(mnx_sketch_gui)
    mnx_enable_sanitizers(mnx_sketch_gui)
    mnx_enable_static_analysis(mnx_sketch_gui)
endif()

add_library(
    mnx_constraints
    constraints/Constraint.cpp
    constraints/CoincidentConstraint.cpp
    constraints/HorizontalConstraint.cpp
    constraints/VerticalConstraint.cpp
    constraints/ParallelConstraint.cpp
    constraints/PerpendicularConstraint.cpp
    constraints/DistanceConstraint.cpp
    constraints/AngleConstraint.cpp
    constraints/RadiusConstraint.cpp
    constraints/TangentConstraint.cpp
    constraints/EqualLengthConstraint.cpp
    constraints/Dimension.cpp
)
add_library(mnx::constraints ALIAS mnx_constraints)
target_include_directories(
    mnx_constraints
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(mnx_constraints PUBLIC mnx::core mnx::geometry mnx::sketch)
mnx_enable_warnings(mnx_constraints)
mnx_enable_sanitizers(mnx_constraints)
mnx_enable_static_analysis(mnx_constraints)

add_library(
    mnx_solver
    solver/SolverDiagnostics.cpp
    solver/SolveResult.cpp
    solver/SolverVariable.cpp
    solver/VariableRegistry.cpp
    solver/ConstraintEquation.cpp
    solver/EquationBuilder.cpp
    solver/JacobianBuilder.cpp
    solver/SketchSolver.cpp
)
add_library(mnx::solver ALIAS mnx_solver)
target_include_directories(
    mnx_solver
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(mnx_solver PUBLIC mnx::core mnx::geometry mnx::sketch mnx::constraints)
mnx_enable_warnings(mnx_solver)
mnx_enable_sanitizers(mnx_solver)
mnx_enable_static_analysis(mnx_solver)

add_library(
    mnx_commands
    commands/CommandManager.cpp
    commands/CompositeCommand.cpp
    commands/CreateSketchEntityCommand.cpp
    commands/DeleteSketchEntityCommand.cpp
    commands/MoveSketchHandleCommand.cpp
    commands/AddConstraintCommand.cpp
    commands/RemoveConstraintCommand.cpp
    commands/EditDimensionCommand.cpp
)
add_library(mnx::commands ALIAS mnx_commands)
target_include_directories(
    mnx_commands
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(mnx_commands PUBLIC mnx::core mnx::geometry mnx::sketch mnx::constraints mnx::solver)
mnx_enable_warnings(mnx_commands)
mnx_enable_sanitizers(mnx_commands)
mnx_enable_static_analysis(mnx_commands)

add_library(
    mnx_topology
    topology/Vertex.cpp
    topology/Edge.cpp
    topology/CoEdge.cpp
    topology/Loop.cpp
    topology/Face.cpp
    topology/Shell.cpp
    topology/Body.cpp
    topology/TopologyValidator.cpp
    topology/TopologyQueries.cpp
)
add_library(mnx::topology ALIAS mnx_topology)
target_include_directories(
    mnx_topology
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(mnx_topology PUBLIC mnx::core mnx::geometry)
mnx_enable_warnings(mnx_topology)
mnx_enable_sanitizers(mnx_topology)
mnx_enable_static_analysis(mnx_topology)

add_library(
    mnx_modeling
    modeling/Feature.cpp
    modeling/FeatureContext.cpp
    modeling/FeatureResult.cpp
    modeling/DisplayMesh.cpp
    modeling/Body.cpp
    modeling/ProfileUtils.cpp
    modeling/RevolveAxis2D.cpp
    modeling/MeshBuilder.cpp
    modeling/BodyBuilder.cpp
    modeling/ExtrudeBuilder.cpp
    modeling/RevolveBuilder.cpp
    modeling/FeatureGraph.cpp
    modeling/RebuildEngine.cpp
    modeling/SketchFeature.cpp
    modeling/ExtrudeFeature.cpp
    modeling/RevolveFeature.cpp
    modeling/PartDocument.cpp
)
add_library(mnx::modeling ALIAS mnx_modeling)
target_include_directories(
    mnx_modeling
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(mnx_modeling PUBLIC mnx::core mnx::geometry mnx::sketch mnx::constraints mnx::solver mnx::topology)
mnx_enable_warnings(mnx_modeling)
mnx_enable_sanitizers(mnx_modeling)
mnx_enable_static_analysis(mnx_modeling)

add_library(
    mnx_rendering
    rendering/Camera.cpp
    rendering/Mesh.cpp
    rendering/MeshBuffer.cpp
    rendering/RenderItem.cpp
    rendering/RenderScene.cpp
    rendering/BodyRenderer.cpp
    rendering/Picking.cpp
)
add_library(mnx::rendering ALIAS mnx_rendering)
target_include_directories(
    mnx_rendering
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(mnx_rendering PUBLIC mnx::core mnx::geometry mnx::topology mnx::modeling)
mnx_enable_warnings(mnx_rendering)
mnx_enable_sanitizers(mnx_rendering)
mnx_enable_static_analysis(mnx_rendering)

if(Qt6_FOUND)
    add_library(mnx_rendering_qt rendering/SceneView.cpp)
    add_library(mnx::rendering_qt ALIAS mnx_rendering_qt)
    target_include_directories(
        mnx_rendering_qt
        PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
    )
    target_link_libraries(mnx_rendering_qt PUBLIC mnx::rendering Qt6::Widgets Qt6::OpenGLWidgets)
    if(OpenGL_FOUND)
        target_link_libraries(mnx_rendering_qt PUBLIC OpenGL::GL)
    endif()
    mnx_enable_warnings(mnx_rendering_qt)
    mnx_enable_sanitizers(mnx_rendering_qt)
    mnx_enable_static_analysis(mnx_rendering_qt)
endif()

add_library(
    mnx_assembly
    assembly/TransformNode.cpp
    assembly/ComponentInstance.cpp
    assembly/MateConstraint.cpp
    assembly/AssemblyDocument.cpp
    assembly/AssemblySolver.cpp
    assembly/AssemblyQueries.cpp
)
add_library(mnx::assembly ALIAS mnx_assembly)
target_include_directories(
    mnx_assembly
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(mnx_assembly PUBLIC mnx::core mnx::geometry mnx::modeling mnx::topology)
mnx_enable_warnings(mnx_assembly)
mnx_enable_sanitizers(mnx_assembly)
mnx_enable_static_analysis(mnx_assembly)

add_library(
    mnx_scripting
    scripting/PythonBindingsPart.cpp
    scripting/PythonBindingsSketch.cpp
    scripting/PythonBindingsFeature.cpp
    scripting/PythonBindingsExport.cpp
)
add_library(mnx::scripting ALIAS mnx_scripting)
target_include_directories(
    mnx_scripting
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_library(
    mnx_persistence
    persistence/JsonConverters.cpp
    persistence/ProjectSerializer.cpp
    persistence/ProjectDeserializer.cpp
    persistence/PersistenceUtils.cpp
)
add_library(mnx::persistence ALIAS mnx_persistence)
target_include_directories(
    mnx_persistence
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
target_link_libraries(
    mnx_persistence
    PUBLIC mnx::core mnx::geometry mnx::sketch mnx::constraints mnx::solver mnx::commands mnx::modeling
           mnx::topology mnx::assembly
)
mnx_enable_warnings(mnx_persistence)
mnx_enable_sanitizers(mnx_persistence)
mnx_enable_static_analysis(mnx_persistence)

target_link_libraries(
    mnx_scripting
    PUBLIC mnx::core mnx::geometry mnx::sketch mnx::constraints mnx::solver mnx::commands
           mnx::modeling mnx::topology mnx::persistence mnx::assembly
)
mnx_enable_warnings(mnx_scripting)
mnx_enable_sanitizers(mnx_scripting)
mnx_enable_static_analysis(mnx_scripting)

if(pybind11_FOUND)
    target_compile_definitions(mnx_scripting PRIVATE MNX_HAS_PYBIND11=1)
    target_link_libraries(mnx_scripting PRIVATE pybind11::headers)
    pybind11_add_module(mini_nx_python scripting/PythonModule.cpp)
    target_compile_definitions(mini_nx_python PRIVATE MNX_HAS_PYBIND11=1)
    target_link_libraries(mini_nx_python PRIVATE mnx::scripting)
    set_target_properties(mini_nx_python PROPERTIES OUTPUT_NAME mini_nx)
    mnx_enable_warnings(mini_nx_python)
    mnx_enable_sanitizers(mini_nx_python)
    mnx_enable_static_analysis(mini_nx_python)
endif()
