// Main.cpp

import <Eqx/std.hpp>;
import Eqx.Lib;
import Eqx.OGL.Window;
import Eqx.OGL.Shader_Program;
import Eqx.OGL.Vertex_Array;

using namespace std::literals;

#ifdef EQX_SMOKE
    constexpr auto c_smoke = true;
#else
    constexpr auto c_smoke = false;
#endif // EQX_SMOKE

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    std::format_to(std::ostream_iterator<char>(std::cout), "Start\n\n"sv);

    eqx::ogl::init();

    auto loc = eqx::lib::Point<float>{};
    auto start = std::chrono::steady_clock::now();
    auto end = start;
    auto frames = 0ull;
    auto fps = 0.0f;

    auto window = eqx::ogl::Window{ 1920, 1080, "eqx::ogl --- Test Quad"sv };
    auto shader_program = eqx::ogl::Shader_Program::from_files(
        "./Resources/Shaders/Quad/Vertex.glsl"sv,
        "./Resources/Shaders/Quad/Fragment.glsl"sv);
    auto vertex_array = eqx::ogl::Vertex_Array{
        std::array<float, 24>{
            0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f },
        std::array<unsigned int, 2>{ 2u, 4u },
        std::array<unsigned int, 6>{
            0u, 1u, 2u,
            1u, 3u, 2u }};

    while (!window.should_close())
    {
        window.clear();

        loc = window.get_cursor_location();
        window.set_name(std::format(
            "eqx::ogl --- "sv
            "Test Quad --- "sv
            "Location: ({}, {}) --- "sv
            "Frames: {} --- "sv
            "FPS: {}"sv,
            loc.get_x(), loc.get_y(), frames, fps));

        shader_program.enable();
        vertex_array.enable();
        eqx::ogl::draw(vertex_array.get_index_count());

        if constexpr (c_smoke == true)
        {
            window.close();
        }

        window.swap();
        window.poll();

        ++frames;
        end = std::chrono::steady_clock::now();
        if ((end - start) > 1'000ms) [[unlikely]]
        {
            fps = (frames * 1'000.0f)
                / std::chrono::duration_cast<std::chrono::milliseconds>(
                    end - start).count();
            frames = 0ull;

            start = end;
        }
    }

    shader_program.free();
    vertex_array.free();
    window.free();
    eqx::ogl::free();

    std::format_to(std::ostream_iterator<char>(std::cout), "\nEnd\n"sv);
    return EXIT_SUCCESS;
}
