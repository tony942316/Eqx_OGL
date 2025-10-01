cmake_minimum_required(VERSION 3.28)

add_executable(Test_View_Smoke)
target_sources(Test_View_Smoke PRIVATE Main.cpp)
target_compile_features(Test_View_Smoke PRIVATE cxx_std_20)
target_compile_definitions(Test_View_Smoke PRIVATE EQX_SMOKE)
target_link_libraries(Test_View_Smoke PRIVATE
    Eqx_Std Eqx_TPL Eqx_Lib Eqx_OGL Eqx_Warnings_Error)
set_target_properties(Test_View_Smoke PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_View_Explore)
target_sources(Test_View_Explore PRIVATE Main.cpp)
target_compile_features(Test_View_Explore PRIVATE cxx_std_20)
target_link_libraries(Test_View_Explore PRIVATE
    Eqx_Std Eqx_TPL Eqx_Lib Eqx_OGL Eqx_Warnings_Error)
set_target_properties(Test_View_Explore PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_View_Smoke_Typical)
target_sources(Test_View_Smoke_Typical PRIVATE Main.cpp)
target_compile_features(Test_View_Smoke_Typical PRIVATE cxx_std_20)
target_compile_definitions(Test_View_Smoke_Typical PRIVATE EQX_SMOKE)
target_link_libraries(Test_View_Smoke_Typical PRIVATE
    Eqx_Std_Typical Eqx_TPL_Typical Eqx_Lib_Typical Eqx_OGL_Typical
    Eqx_Warnings_Error)
set_target_properties(Test_View_Smoke_Typical PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_View_Explore_Typical)
target_sources(Test_View_Explore_Typical PRIVATE Main.cpp)
target_compile_features(Test_View_Explore_Typical PRIVATE cxx_std_20)
target_link_libraries(Test_View_Explore_Typical PRIVATE
    Eqx_Std_Typical Eqx_TPL_Typical Eqx_Lib_Typical Eqx_OGL_Typical
    Eqx_Warnings_Error)
set_target_properties(Test_View_Explore_Typical PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/View/)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/View/Vertex.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/View/Vertex.glsl
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/View/Fragment.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/View/Fragment.glsl
    COPYONLY)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/Brick_Wall.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/Brick_Wall.png
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/Box.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/Box.png
    COPYONLY)

eqx_copy_asan(Test_View_Smoke_Typical)
eqx_copy_asan(Test_View_Explore_Typical)

add_test(NAME Test_View_Smoke
    COMMAND $<TARGET_FILE:Test_View_Smoke>)
add_test(NAME Test_View_Smoke_Typical
    COMMAND $<TARGET_FILE:Test_View_Smoke_Typical>)
