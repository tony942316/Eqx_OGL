# CMakeLists.txt

cmake_minimum_required(VERSION 3.28)

add_executable(Test_KGame_Smoke)
target_sources(Test_KGame_Smoke PRIVATE Main.cpp)
target_compile_features(Test_KGame_Smoke PRIVATE cxx_std_20)
target_compile_definitions(Test_KGame_Smoke PRIVATE EQX_SMOKE)
target_link_libraries(Test_KGame_Smoke PRIVATE
    Eqx_Std Eqx_Lib Eqx_TPL Eqx_OGL Eqx_Warnings_Error)
set_target_properties(Test_KGame_Smoke PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_KGame_Explore)
target_sources(Test_KGame_Explore PRIVATE Main.cpp)
target_compile_features(Test_KGame_Explore PRIVATE cxx_std_20)
target_link_libraries(Test_KGame_Explore PRIVATE
    Eqx_Std Eqx_Lib Eqx_TPL Eqx_OGL Eqx_Warnings_Error)
set_target_properties(Test_KGame_Explore PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_KGame_Smoke_Typical)
target_sources(Test_KGame_Smoke_Typical PRIVATE Main.cpp)
target_compile_features(Test_KGame_Smoke_Typical PRIVATE cxx_std_20)
target_compile_definitions(Test_KGame_Smoke_Typical PRIVATE EQX_SMOKE)
target_link_libraries(Test_KGame_Smoke_Typical PRIVATE
    Eqx_Std_Typical Eqx_Lib_Typical Eqx_TPL_Typical Eqx_OGL_Typical
    Eqx_Warnings_Error)
set_target_properties(Test_KGame_Smoke_Typical PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_KGame_Explore_Typical)
target_sources(Test_KGame_Explore_Typical PRIVATE Main.cpp)
target_compile_features(Test_KGame_Explore_Typical PRIVATE cxx_std_20)
target_link_libraries(Test_KGame_Explore_Typical PRIVATE
    Eqx_Std_Typical Eqx_Lib_Typical Eqx_TPL_Typical Eqx_OGL_Typical
    Eqx_Warnings_Error)
set_target_properties(Test_KGame_Explore_Typical PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/KGame/)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/KGame/Vertex.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/KGame/Vertex.glsl
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/KGame/Fragment.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/KGame/Fragment.glsl
    COPYONLY)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/M4_Sherman_Hull.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/M4_Sherman_Hull.png
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/M4_Sherman_Turret.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/M4_Sherman_Turret.png
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/Panzer3_Hull.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/Panzer3_Hull.png
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/Panzer3_Turret.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/Panzer3_Turret.png
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/Panzer3_Destroyed.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/Panzer3_Destroyed.png
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/TankShell.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/TankShell.png
    COPYONLY)

eqx_copy_asan(Test_KGame_Smoke_Typical)
eqx_copy_asan(Test_KGame_Explore_Typical)

add_test(NAME Test_KGame_Smoke
    COMMAND $<TARGET_FILE:Test_KGame_Smoke>)
add_test(NAME Test_KGame_Smoke_Typical
    COMMAND $<TARGET_FILE:Test_KGame_Smoke_Typical>)
