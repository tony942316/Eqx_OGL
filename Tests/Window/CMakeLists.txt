cmake_minimum_required(VERSION 3.28)

add_executable(Test_Window_Smoke)
target_sources(Test_Window_Smoke PRIVATE Main.cpp)
target_compile_features(Test_Window_Smoke PRIVATE cxx_std_20)
target_compile_definitions(Test_Window_Smoke PRIVATE EQX_SMOKE)
target_link_libraries(Test_Window_Smoke PRIVATE
    Eqx_Std Eqx_Lib Eqx_OGL Eqx_Typical)
set_target_properties(Test_Window_Smoke PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

add_executable(Test_Window_Explore)
target_sources(Test_Window_Explore PRIVATE Main.cpp)
target_compile_features(Test_Window_Explore PRIVATE cxx_std_20)
target_link_libraries(Test_Window_Explore PRIVATE
    Eqx_Std Eqx_Lib Eqx_OGL Eqx_Typical)
set_target_properties(Test_Window_Explore PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)

if (Eqx_MSVC)
    add_custom_command(TARGET Test_Window_Smoke POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/clang_rt.asan_dynamic-x86_64.dll
            $<TARGET_FILE_DIR:Test_Window_Smoke>)
elseif (Eqx_Clang OR Eqx_GNU)
else()
    message(FATAL_ERROR "Invalid Configuration!")
endif()

add_test(NAME Test_Window_Smoke
    COMMAND $<TARGET_FILE:Test_Window_Smoke>)
