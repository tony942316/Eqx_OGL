cmake_minimum_required(VERSION 3.28)

add_executable(Test_Texture_Smoke)
target_sources(Test_Texture_Smoke PRIVATE Main.cpp)
target_compile_features(Test_Texture_Smoke PRIVATE cxx_std_20)
target_compile_definitions(Test_Texture_Smoke PRIVATE EQX_SMOKE)
target_link_libraries(Test_Texture_Smoke PRIVATE
    Eqx_Std Eqx_TPL Eqx_Lib Eqx_OGL Eqx_Typical)
set_target_properties(Test_Texture_Smoke PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_Texture_Explore)
target_sources(Test_Texture_Explore PRIVATE Main.cpp)
target_compile_features(Test_Texture_Explore PRIVATE cxx_std_20)
target_link_libraries(Test_Texture_Explore PRIVATE
    Eqx_Std Eqx_TPL Eqx_Lib Eqx_OGL Eqx_Typical)
set_target_properties(Test_Texture_Explore PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/Texture/)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/Texture/Vertex.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/Texture/Vertex.glsl
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/Texture/Fragment.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/Texture/Fragment.glsl
    COPYONLY)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/Box.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/Box.png
    COPYONLY)

if (Eqx_MSVC)
    add_custom_command(TARGET Test_Texture_Smoke POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/clang_rt.asan_dynamic-x86_64.dll
            $<TARGET_FILE_DIR:Test_Texture_Smoke>)
elseif (Eqx_Clang OR Eqx_GNU)
else()
    message(FATAL_ERROR "Invalid Configuration!")
endif()

add_test(NAME Test_Texture_Smoke
    COMMAND $<TARGET_FILE:Test_Texture_Smoke>)
