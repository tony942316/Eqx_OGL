// Main.cpp

import <Eqx/std.hpp>;
import <Eqx/TPL/glm/glm.hpp>;
import Eqx.Lib;
import Eqx.OGL.Window;
import Eqx.OGL.Shader_Program;
import Eqx.OGL.Vertex_Array;
import Eqx.OGL.Texture;

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

    auto window = eqx::ogl::Window{ 1920, 1080, "eqx::OGL --- Test View"sv };
    auto shader_program = eqx::ogl::Shader_Program::from_files(
        "./Resources/Shaders/View/Vertex.glsl"sv,
        "./Resources/Shaders/View/Fragment.glsl"sv);
    auto vertex_array = eqx::ogl::Vertex_Array{
        std::array<float, 16>{
            0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f },
        std::array<unsigned int, 2>{ 2u, 2u },
        std::array<unsigned int, 6>{
            0u, 1u, 2u,
            1u, 3u, 2u } };
    auto texture = eqx::ogl::Texture{ "./Resources/Textures/Brick_Wall.png"sv };
    auto texture2 = eqx::ogl::Texture{ "./Resources/Textures/Box.png"sv };

    auto model = glm::translate(glm::mat4{ 1.0f },
        glm::vec3{ 0.0f, 0.0f, 0.0f });
    model = glm::scale(model, glm::vec3{ 100.0f, 100.0f, 1.0f });
    auto view = glm::translate(glm::mat4{ 1.0f },
        glm::vec3{ 100.0f, -100.0f, 0.0f });
    view = glm::scale(view, glm::vec3{ 1.0f, 1.0f, 1.0f });
    auto proj = glm::ortho(0.0f, 1920.0f, -1080.0f, 0.0f);
    shader_program.set_mat4("u_model"sv, model);
    shader_program.set_mat4("u_view"sv, view);
    shader_program.set_mat4("u_proj"sv, proj);

    shader_program.activate_texture("u_tex0"sv, 0);

    auto box_loc = eqx::lib::Point<float>{ 100.0f, -100.0f };
    auto box_vel = eqx::lib::Point<float>{ 0.0f, 0.0f };
    auto box_acc = eqx::lib::Point<float>{ 0.0f, 0.0f };

    auto box_loc2 = eqx::lib::Point<float>{ 600.0f, -600.0f };
    auto box_vel2 = eqx::lib::Point<float>{ 0.0f, 0.0f };
    auto box_acc2 = eqx::lib::Point<float>{ 0.0f, 0.0f };

    auto view_scale = eqx::lib::Point<float>{ 1.0f, 1.0f };

    while (!window.should_close())
    {
        window.clear();

        loc = window.get_cursor_location();
        window.set_name(std::format(
            "eqx::ogl --- "sv
            "Test View --- "sv
            "Location: ({}, {}) --- "sv
            "Frames: {} --- "sv
            "FPS: {}"sv,
            loc.get_x(), loc.get_y(), frames, fps));

        if (window.key_down(eqx::ogl::Window::Key::W)
            && window.key_down(eqx::ogl::Window::Key::S))
        {
            box_acc.set_y(0.0f);
        }
        else if (window.key_up(eqx::ogl::Window::Key::W)
            && window.key_up(eqx::ogl::Window::Key::S))
        {
            box_acc.set_y(0.0f);
        }
        else if (window.key_down(eqx::ogl::Window::Key::W))
        {
            box_acc.set_y(0.1f);
        }
        else if (window.key_down(eqx::ogl::Window::Key::S))
        {
            box_acc.set_y(-0.1f);
        }

        if (window.key_down(eqx::ogl::Window::Key::A)
            && window.key_down(eqx::ogl::Window::Key::D))
        {
            box_acc.set_x(0.0f);
        }
        else if (window.key_up(eqx::ogl::Window::Key::A)
            && window.key_up(eqx::ogl::Window::Key::D))
        {
            box_acc.set_x(0.0f);
        }
        else if (window.key_down(eqx::ogl::Window::Key::A))
        {
            box_acc.set_x(-0.1f);
        }
        else if (window.key_down(eqx::ogl::Window::Key::D))
        {
            box_acc.set_x(0.1f);
        }

        if (window.key_down(eqx::ogl::Window::Key::Up))
        {
            view_scale.trans(eqx::lib::Point<float>{ 0.1f, 0.1f });
        }

        if (window.key_down(eqx::ogl::Window::Key::Down))
        {
            view_scale.trans(eqx::lib::Point<float>{ -0.1f, -0.1f });
        }

        box_vel.trans(box_acc);
        box_vel.set_xy(std::ranges::clamp(box_vel.get_x(), -5.0f, 5.0f),
            std::ranges::clamp(box_vel.get_y(), -5.0f, 5.0f));

        box_loc.trans(box_vel);

        model = glm::translate(glm::mat4{ 1.0f },
            glm::vec3{ box_loc.get_x(), box_loc.get_y(), 0.0f });
        model = glm::scale(model, glm::vec3{ 100.0f, 100.0f, 1.0f });
        shader_program.set_mat4("u_model"sv, model);

        view = glm::translate(glm::mat4{ 1.0f },
            glm::vec3{ box_loc.get_x() - (1920.0f / 2.0f), box_loc.get_y() + (1080.0f / 2.0f), 0.0f });
        view = glm::scale(view, glm::vec3{ view_scale.get_x(), view_scale.get_y(), 1.0f });
        view = glm::inverse(view);
        shader_program.set_mat4("u_view"sv, view);

        shader_program.enable();
        vertex_array.enable();
        texture.enable(0);
        eqx::ogl::draw(vertex_array.get_index_count());

        model = glm::translate(glm::mat4{ 1.0f },
            glm::vec3{ box_loc2.get_x(), box_loc2.get_y(), 0.0f });
        model = glm::scale(model, glm::vec3{ 100.0f, 100.0f, 1.0f });
        shader_program.set_mat4("u_model"sv, model);

        shader_program.enable();
        vertex_array.enable();
        texture2.enable(0);
        eqx::ogl::draw(vertex_array.get_index_count());

        if (window.key_down(eqx::ogl::Window::Key::Escape))
        {
            window.close();
        }

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

    texture.free();
    shader_program.free();
    vertex_array.free();
    window.free();
    eqx::ogl::free();

    std::format_to(std::ostream_iterator<char>(std::cout), "\nEnd\n"sv);
    return EXIT_SUCCESS;
}
