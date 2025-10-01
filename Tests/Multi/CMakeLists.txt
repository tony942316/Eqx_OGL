cmake_minimum_required(VERSION 3.28)

add_executable(Test_Multi_Smoke)
target_sources(Test_Multi_Smoke PRIVATE Main.cpp)
target_compile_features(Test_Multi_Smoke PRIVATE cxx_std_20)
target_compile_definitions(Test_Multi_Smoke PRIVATE EQX_SMOKE)
target_link_libraries(Test_Multi_Smoke PRIVATE
    Eqx_Std Eqx_TPL Eqx_Lib Eqx_OGL Eqx_Warnings_Error)
set_target_properties(Test_Multi_Smoke PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_Multi_Explore)
target_sources(Test_Multi_Explore PRIVATE Main.cpp)
target_compile_features(Test_Multi_Explore PRIVATE cxx_std_20)
target_link_libraries(Test_Multi_Explore PRIVATE
    Eqx_Std Eqx_TPL Eqx_Lib Eqx_OGL Eqx_Warnings_Error)
set_target_properties(Test_Multi_Explore PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_Multi_Smoke_Typical)
target_sources(Test_Multi_Smoke_Typical PRIVATE Main.cpp)
target_compile_features(Test_Multi_Smoke_Typical PRIVATE cxx_std_20)
target_compile_definitions(Test_Multi_Smoke_Typical PRIVATE EQX_SMOKE)
target_link_libraries(Test_Multi_Smoke_Typical PRIVATE
    Eqx_Std_Typical Eqx_TPL_Typical Eqx_Lib_Typical Eqx_OGL_Typical
    Eqx_Warnings_Error)
set_target_properties(Test_Multi_Smoke_Typical PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_Multi_Explore_Typical)
target_sources(Test_Multi_Explore_Typical PRIVATE Main.cpp)
target_compile_features(Test_Multi_Explore_Typical PRIVATE cxx_std_20)
target_link_libraries(Test_Multi_Explore_Typical PRIVATE
    Eqx_Std_Typical Eqx_TPL_Typical Eqx_Lib_Typical Eqx_OGL_Typical
    Eqx_Warnings_Error)
set_target_properties(Test_Multi_Explore_Typical PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/Multi/)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/Multi/Vertex.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/Multi/Vertex.glsl
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/Multi/Fragment.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/Multi/Fragment.glsl
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

eqx_copy_asan(Test_Multi_Smoke_Typical)
eqx_copy_asan(Test_Multi_Explore_Typical)

add_test(NAME Test_Multi_Smoke
    COMMAND $<TARGET_FILE:Test_Multi_Smoke>)
add_test(NAME Test_Multi_Smoke_Typical
    COMMAND $<TARGET_FILE:Test_Multi_Smoke_Typical>)
