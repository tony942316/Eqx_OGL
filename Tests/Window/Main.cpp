// Main.cpp

import <Eqx/std.hpp>;
import Eqx.Lib;
import Eqx.OGL.Window;

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

    auto window = eqx::ogl::Window{ 1920, 1080, "eqx::ogl --- Test Window"sv };

    while (!window.should_close())
    {
        window.clear();

        loc = window.get_cursor_location();
        window.set_name(std::format(
            "eqx::ogl --- "sv
            "Test Window --- "sv
            "Location: ({}, {}) --- "sv
            "Frames: {} --- "sv
            "FPS: {}"sv,
            loc.get_x(), loc.get_y(), frames, fps));

        if constexpr (c_smoke == true)
        {
            window.close();
        }

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

        window.swap();
        window.poll();
    }

    window.free();
    eqx::ogl::free();

    std::format_to(std::ostream_iterator<char>(std::cout), "\nEnd\n"sv);
    return EXIT_SUCCESS;
}
