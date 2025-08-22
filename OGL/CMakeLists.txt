cmake_minimum_required(VERSION 3.28)

FetchContent_Declare(
    Eqx_Lib
    GIT_REPOSITORY https://github.com/tony942316/Eqx_Lib.git
    GIT_TAG main)

FetchContent_Declare(
    Eqx_TPL
    GIT_REPOSITORY https://github.com/tony942316/Eqx_TPL.git
    GIT_TAG main)

FetchContent_Declare(
    Eqx_Std
    GIT_REPOSITORY https://github.com/tony942316/Eqx_Std.git
    GIT_TAG main)

FetchContent_MakeAvailable(Eqx_Lib Eqx_TPL Eqx_Std)

add_library(Eqx_OGL STATIC)
target_sources(Eqx_OGL PUBLIC
    FILE_SET CXX_MODULES FILES
        OGL.cpp
        Buffer.cpp
    #    Context.cpp
        Shader_Program.cpp
        Shader_Source.cpp
        Texture.cpp
        Vertex_Array.cpp
        Window.cpp)
target_compile_features(Eqx_OGL PUBLIC cxx_std_20)
target_link_libraries(Eqx_OGL PRIVATE Eqx_Std Eqx_Lib Eqx_TPL)
set_target_properties(Eqx_OGL PROPERTIES
    CXX_STANDARD_REQUIRED On
    CXX_EXTENSIONS Off
    EXPORT_COMPILE_COMMANDS On)
