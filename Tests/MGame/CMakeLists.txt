# CMakeLists.txt

cmake_minimum_required(VERSION 3.28)

add_executable(Test_MGame_Smoke)
target_sources(Test_MGame_Smoke PRIVATE Main.cpp)
target_compile_features(Test_MGame_Smoke PRIVATE cxx_std_20)
target_compile_definitions(Test_MGame_Smoke PRIVATE EQX_SMOKE)
target_link_libraries(Test_MGame_Smoke PRIVATE
    Eqx_Std Eqx_Lib Eqx_OGL Eqx_Warnings_Error)
set_target_properties(Test_MGame_Smoke PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_MGame_Explore)
target_sources(Test_MGame_Explore PRIVATE Main.cpp)
target_compile_features(Test_MGame_Explore PRIVATE cxx_std_20)
target_link_libraries(Test_MGame_Explore PRIVATE
    Eqx_Std Eqx_Lib Eqx_OGL Eqx_Warnings_Error)
set_target_properties(Test_MGame_Explore PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_MGame_Smoke_Typical)
target_sources(Test_MGame_Smoke_Typical PRIVATE Main.cpp)
target_compile_features(Test_MGame_Smoke_Typical PRIVATE cxx_std_20)
target_compile_definitions(Test_MGame_Smoke_Typical PRIVATE EQX_SMOKE)
target_link_libraries(Test_MGame_Smoke_Typical PRIVATE
    Eqx_Std_Typical Eqx_Lib_Typical Eqx_OGL_Typical Eqx_Warnings_Error)
set_target_properties(Test_MGame_Smoke_Typical PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_MGame_Explore_Typical)
target_sources(Test_MGame_Explore_Typical PRIVATE Main.cpp)
target_compile_features(Test_MGame_Explore_Typical PRIVATE cxx_std_20)
target_link_libraries(Test_MGame_Explore_Typical PRIVATE
    Eqx_Std_Typical Eqx_Lib_Typical Eqx_OGL_Typical Eqx_Warnings_Error)
set_target_properties(Test_MGame_Explore_Typical PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

eqx_copy_asan(Test_MGame_Smoke_Typical)
eqx_copy_asan(Test_MGame_Explore_Typical)

add_test(NAME Test_MGame_Smoke
    COMMAND $<TARGET_FILE:Test_MGame_Smoke>)
add_test(NAME Test_MGame_Smoke_Typical
    COMMAND $<TARGET_FILE:Test_MGame_Smoke_Typical>)
