cmake_minimum_required(VERSION 3.28)

add_executable(Test_MVP_Smoke)
target_sources(Test_MVP_Smoke PRIVATE Main.cpp)
target_compile_features(Test_MVP_Smoke PRIVATE cxx_std_20)
target_compile_definitions(Test_MVP_Smoke PRIVATE EQX_SMOKE)
target_link_libraries(Test_MVP_Smoke PRIVATE
    Eqx_Std Eqx_TPL Eqx_Lib Eqx_OGL Eqx_Warnings_Error)
set_target_properties(Test_MVP_Smoke PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_MVP_Explore)
target_sources(Test_MVP_Explore PRIVATE Main.cpp)
target_compile_features(Test_MVP_Explore PRIVATE cxx_std_20)
target_link_libraries(Test_MVP_Explore PRIVATE
    Eqx_Std Eqx_TPL Eqx_Lib Eqx_OGL Eqx_Warnings_Error)
set_target_properties(Test_MVP_Explore PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_MVP_Smoke_Typical)
target_sources(Test_MVP_Smoke_Typical PRIVATE Main.cpp)
target_compile_features(Test_MVP_Smoke_Typical PRIVATE cxx_std_20)
target_compile_definitions(Test_MVP_Smoke_Typical PRIVATE EQX_SMOKE)
target_link_libraries(Test_MVP_Smoke_Typical PRIVATE
    Eqx_Std_Typical Eqx_TPL_Typical Eqx_Lib_Typical Eqx_OGL_Typical
    Eqx_Warnings_Error)
set_target_properties(Test_MVP_Smoke_Typical PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_MVP_Explore_Typical)
target_sources(Test_MVP_Explore_Typical PRIVATE Main.cpp)
target_compile_features(Test_MVP_Explore_Typical PRIVATE cxx_std_20)
target_link_libraries(Test_MVP_Explore_Typical PRIVATE
    Eqx_Std_Typical Eqx_TPL_Typical Eqx_Lib_Typical Eqx_OGL_Typical
    Eqx_Warnings_Error)
set_target_properties(Test_MVP_Explore_Typical PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/MVP/)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/MVP/Vertex.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/MVP/Vertex.glsl
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/MVP/Fragment.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/MVP/Fragment.glsl
    COPYONLY)

eqx_copy_asan(Test_MVP_Smoke_Typical)
eqx_copy_asan(Test_MVP_Explore_Typical)

add_test(NAME Test_MVP_Smoke
    COMMAND $<TARGET_FILE:Test_MVP_Smoke>)
add_test(NAME Test_MVP_Smoke_Typical
    COMMAND $<TARGET_FILE:Test_MVP_Smoke_Typical>)
