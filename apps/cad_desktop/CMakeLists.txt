if(NOT Qt6_FOUND)
    return()
endif()

add_executable(
    mnx_cad_desktop
    main.cpp
    MainWindow.h
    MainWindow.cpp
    FeatureTreeWidget.h
    FeatureTreeWidget.cpp
    AssemblyTreeWidget.h
    AssemblyTreeWidget.cpp
    FeaturePropertyPanel.h
    FeaturePropertyPanel.cpp
    MateEditorPanel.h
    MateEditorPanel.cpp
    AppController.h
    AppController.cpp
)

target_link_libraries(
    mnx_cad_desktop
    PRIVATE Qt6::Widgets
            mnx::core
            mnx::geometry
            mnx::sketch
            mnx::sketch_gui
            mnx::constraints
            mnx::solver
            mnx::commands
            mnx::modeling
            mnx::topology
            mnx::rendering
            mnx::rendering_qt
            mnx::persistence
            mnx::assembly
            mnx::scripting
)

mnx_enable_warnings(mnx_cad_desktop)
mnx_enable_sanitizers(mnx_cad_desktop)
mnx_enable_static_analysis(mnx_cad_desktop)

set_target_properties(mnx_cad_desktop PROPERTIES WIN32_EXECUTABLE TRUE)
