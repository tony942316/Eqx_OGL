# CMakeLists.txt

cmake_minimum_required(VERSION 3.28)

add_executable(Test_MGame_Smoke)
target_sources(Test_MGame_Smoke PRIVATE Main.cpp)
target_compile_features(Test_MGame_Smoke PRIVATE cxx_std_20)
target_compile_definitions(Test_MGame_Smoke PRIVATE EQX_SMOKE)
target_link_libraries(Test_MGame_Smoke PRIVATE
    Eqx_Std Eqx_Lib Eqx_TPL Eqx_OGL Eqx_Warnings_Error)
set_target_properties(Test_MGame_Smoke PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_MGame_Explore)
target_sources(Test_MGame_Explore PRIVATE Main.cpp)
target_compile_features(Test_MGame_Explore PRIVATE cxx_std_20)
target_link_libraries(Test_MGame_Explore PRIVATE
    Eqx_Std Eqx_Lib Eqx_TPL Eqx_OGL Eqx_Warnings_Error)
set_target_properties(Test_MGame_Explore PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_MGame_Smoke_Typical)
target_sources(Test_MGame_Smoke_Typical PRIVATE Main.cpp)
target_compile_features(Test_MGame_Smoke_Typical PRIVATE cxx_std_20)
target_compile_definitions(Test_MGame_Smoke_Typical PRIVATE EQX_SMOKE)
target_link_libraries(Test_MGame_Smoke_Typical PRIVATE
    Eqx_Std_Typical Eqx_Lib_Typical Eqx_TPL_Typical Eqx_OGL_Typical
    Eqx_Warnings_Error)
set_target_properties(Test_MGame_Smoke_Typical PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_MGame_Explore_Typical)
target_sources(Test_MGame_Explore_Typical PRIVATE Main.cpp)
target_compile_features(Test_MGame_Explore_Typical PRIVATE cxx_std_20)
target_link_libraries(Test_MGame_Explore_Typical PRIVATE
    Eqx_Std_Typical Eqx_Lib_Typical Eqx_TPL_Typical Eqx_OGL_Typical
    Eqx_Warnings_Error)
set_target_properties(Test_MGame_Explore_Typical PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/MGame/)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/MGame/Vertex.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/MGame/Vertex.glsl
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Shaders/MGame/Fragment.glsl
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/MGame/Fragment.glsl
    COPYONLY)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/Panzer3_Hull.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/Panzer3_Hull.png
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/Panzer3_Turret.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/Panzer3_Turret.png
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/M4_Sherman_Hull.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/M4_Sherman_Hull.png
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/M4_Sherman_Turret.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/M4_Sherman_Turret.png
    COPYONLY)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Textures/TankShell.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Textures/TankShell.png
    COPYONLY)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Resources/Maps/)
configure_file(
    ${CMAKE_SOURCE_DIR}/Resources/Maps/MGame.png
    ${CMAKE_CURRENT_BINARY_DIR}/Resources/Maps/MGame.png
    COPYONLY)

eqx_copy_asan(Test_MGame_Smoke_Typical)
eqx_copy_asan(Test_MGame_Explore_Typical)

add_test(NAME Test_MGame_Smoke
    COMMAND $<TARGET_FILE:Test_MGame_Smoke>)
add_test(NAME Test_MGame_Smoke_Typical
    COMMAND $<TARGET_FILE:Test_MGame_Smoke_Typical>)
