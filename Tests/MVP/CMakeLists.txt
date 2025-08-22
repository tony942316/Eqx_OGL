cmake_minimum_required(VERSION 3.28)

add_executable(Test_MVP_Smoke)
target_sources(Test_MVP_Smoke PRIVATE Main.cpp)
target_compile_features(Test_MVP_Smoke PRIVATE cxx_std_20)
target_compile_definitions(Test_MVP_Smoke PRIVATE EQX_SMOKE)
target_link_libraries(Test_MVP_Smoke PRIVATE
    Eqx_Std Eqx_TPL Eqx_Lib Eqx_OGL Eqx_Typical)
set_target_properties(Test_MVP_Smoke PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_MVP_Explore)
target_sources(Test_MVP_Explore PRIVATE Main.cpp)
target_compile_features(Test_MVP_Explore PRIVATE cxx_std_20)
target_link_libraries(Test_MVP_Explore PRIVATE
    Eqx_Std Eqx_TPL Eqx_Lib Eqx_OGL Eqx_Typical)
set_target_properties(Test_MVP_Explore PROPERTIES
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

if (Eqx_MSVC)
    add_custom_command(TARGET Test_MVP_Smoke POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/clang_rt.asan_dynamic-x86_64.dll
            $<TARGET_FILE_DIR:Test_MVP_Smoke>)
elseif (Eqx_Clang OR Eqx_GNU)
else()
    message(FATAL_ERROR "Invalid Configuration!")
endif()

add_test(NAME Test_MVP_Smoke
    COMMAND $<TARGET_FILE:Test_MVP_Smoke>)
