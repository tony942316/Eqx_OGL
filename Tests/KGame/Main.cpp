// Main.cpp

import <Eqx/std.hpp>;
import <Eqx/TPL/glm/glm.hpp>;
import Eqx.Lib;
import Eqx.OGL;

using namespace std::literals;

#ifdef EQX_SMOKE
    constexpr auto c_smoke = true;
#else
    constexpr auto c_smoke = false;
#endif // EQX_SMOKE

class Sim
{
public:
    Sim(const Sim&) = default;
    Sim(Sim&&) = default;
    Sim& operator= (const Sim&) = default;
    Sim& operator= (Sim&&) = default;
    ~Sim() = default;

    enum class Turning_State : int
    {
        Left = 1,
        Right = -1,
        Stop = 0
    };

    explicit constexpr Sim() noexcept
        :
        m_player(),
        m_angv(0.0F),
        m_mangv(80.0F),
        m_alive(true)
    {
        this->m_player = eqx::lib::Polygon<float, 4>{
            eqx::lib::Point<float>{ 200.0F, 200.0F },
            eqx::lib::Point<float>{ -200.0F, 200.0F },
            eqx::lib::Point<float>{ -200.0F, -200.0F },
            eqx::lib::Point<float>{ 200.0F, -200.0F } };
        this->m_player.move(eqx::lib::Point<float>{ 1700.0F, -540.0F });
        this->m_player.rotate(std::numbers::pi_v<float> / 2.0F);

        this->m_panzer = eqx::lib::Polygon<float, 4>{
            eqx::lib::Point<float>{ 200.0F, 200.0F },
            eqx::lib::Point<float>{ -200.0F, 200.0F },
            eqx::lib::Point<float>{ -200.0F, -200.0F },
            eqx::lib::Point<float>{ 200.0F, -200.0F } };
        this->m_panzer.move(eqx::lib::Point<float>{ 400.0F, -540.0F });
        this->m_panzer.rotate(-std::numbers::pi_v<float> / 2.0F);
    }

    constexpr void update(const float dt) noexcept
    {
        auto pivot = eqx::lib::Point<float>{
            1700.0F + 80.0F, -540.0F };

        m_angv += 50.0F * static_cast<int>(this->m_turning_state) * dt;
        m_angv = std::ranges::clamp(m_angv, -m_mangv, m_mangv);

        auto fric = std::ranges::min(25.0F * dt, eqx::lib::Math::abs(m_angv));
        fric = m_angv >= 0.0F ? -fric : fric;
        m_angv += fric;

        m_player.rotate(eqx::lib::Math::to_radians(m_angv * dt), pivot);

        if (this->m_shell.has_value())
        {
            if (this->m_shell->aabb(this->m_panzer))
            {
                this->m_alive = false;
                this->m_shell.reset();
                return;
            }

            this->m_shell->translate(
                eqx::lib::Point<float>::scale(this->m_shellv, dt));

            if (this->m_shell->center().get_x() < -50.0F
                || this->m_shell->center().get_y() > 50.0F
                || this->m_shell->center().get_x() > 2100.0F
                || this->m_shell->center().get_y() < -1200.0F)
            {
                this->m_shell.reset();
            }
        }
    }

    constexpr void set_turning_state(const Turning_State state) noexcept
    {
        this->m_turning_state = state;
    }

    constexpr void fire() noexcept
    {
        if (!this->m_shell.has_value())
        {
            auto dir = eqx::lib::Point<float>::translate(
                eqx::lib::Point<float>::midpoint(
                    this->m_player.get_data()[0], this->m_player.get_data()[1]),
                eqx::lib::Point<float>::negate(this->m_player.center()));
            dir.normalize();

            auto dest = eqx::lib::Point<float>::midpoint(
                this->m_player.get_data()[0], this->m_player.get_data()[1]);
            dest.translate(eqx::lib::Point<float>::negate(
                        eqx::lib::Point<float>::scale(dir, 100.0F)));

            m_shell.emplace();
            this->m_shell.emplace(eqx::lib::Polygon<float, 4>{
                eqx::lib::Point<float>{ 50.0F, 50.0F },
                eqx::lib::Point<float>{ -50.0F, 50.0F },
                eqx::lib::Point<float>{ -50.0F, -50.0F },
                eqx::lib::Point<float>{ 50.0F, -50.0F } });
            this->m_shell->move(dest);
            this->m_shell->rotate(std::atan2f(dir.get_y(), dir.get_x())
                - (std::numbers::pi_v<float> / 2.0F));
            this->m_shellv = eqx::lib::Point<float>::scale(dir, 5'000.0F);
        }
    }

    constexpr void reset() noexcept
    {
        this->m_alive = true;
    }

    [[nodiscard]] constexpr eqx::lib::Point<float>
        get_player_location() const noexcept
    {
        return this->m_player.center();
    }

    [[nodiscard]] constexpr const eqx::lib::Polygon<float, 4>&
        get_player() const noexcept
    {
        return this->m_player;
    }

    [[nodiscard]] constexpr const eqx::lib::Polygon<float, 4>&
        get_panzer() const noexcept
    {
        return this->m_panzer;
    }

    [[nodiscard]] constexpr const std::optional<eqx::lib::Polygon<float, 4>>
        get_shell() const noexcept
    {
        return this->m_shell;
    }

    [[nodiscard]] constexpr bool panzer_alive() const noexcept
    {
        return this->m_alive;
    }

private:
    eqx::lib::Polygon<float, 4> m_player;
    eqx::lib::Polygon<float, 4> m_panzer;
    std::optional<eqx::lib::Polygon<float, 4>> m_shell;
    Turning_State m_turning_state;
    float m_angv;
    float m_mangv;
    bool m_alive;
    eqx::lib::Point<float> m_shellv;
};

class KGame
{
public:
    KGame(const KGame&) = default;
    KGame(KGame&&) = default;
    KGame& operator= (const KGame&) = default;
    KGame& operator= (KGame&&) = default;
    ~KGame() = default;

    explicit inline KGame() noexcept
        :
        m_sim(),
        m_window(1920, 1080, "eqx::OGL --- Test Kgame"sv),
        m_shader_program(eqx::ogl::Shader_Program::from_files(
            "./Resources/Shaders/KGame/Vertex.glsl"sv,
            "./Resources/Shaders/KGame/Fragment.glsl"sv)),
        m_vertex_array(
            std::array<float, 16>{},
            std::array<unsigned int, 2>{ 2u, 2u },
            std::array<unsigned int, 6>{ 0u, 1u, 2u, 1u, 3u, 2u }),
        m_turret_tex("./Resources/Textures/TankTurret.png"sv),
        m_panzer_tex("./Resources/Textures/Panzer.png"sv),
        m_panzer3_destroyed_tex("./Resources/Textures/Panzer3_Destroyed.png"sv),
        m_shell_tex("./Resources/Textures/TankShell.png"sv)
    {
        auto model = glm::mat4{ 1.0F };
        auto view = glm::mat4{ 1.0f };
        auto proj = glm::ortho(0.0f, 1920.0f, -1080.0f, 0.0f);
        this->m_shader_program.set_mat4("u_model"sv, model);
        this->m_shader_program.set_mat4("u_view"sv, view);
        this->m_shader_program.set_mat4("u_proj"sv, proj);

        this->m_shader_program.activate_texture("u_tex0"sv, 0);
    }

    inline void run() noexcept
    {
        auto loc = eqx::lib::Point<float>{};
        auto frame_timer = eqx::ogl::Frame_Timer{};
        auto sim_timer = eqx::lib::Timer{};
        auto seconds = 0.0F;
        constexpr auto tick = 1.0F / 60.0F;

        sim_timer.start();
        while (!this->m_window.should_close())
        {
            seconds += sim_timer.readf();
            sim_timer.start();

            this->m_window.clear();

            loc = this->m_window.get_cursor_location();
            this->m_window.set_name(std::format(
                "eqx::ogl --- "sv
                "Test KGame --- "sv
                "Location: ({}, {}) --- "sv
                "Frames: {} --- "sv
                "FPS: {}"sv,
                loc.get_x(), loc.get_y(), frame_timer.get_frames(),
                frame_timer.get_fps()));

            this->handle_input();

            while (seconds > tick)
            {
                this->m_sim.update(tick);
                seconds -= tick;
            }

            this->render();

            if (this->m_window.key_down(eqx::ogl::Window::Key::Escape))
            {
                this->m_window.close();
            }

            if constexpr (c_smoke == true)
            {
                this->m_window.close();
            }

            this->m_window.swap();
            this->m_window.poll();
            frame_timer.update();
        }
    }

private:
    inline void handle_input() noexcept
    {
        if (this->m_window.key_down(eqx::ogl::Window::Key::Left)
            && this->m_window.key_down(eqx::ogl::Window::Key::Right))
        {
            this->m_sim.set_turning_state(Sim::Turning_State::Stop);
        }
        else if (this->m_window.key_up(eqx::ogl::Window::Key::Left)
            && this->m_window.key_up(eqx::ogl::Window::Key::Right))
        {
            this->m_sim.set_turning_state(Sim::Turning_State::Stop);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::Left))
        {
            this->m_sim.set_turning_state(Sim::Turning_State::Left);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::Right))
        {
            this->m_sim.set_turning_state(Sim::Turning_State::Right);
        }

        if (this->m_window.key_down(eqx::ogl::Window::Key::Space))
        {
            this->m_sim.fire();
        }

        if (this->m_window.key_down(eqx::ogl::Window::Key::R))
        {
            this->m_sim.reset();
        }
    }

    inline void render() noexcept
    {
        if (this->m_sim.get_shell().has_value())
        {
            this->m_vertex_array.set_vertex_buffer(
                std::array<float, 16>{
                    this->m_sim.get_shell()->get_data()[0].get_x(), this->m_sim.get_shell()->get_data()[0].get_y(), 1.0F, 0.0F,
                    this->m_sim.get_shell()->get_data()[1].get_x(), this->m_sim.get_shell()->get_data()[1].get_y(), 0.0F, 0.0F,
                    this->m_sim.get_shell()->get_data()[3].get_x(), this->m_sim.get_shell()->get_data()[3].get_y(), 1.0F, 1.0F,
                    this->m_sim.get_shell()->get_data()[2].get_x(), this->m_sim.get_shell()->get_data()[2].get_y(), 0.0F, 1.0F },
                std::array<unsigned int, 2>{ 2U, 2U });

            this->m_shader_program.enable();
            this->m_vertex_array.enable();
            this->m_shell_tex.enable(0);
            eqx::ogl::draw(this->m_vertex_array.get_index_count());
        }

        this->m_vertex_array.set_vertex_buffer(
            std::array<float, 16>{
                this->m_sim.get_player().get_data()[0].get_x(), this->m_sim.get_player().get_data()[0].get_y(), 1.0F, 0.0F,
                this->m_sim.get_player().get_data()[1].get_x(), this->m_sim.get_player().get_data()[1].get_y(), 0.0F, 0.0F,
                this->m_sim.get_player().get_data()[3].get_x(), this->m_sim.get_player().get_data()[3].get_y(), 1.0F, 1.0F,
                this->m_sim.get_player().get_data()[2].get_x(), this->m_sim.get_player().get_data()[2].get_y(), 0.0F, 1.0F },
            std::array<unsigned int, 2>{ 2U, 2U });

        this->m_shader_program.enable();
        this->m_vertex_array.enable();
        this->m_turret_tex.enable(0);
        eqx::ogl::draw(this->m_vertex_array.get_index_count());

        this->m_vertex_array.set_vertex_buffer(
            std::array<float, 16>{
                this->m_sim.get_panzer().get_data()[0].get_x(), this->m_sim.get_panzer().get_data()[0].get_y(), 1.0F, 0.0F,
                this->m_sim.get_panzer().get_data()[1].get_x(), this->m_sim.get_panzer().get_data()[1].get_y(), 0.0F, 0.0F,
                this->m_sim.get_panzer().get_data()[3].get_x(), this->m_sim.get_panzer().get_data()[3].get_y(), 1.0F, 1.0F,
                this->m_sim.get_panzer().get_data()[2].get_x(), this->m_sim.get_panzer().get_data()[2].get_y(), 0.0F, 1.0F },
            std::array<unsigned int, 2>{ 2U, 2U });

        this->m_shader_program.enable();
        this->m_vertex_array.enable();
        if (this->m_sim.panzer_alive())
        {
            this->m_panzer_tex.enable(0);
        }
        else
        {
            this->m_panzer3_destroyed_tex.enable(0);
        }
        eqx::ogl::draw(this->m_vertex_array.get_index_count());
    }

    Sim m_sim;
    eqx::ogl::Window m_window;
    eqx::ogl::Shader_Program m_shader_program;
    eqx::ogl::Vertex_Array m_vertex_array;
    eqx::ogl::Texture m_turret_tex;
    eqx::ogl::Texture m_panzer_tex;
    eqx::ogl::Texture m_panzer3_destroyed_tex;
    eqx::ogl::Texture m_shell_tex;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    std::format_to(std::ostream_iterator<char>(std::cout), "Start\n\n"sv);

    eqx::ogl::init();

    {
        auto kgame = KGame{};
        kgame.run();
    }

    eqx::ogl::free();

    std::format_to(std::ostream_iterator<char>(std::cout), "\nEnd\n"sv);
    return EXIT_SUCCESS;
}
