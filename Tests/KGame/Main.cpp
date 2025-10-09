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

namespace Component
{
    class Angular
    {
    public:
        enum class State : int
        {
            Right = -1,
            Stop = 0,
            Left = 1
        };

        Angular(const Angular&) = default;
        Angular(Angular&&) = default;
        Angular& operator= (const Angular&) = default;
        Angular& operator= (Angular&&) = default;
        ~Angular() = default;

        explicit constexpr Angular(const float velocity,
            const float max_velocity, const float acceleration,
            const eqx::lib::Point<float>& pivot, const float friction) noexcept
            :
            m_state(State::Stop),
            m_velocity(velocity),
            m_max_velocity(max_velocity),
            m_acceleration(acceleration),
            m_pivot(pivot),
            m_friction(friction)
        {
        }

        constexpr std::pair<float, eqx::lib::Point<float>>
            update(eqx::lib::Polygon<float, 4>& geometry,
            const float dt) noexcept
        {
            this->m_velocity += std::ranges::clamp(
                this->m_acceleration * static_cast<int>(this->m_state) * dt,
                -this->m_max_velocity, this->m_max_velocity);

            auto fric = std::ranges::min(this->m_friction * dt,
                eqx::lib::Math::abs(this->m_velocity));
            fric = this->m_velocity >= 0.0F ? -fric : fric;
            this->m_velocity += fric;

            geometry.rotate(
                eqx::lib::Math::to_radians(this->m_velocity * dt),
                this->m_pivot);

            return std::make_pair(
                eqx::lib::Math::to_radians(this->m_velocity * dt),
                this->m_pivot);
        }

        constexpr void set_state(const State state) noexcept
        {
            this->m_state = state;
        }

        constexpr void set_pivot(const eqx::lib::Point<float>& pivot) noexcept
        {
            this->m_pivot = pivot;
        }

        [[nodiscard]] constexpr float get_velocity() const noexcept
        {
            return this->m_velocity;
        }

    private:
        State m_state;
        float m_velocity;
        float m_max_velocity;
        float m_acceleration;
        eqx::lib::Point<float> m_pivot;
        float m_friction;
    };

    class Linear
    {
    public:
        enum class State : int
        {
            Reverse = -1,
            Stop = 0,
            Forward = 1
        };

        Linear(const Linear&) = default;
        Linear(Linear&&) = default;
        Linear& operator= (const Linear&) = default;
        Linear& operator= (Linear&&) = default;
        ~Linear() = default;

        explicit constexpr Linear(const eqx::lib::Point<float>& velocity,
            const float max_speed,
            const eqx::lib::Point<float>& acceleration,
            const float friction) noexcept
            :
            m_state(State::Stop),
            m_velocity(velocity),
            m_max_speed(max_speed),
            m_acceleration(acceleration),
            m_friction(friction)
        {
        }

        constexpr eqx::lib::Point<float> update(
            eqx::lib::Polygon<float, 4>& geometry, const float dt) noexcept
        {
            this->m_velocity.translate(eqx::lib::Point<float>::scale(
                this->m_acceleration, static_cast<int>(this->m_state) * dt));

            this->m_velocity.projection(this->m_acceleration);

            if (this->m_velocity.magnitude() > this->m_max_speed)
            {
                this->m_velocity.scale(
                    1.0F / (this->m_velocity.magnitude() / this->m_max_speed));
            }

            if (this->m_velocity.magnitude2() > 0.0F)
            {
                auto friction = eqx::lib::Point<float>::scale(
                    eqx::lib::Point<float>::normalize(
                        eqx::lib::Point<float>::negate(this->m_velocity)),
                    this->m_friction * dt);

                this->m_velocity.translate(friction);
            }

            geometry.translate(eqx::lib::Point<float>::scale(
                this->m_velocity, dt));

            return eqx::lib::Point<float>::scale(this->m_velocity, dt);
        }

        constexpr void set_state(const State state) noexcept
        {
            this->m_state = state;
        }

        constexpr void set_acceleration(
            const eqx::lib::Point<float>& acceleration) noexcept
        {
            this->m_acceleration = acceleration;
        }

        [[nodiscard]] constexpr const eqx::lib::Point<float>&
            get_velocity() const noexcept
        {
            return this->m_velocity;
        }

    private:
        State m_state;
        eqx::lib::Point<float> m_velocity;
        float m_max_speed;
        eqx::lib::Point<float> m_acceleration;
        float m_friction;
    };
}

class Tank
{
public:
    Tank(const Tank&) = default;
    Tank(Tank&&) = default;
    Tank& operator= (const Tank&) = default;
    Tank& operator= (Tank&&) = default;
    ~Tank() = default;

    explicit constexpr Tank(const float hw, const float hh,
        const float tw, const float th,
        const eqx::lib::Point<float>& hl,
        const eqx::lib::Point<float>& tl, const float ang) noexcept
        :
        m_hull_geometry(),
        m_turret_geometry(),
        m_hull_angular(0.0F, 30.0F, 35.0F,
            eqx::lib::Point<float>{ 1730.0F, -540.0F }, 30.0F),
        m_turret_angular(0.0F, 50.0F, 55.0F,
            eqx::lib::Point<float>{ 1730.0F, -540.0F }, 50.0F),
        m_hull_linear(eqx::lib::Point<float>::origin(), 100.0F,
            eqx::lib::Point<float>::origin(), 40.0F)
    {
        this->m_hull_geometry = eqx::lib::Polygon<float, 4>{
            eqx::lib::Point<float>{ hw, hh },
            eqx::lib::Point<float>{ -hw, hh },
            eqx::lib::Point<float>{ -hw, -hh },
            eqx::lib::Point<float>{ hw, -hh } };
        //this->m_hull_geometry = eqx::lib::Polygon<float, 4>::make_rectangle(
            //115.0203F, 200.0F);
        this->m_hull_geometry.move(hl);
        this->m_hull_geometry.rotate(ang);

        this->m_turret_geometry = eqx::lib::Polygon<float, 4>{
            eqx::lib::Point<float>{ tw, th },
            eqx::lib::Point<float>{ -tw, th },
            eqx::lib::Point<float>{ -tw, -th },
            eqx::lib::Point<float>{ tw, -th } };
        this->m_turret_geometry.move(tl);
        this->m_turret_geometry.rotate(ang);
    }

    constexpr void update(const float dt) noexcept
    {
        const auto p1 = eqx::lib::Point<float>::normalize(
            eqx::lib::Point<float>::translate(
                this->m_hull_geometry.get_data()[0],
                eqx::lib::Point<float>::negate(
                    this->m_hull_geometry.get_data()[3])));

        this->m_hull_linear.set_acceleration(
            eqx::lib::Point<float>::scale(p1, 50.0F));

        const auto trans =
            this->m_hull_linear.update(this->m_hull_geometry, dt);
        this->m_turret_geometry.translate(trans);

        if (this->m_hull_angular.get_velocity() < 0.0F)
        {
            this->m_hull_angular.set_pivot(eqx::lib::Point<float>::midpoint(
                this->m_hull_geometry.get_data()[0],
                this->m_hull_geometry.get_data()[3]));
        }
        else
        {
            this->m_hull_angular.set_pivot(eqx::lib::Point<float>::midpoint(
                this->m_hull_geometry.get_data()[1],
                this->m_hull_geometry.get_data()[2]));
        }

        const auto [rad, pivot] =
            this->m_hull_angular.update(this->m_hull_geometry, dt);
        this->m_turret_geometry.rotate(rad, pivot);

        const auto front = eqx::lib::Point<float>::midpoint(
            this->m_hull_geometry.get_data()[0],
            this->m_hull_geometry.get_data()[1]);
        const auto back = eqx::lib::Point<float>::midpoint(
            this->m_hull_geometry.get_data()[2],
            this->m_hull_geometry.get_data()[3]);

        this->m_turret_angular.set_pivot(eqx::lib::Point<float>::lerp(
            front, back, 0.55F));
        this->m_turret_angular.update(this->m_turret_geometry, dt);
    }

    constexpr void set_hull_state(
        const Component::Angular::State state) noexcept
    {
        this->m_hull_angular.set_state(state);
    }

    constexpr void set_turret_state(
        const Component::Angular::State state) noexcept
    {
        this->m_turret_angular.set_state(state);
    }

    constexpr void set_hull_linear_state(
        const Component::Linear::State state) noexcept
    {
        this->m_hull_linear.set_state(state);
    }

    [[nodiscard]] constexpr const eqx::lib::Polygon<float, 4>&
        get_hull_geometry() const noexcept
    {
        return this->m_hull_geometry;
    }

    [[nodiscard]] constexpr const eqx::lib::Polygon<float, 4>&
        get_turret_geometry() const noexcept
    {
        return this->m_turret_geometry;
    }

private:
    eqx::lib::Polygon<float, 4> m_hull_geometry;
    eqx::lib::Polygon<float, 4> m_turret_geometry;
    Component::Angular m_hull_angular;
    Component::Angular m_turret_angular;
    Component::Linear m_hull_linear;
};

class Sim
{
public:
    Sim(const Sim&) = default;
    Sim(Sim&&) = default;
    Sim& operator= (const Sim&) = default;
    Sim& operator= (Sim&&) = default;
    ~Sim() = default;

    explicit constexpr Sim() noexcept
        :
        m_sherman(114.0F, 200.0F, 74.065F, 110.0F,
            eqx::lib::Point<float>{ 1700.0F, -540.0F },
            eqx::lib::Point<float>{ 1715.0F, -540.0F },
            std::numbers::pi_v<float> / 2.0F),
        m_panzer(200.0F * 0.57F, 200.0F, 120.0F * 0.5227F, 120.0F,
            eqx::lib::Point<float>{ 300.0F, -540.0F },
            eqx::lib::Point<float>{ 345.0F, -540.0F },
            -std::numbers::pi_v<float> / 2.0F),
        m_alive(true)
    {
    }

    constexpr void update(const float dt) noexcept
    {
        this->m_sherman.update(dt);
        this->m_panzer.update(dt);

        if (this->m_shell.has_value())
        {
            if (this->m_shell->aabb(this->m_panzer.get_hull_geometry()))
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

    constexpr void set_sherman_turret_state(
        const Component::Angular::State state) noexcept
    {
        this->m_sherman.set_turret_state(state);
    }

    constexpr void set_sherman_hull_state(
        const Component::Angular::State state) noexcept
    {
        this->m_sherman.set_hull_state(state);
    }

    constexpr void set_sherman_hull_linear_state(
        const Component::Linear::State state) noexcept
    {
        this->m_sherman.set_hull_linear_state(state);
    }

    constexpr void fire() noexcept
    {
        if (!this->m_shell.has_value())
        {
            const auto front = eqx::lib::Point<float>::midpoint(
                this->m_sherman.get_turret_geometry().get_data()[0],
                this->m_sherman.get_turret_geometry().get_data()[1]);
            const auto back = eqx::lib::Point<float>::midpoint(
                this->m_sherman.get_turret_geometry().get_data()[2],
                this->m_sherman.get_turret_geometry().get_data()[3]);
            const auto dir = eqx::lib::Point<float>::normalize(
                eqx::lib::Point<float>::translate(
                    front, eqx::lib::Point<float>::negate(back)));

            const auto dest = eqx::lib::Point<float>::translate(front,
                    eqx::lib::Point<float>::negate(
                        eqx::lib::Point<float>::scale(dir, 100.0F)));

            m_shell.emplace();
            this->m_shell.emplace(eqx::lib::Polygon<float, 4>{
                eqx::lib::Point<float>{ 6.6F, 20.0F },
                eqx::lib::Point<float>{ -6.6F, 20.0F },
                eqx::lib::Point<float>{ -6.6F, -20.0F },
                eqx::lib::Point<float>{ 6.6F, -20.0F } });
            this->m_shell->move(dest);
            this->m_shell->rotate(std::atan2f(dir.get_y(), dir.get_x())
                - (std::numbers::pi_v<float> / 2.0F));
            this->m_shellv = eqx::lib::Point<float>::scale(dir, 2'000.0F);
        }
    }

    constexpr void reset() noexcept
    {
        this->m_alive = true;
    }

    [[nodiscard]] constexpr const eqx::lib::Polygon<float, 4>&
        get_sherman_turret() const noexcept
    {
        return this->m_sherman.get_turret_geometry();
    }

    [[nodiscard]] constexpr const eqx::lib::Polygon<float, 4>&
        get_sherman_hull() const noexcept
    {
        return this->m_sherman.get_hull_geometry();
    }

    [[nodiscard]] constexpr const eqx::lib::Polygon<float, 4>&
        get_panzer_turret() const noexcept
    {
        return this->m_panzer.get_turret_geometry();
    }

    [[nodiscard]] constexpr const eqx::lib::Polygon<float, 4>&
        get_panzer_hull() const noexcept
    {
        return this->m_panzer.get_hull_geometry();
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
    Tank m_sherman;
    Tank m_panzer;
    std::optional<eqx::lib::Polygon<float, 4>> m_shell;
    bool m_alive;
    eqx::lib::Point<float> m_shellv;
};

class Renderer
{
public:
    Renderer(const Renderer&) = default;
    Renderer(Renderer&&) = default;
    Renderer& operator= (const Renderer&) = default;
    Renderer& operator= (Renderer&&) = default;
    ~Renderer() = default;

    explicit inline Renderer() noexcept
        :
        m_shader_program(eqx::ogl::Shader_Program::from_files(
            "./Resources/Shaders/KGame/Vertex.glsl"sv,
            "./Resources/Shaders/KGame/Fragment.glsl"sv)),
        m_vertex_array(
            std::array<float, 16>{},
            std::array<unsigned int, 2>{ 2u, 2u },
            std::array<unsigned int, 6>{ 0u, 1u, 2u, 1u, 3u, 2u }),
        m_sherman_turret_tex("./Resources/Textures/M4_Sherman_Turret.png"sv),
        m_sherman_hull_tex("./Resources/Textures/M4_Sherman_Hull.png"sv),
        m_panzer_turret_tex("./Resources/Textures/Panzer3_Turret.png"sv),
        m_panzer_hull_tex("./Resources/Textures/Panzer3_Hull.png"sv),
        m_panzer_destroyed_tex("./Resources/Textures/Panzer3_Destroyed.png"sv),
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

    inline void render(const Sim& sim) noexcept
    {
        this->set_vertex_array(sim.get_sherman_hull());
        this->draw(this->m_sherman_hull_tex);

        if (sim.get_shell().has_value())
        {
            this->set_vertex_array(sim.get_shell().value());
            this->draw(this->m_shell_tex);
        }

        this->set_vertex_array(sim.get_sherman_turret());
        this->draw(this->m_sherman_turret_tex);

        if (sim.panzer_alive())
        {
            this->set_vertex_array(sim.get_panzer_hull());
            this->draw(this->m_panzer_hull_tex);

            this->set_vertex_array(sim.get_panzer_turret());
            this->draw(this->m_panzer_turret_tex);
        }
        else
        {
            this->set_vertex_array(sim.get_panzer_hull());
            this->draw(this->m_panzer_destroyed_tex);
        }
    }

private:
    inline void draw(const eqx::ogl::Texture& tex) const noexcept
    {
        this->m_shader_program.enable();
        this->m_vertex_array.enable();
        tex.enable(0);
        eqx::ogl::draw(this->m_vertex_array.get_index_count());
    }

    inline void set_vertex_array(
        const eqx::lib::Polygon<float, 4>& geometry) noexcept
    {
        this->m_vertex_array.set_vertex_buffer(
            std::array<float, 16>{
                geometry.get_data()[0].get_x(), geometry.get_data()[0].get_y(), 1.0F, 0.0F,
                geometry.get_data()[1].get_x(), geometry.get_data()[1].get_y(), 0.0F, 0.0F,
                geometry.get_data()[3].get_x(), geometry.get_data()[3].get_y(), 1.0F, 1.0F,
                geometry.get_data()[2].get_x(), geometry.get_data()[2].get_y(), 0.0F, 1.0F },
            std::array<unsigned int, 2>{ 2U, 2U });
    }

    eqx::ogl::Shader_Program m_shader_program;
    eqx::ogl::Vertex_Array m_vertex_array;
    eqx::ogl::Texture m_sherman_turret_tex;
    eqx::ogl::Texture m_sherman_hull_tex;
    eqx::ogl::Texture m_panzer_turret_tex;
    eqx::ogl::Texture m_panzer_hull_tex;
    eqx::ogl::Texture m_panzer_destroyed_tex;
    eqx::ogl::Texture m_shell_tex;
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
        m_window(1920, 1080, "eqx::OGL --- Test Kgame"sv),
        m_sim(),
        m_renderer()
    {
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

            this->m_renderer.render(this->m_sim);

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
            this->m_sim.set_sherman_turret_state(Component::Angular::State::Stop);
        }
        else if (this->m_window.key_up(eqx::ogl::Window::Key::Left)
            && this->m_window.key_up(eqx::ogl::Window::Key::Right))
        {
            this->m_sim.set_sherman_turret_state(Component::Angular::State::Stop);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::Left))
        {
            this->m_sim.set_sherman_turret_state(Component::Angular::State::Left);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::Right))
        {
            this->m_sim.set_sherman_turret_state(Component::Angular::State::Right);
        }

        if (this->m_window.key_down(eqx::ogl::Window::Key::A)
            && this->m_window.key_down(eqx::ogl::Window::Key::D))
        {
            this->m_sim.set_sherman_hull_state(Component::Angular::State::Stop);
        }
        else if (this->m_window.key_up(eqx::ogl::Window::Key::A)
            && this->m_window.key_up(eqx::ogl::Window::Key::D))
        {
            this->m_sim.set_sherman_hull_state(Component::Angular::State::Stop);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::A))
        {
            this->m_sim.set_sherman_hull_state(Component::Angular::State::Left);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::D))
        {
            this->m_sim.set_sherman_hull_state(Component::Angular::State::Right);
        }

        if (this->m_window.key_down(eqx::ogl::Window::Key::W)
            && this->m_window.key_down(eqx::ogl::Window::Key::S))
        {
            this->m_sim.set_sherman_hull_linear_state(Component::Linear::State::Stop);
        }
        else if (this->m_window.key_up(eqx::ogl::Window::Key::W)
            && this->m_window.key_up(eqx::ogl::Window::Key::S))
        {
            this->m_sim.set_sherman_hull_linear_state(Component::Linear::State::Stop);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::W))
        {
            this->m_sim.set_sherman_hull_linear_state(Component::Linear::State::Forward);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::S))
        {
            this->m_sim.set_sherman_hull_linear_state(Component::Linear::State::Reverse);
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

    eqx::ogl::Window m_window;
    Sim m_sim;
    Renderer m_renderer;
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
