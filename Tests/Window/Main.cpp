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

    auto window = eqx::ogl::Window{ 1920, 1080, "eqx::ogl --- Test Window"sv };

    auto frame_timer = eqx::ogl::Frame_Timer{};
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
            loc.get_x(), loc.get_y(), frame_timer.get_frames(),
            frame_timer.get_fps()));

        if constexpr (c_smoke == true)
        {
            window.close();
        }

        frame_timer.update();
        window.swap();
        window.poll();
    }

    window.free();
    eqx::ogl::free();

    std::format_to(std::ostream_iterator<char>(std::cout), "\nEnd\n"sv);
    return EXIT_SUCCESS;
}
