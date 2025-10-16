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

[[nodiscard]] constexpr bool near(const eqx::lib::Point<float>& p1,
    const eqx::lib::Point<float>& p2) noexcept
{
    return eqx::lib::Math::near(p1.get_x(), p2.get_x(), 1.0F)
        && eqx::lib::Math::near(p1.get_y(), p2.get_y(), 1.0F);
}

[[nodiscard]] constexpr std::pair<float, float> get_wh(
    const eqx::lib::Polygon<float, 4>& p) noexcept
{
    const auto width = eqx::lib::Math::abs(
        p.get_data()[0].get_x() - p.get_data()[1].get_x());
    const auto height = eqx::lib::Math::abs(
        p.get_data()[0].get_y() - p.get_data()[3].get_y());

    return std::make_pair(width, height);
}

class Shell
{
public:
    Shell(const Shell&) = default;
    Shell(Shell&&) = default;
    Shell& operator= (const Shell&) = default;
    Shell& operator= (Shell&&) = default;
    ~Shell() = default;

    explicit constexpr Shell(const eqx::lib::Point<float>& location,
        const float rotation) noexcept
        :
        m_geometry(eqx::lib::Polygon<float, 4>::rectangle(292.0F, 879.0F))
    {
        this->m_geometry.scale(0.01F);
        //this->m_geometry.rotate(eqx::lib::Math::to_radians(rotation));
        this->m_geometry.rotate(rotation);
        this->m_geometry.translate(location);
    }

    constexpr void update(const float dt) noexcept
    {
        constexpr auto speed = 250.0F;

        const auto normal = eqx::lib::Point<float>::translate(
            eqx::lib::Point<float>::midpoint(
                this->m_geometry.get_data()[0],
                this->m_geometry.get_data()[1]),
            eqx::lib::Point<float>::negate(this->m_geometry.center()));

        this->m_geometry.translate(
            eqx::lib::Point<float>::scale(
                eqx::lib::Point<float>::normalize(normal), speed * dt));
    }

    [[nodiscard]] constexpr const eqx::lib::Polygon<float, 4>&
        get_geometry() const noexcept
    {
        return this->m_geometry;
    }

private:
    eqx::lib::Polygon<float, 4> m_geometry;
};

class Tank
{
public:
    Tank(const Tank&) = default;
    Tank(Tank&&) = default;
    Tank& operator= (const Tank&) = default;
    Tank& operator= (Tank&&) = default;
    ~Tank() = default;

    explicit constexpr Tank([[maybe_unused]] const eqx::lib::Point<float>& location,
        [[maybe_unused]] const float rotation) noexcept
        :
        m_hull_geometry(
            eqx::lib::Polygon<float, 4>::rectangle(625.0F, 1099.0F)),
        m_turret_geometry(
            eqx::lib::Polygon<float, 4>::rectangle(347.0F, 761.0F)),
        m_speed(25.0F),
        m_hull_angular(15.0F),
        m_turret_angular(15.0F),
        m_move_target(location),
        m_look_target(eqx::lib::Point<float>::origin())
    {
        this->m_hull_geometry.scale(0.1F);
        this->m_turret_geometry.scale(0.1F);

        const auto [width, height] = get_wh(this->m_hull_geometry);
        const auto turret_offset = eqx::lib::Point<float>{ 0.5F * width,
            -0.4F * height };
        const auto turret_loc = eqx::lib::Point<float>::translate(
            this->m_hull_geometry.get_data()[1], turret_offset);
        this->m_turret_geometry.move(turret_loc);

        this->m_hull_geometry.rotate(eqx::lib::Math::to_radians(rotation));
        this->m_turret_geometry.rotate(eqx::lib::Math::to_radians(rotation),
            this->m_hull_geometry.center());

        this->m_hull_geometry.translate(location);
        this->m_turret_geometry.translate(location);
    }

    [[nodiscard]] constexpr bool update(const float dt) noexcept
    {
        if (near(this->m_move_target, this->m_hull_geometry.center()) == false)
        {
            const auto normal = eqx::lib::Point<float>::translate(
                eqx::lib::Point<float>::midpoint(
                    this->m_hull_geometry.get_data()[0],
                    this->m_hull_geometry.get_data()[1]),
                eqx::lib::Point<float>::negate(this->m_hull_geometry.center()));
            const auto target = eqx::lib::Point<float>::translate(
                this->m_move_target,
                eqx::lib::Point<float>::negate(this->m_hull_geometry.center()));
            const auto cross = eqx::lib::Point<float>::cross(
                eqx::lib::Point<float>::normalize(normal),
                eqx::lib::Point<float>::normalize(target));
            const auto angle = eqx::lib::Math::to_radians(
                this->m_hull_angular * dt);

            if (eqx::lib::Math::near(cross, 0.0F, angle * 1.1F))
            {
                this->m_hull_geometry.translate(
                    eqx::lib::Point<float>::scale(
                        eqx::lib::Point<float>::normalize(normal),
                        this->m_speed * dt));
                this->m_turret_geometry.translate(
                    eqx::lib::Point<float>::scale(
                        eqx::lib::Point<float>::normalize(normal),
                        this->m_speed * dt));
            }
            else if (cross > 0.0F)
            {
                this->m_hull_geometry.rotate(angle);
                this->m_turret_geometry.rotate(angle,
                    this->m_hull_geometry.center());
            }
            else
            {
                this->m_hull_geometry.rotate(-angle);
                this->m_turret_geometry.rotate(-angle,
                    this->m_hull_geometry.center());
            }
        }

        return this->update_turret(dt);
    }

    constexpr bool update_turret(const float dt) noexcept
    {
        const auto normal = eqx::lib::Point<float>::translate(
            eqx::lib::Point<float>::midpoint(
                this->m_turret_geometry.get_data()[0],
                this->m_turret_geometry.get_data()[1]),
            eqx::lib::Point<float>::negate(this->m_turret_geometry.center()));
        const auto target = eqx::lib::Point<float>::translate(
            this->m_look_target,
            eqx::lib::Point<float>::negate(this->m_turret_geometry.center()));
        const auto cross = eqx::lib::Point<float>::cross(
            eqx::lib::Point<float>::normalize(normal),
            eqx::lib::Point<float>::normalize(target));
        const auto angle = eqx::lib::Math::to_radians(
            this->m_turret_angular * dt);

        if (eqx::lib::Math::near(cross, 0.0F, angle * 1.1F))
        {
            return true;
        }
        else if (cross > 0.0F)
        {
            this->m_turret_geometry.rotate(angle,
                this->m_hull_geometry.center());
        }
        else
        {
            this->m_turret_geometry.rotate(-angle,
                this->m_hull_geometry.center());
        }

        return false;
    }

    constexpr void set_move_target(const eqx::lib::Point<float>& p) noexcept
    {
        this->m_move_target = p;
    }

    constexpr void set_look_target(const eqx::lib::Point<float>& p) noexcept
    {
        this->m_look_target = p;
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
    float m_speed;
    float m_hull_angular;
    float m_turret_angular;
    eqx::lib::Point<float> m_move_target;
    eqx::lib::Point<float> m_look_target;
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
        m_map(eqx::lib::Polygon<float, 4>::rectangle(960.0F * 2.0F, 704.0F * 2.0F)),
        m_view(eqx::lib::Polygon<float, 4>::rectangle(1'600.0F, 900.0F)),
        m_view_offset(eqx::lib::Point<float>::origin()),
        m_view_state(eqx::lib::Point<float>::origin()),
        m_view_zoom(1.0F),
        m_view_zoom_state(0.0F),
        m_sherman(eqx::lib::Point<float>{ 715.0F, 270.0F }, 90.0F),
        m_sherman_shell(std::nullopt),
        m_panzer(eqx::lib::Point<float>{ -600.0F, -200.0F }, -90.0F),
        m_panzer_shell(std::nullopt),
        m_selected_tank(std::nullopt)
    {
    }

    constexpr void update(const float dt) noexcept
    {
        bool in_range = false;

        if (eqx::lib::Point<float>::distance(
            this->m_sherman.get_hull_geometry().center(),
            this->m_panzer.get_hull_geometry().center()) < 800.0F)
        {
            this->m_sherman.set_look_target(
                this->m_panzer.get_hull_geometry().center());
            this->m_panzer.set_look_target(
                this->m_sherman.get_hull_geometry().center());
            in_range = true;
        }
        else
        {
            const auto sherman_normal = eqx::lib::Point<float>::midpoint(
                this->m_sherman.get_hull_geometry().get_data()[0],
                this->m_sherman.get_hull_geometry().get_data()[1]);
            const auto panzer_normal = eqx::lib::Point<float>::midpoint(
                this->m_panzer.get_hull_geometry().get_data()[0],
                this->m_panzer.get_hull_geometry().get_data()[1]);
            this->m_sherman.set_look_target(sherman_normal);
            this->m_panzer.set_look_target(panzer_normal);
        }

        if (this->m_sherman_shell.has_value())
        {
            if (this->m_sherman_shell->get_geometry().aabb(this->get_map()))
            {
                this->m_sherman_shell->update(dt);
            }
            else
            {
                this->m_sherman_shell.reset();
            }
        }

        if (this->m_panzer_shell.has_value())
        {
            if (this->m_panzer_shell->get_geometry().aabb(this->get_map()))
            {
                this->m_panzer_shell->update(dt);
            }
            else
            {
                this->m_panzer_shell.reset();
            }
        }

        const auto sherman_fire = this->m_sherman.update(dt);
        const auto panzer_fire = this->m_panzer.update(dt);

        if (sherman_fire && !this->m_sherman_shell.has_value() && in_range)
        {
            const auto turret_normal = eqx::lib::Point<float>::midpoint(
                this->m_sherman.get_turret_geometry().get_data()[0],
                this->m_sherman.get_turret_geometry().get_data()[1]);

            const auto temp = eqx::lib::Point<float>::translate(turret_normal,
                eqx::lib::Point<float>::negate(
                    this->m_sherman.get_turret_geometry().center()));

            const auto angle = std::atan2(temp.get_y(), temp.get_x());

            this->m_sherman_shell.emplace(turret_normal,
                (angle + std::numbers::pi_v<float> + (std::numbers::pi_v<float> / 2.0F)));
        }

        if (panzer_fire && !this->m_panzer_shell.has_value() && in_range)
        {
            const auto turret_normal = eqx::lib::Point<float>::midpoint(
                this->m_panzer.get_turret_geometry().get_data()[0],
                this->m_panzer.get_turret_geometry().get_data()[1]);

            const auto temp = eqx::lib::Point<float>::translate(turret_normal,
                eqx::lib::Point<float>::negate(
                    this->m_panzer.get_turret_geometry().center()));

            const auto angle = std::atan2(temp.get_y(), temp.get_x());

            this->m_panzer_shell.emplace(turret_normal,
                (angle + std::numbers::pi_v<float> + (std::numbers::pi_v<float> / 2.0F)));
        }

        this->update_view(dt);
    }

    constexpr void update_view(const float dt) noexcept
    {
        constexpr auto speed = 200.0F;
        const auto zoom_speed = this->m_view_zoom > 1.0F ? 2.0F : 1.0F;

        const auto velocity = eqx::lib::Math::near(
            eqx::lib::Point<float>::magnitude2(this->m_view_state), 0.0F)
        ? eqx::lib::Point<float>::origin()
        : eqx::lib::Point<float>::scale(
            eqx::lib::Point<float>::normalize(this->m_view_state), speed * dt);

        const auto zoom_velocity = this->m_view_zoom_state * zoom_speed * dt;

        this->m_view_offset.translate(velocity);
        this->m_view_zoom = std::ranges::clamp(
            this->m_view_zoom + zoom_velocity, 0.1F, 2.0F);
    }

    constexpr void select_unit(const eqx::lib::Point<float>& p) noexcept
    {
        auto box = eqx::lib::Polygon<float, 4>::rectangle(1.0F, 1.0F);
        box.translate(p);
        if (this->m_sherman.get_hull_geometry().aabb(box))
        {
            this->m_selected_tank.emplace(&this->m_sherman);
        }
        else if (this->m_panzer.get_hull_geometry().aabb(box))
        {
            this->m_selected_tank.emplace(&this->m_panzer);
        }
        else
        {
            this->m_selected_tank.reset();
        }
    }

    constexpr void move_unit(const eqx::lib::Point<float>& p) noexcept
    {
        if (this->m_selected_tank.has_value())
        {
            this->m_selected_tank.value()->set_move_target(p);
        }
    }

    constexpr void stop_unit() noexcept
    {
        if (this->m_selected_tank.has_value())
        {
            this->m_selected_tank.value()->set_move_target(
                this->m_selected_tank.value()->get_hull_geometry().center());
        }
    }

    [[nodiscard]] constexpr eqx::lib::Polygon<float, 4> view() const noexcept
    {
        auto view = this->get_view();
        view.scale(this->get_view_zoom());
        view.translate(this->get_view_offset());
        return view;
    }

    constexpr void set_view_state(float x, float y, float z) noexcept
    {
        this->m_view_state = eqx::lib::Point<float>{ x, y };
        this->m_view_zoom_state = z;
    }

    [[nodiscard]] constexpr const eqx::lib::Polygon<float, 4>&
        get_map() const noexcept
    {
        return this->m_map;
    }

    [[nodiscard]] constexpr const eqx::lib::Polygon<float, 4>&
        get_view() const noexcept
    {
        return this->m_view;
    }

    [[nodiscard]] constexpr const eqx::lib::Point<float>&
        get_view_offset() const noexcept
    {
        return this->m_view_offset;
    }

    [[nodiscard]] constexpr float get_view_zoom() const noexcept
    {
        return this->m_view_zoom;
    }

    [[nodiscard]] constexpr const Tank& get_sherman() const noexcept
    {
        return this->m_sherman;
    }

    [[nodiscard]] constexpr const std::optional<Shell>&
        get_sherman_shell() const noexcept
    {
        return this->m_sherman_shell;
    }

    [[nodiscard]] constexpr const Tank& get_panzer() const noexcept
    {
        return this->m_panzer;
    }

    [[nodiscard]] constexpr const std::optional<Shell>&
        get_panzer_shell() const noexcept
    {
        return this->m_panzer_shell;
    }

private:
    eqx::lib::Polygon<float, 4> m_map;
    eqx::lib::Polygon<float, 4> m_view;
    eqx::lib::Point<float> m_view_offset;
    eqx::lib::Point<float> m_view_state;
    float m_view_zoom;
    float m_view_zoom_state;
    Tank m_sherman;
    std::optional<Shell> m_sherman_shell;
    Tank m_panzer;
    std::optional<Shell> m_panzer_shell;
    std::optional<Tank*> m_selected_tank;
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
            "./Resources/Shaders/MGame/Vertex.glsl"sv,
            "./Resources/Shaders/MGame/Fragment.glsl"sv)),
        m_vertex_array(
            std::array<float, 16>{},
            std::array<unsigned int, 2>{ 2u, 2u },
            std::array<unsigned int, 6>{ 0u, 1u, 2u, 1u, 3u, 2u }),
        m_map_tex("./Resources/Maps/MGame.png"sv),
        m_panzer3_hull_tex("./Resources/Textures/Panzer3_Hull.png"sv),
        m_panzer3_turret_tex("./Resources/Textures/Panzer3_Turret.png"sv),
        m_m4_sherman_hull_tex("./Resources/Textures/M4_Sherman_Hull.png"sv),
        m_m4_sherman_turret_tex("./Resources/Textures/M4_Sherman_Turret.png"sv),
        m_tank_shell_tex("./Resources/Textures/TankShell.png"sv)
    {
        auto view = glm::scale(glm::mat4{ 1.0F }, glm::vec3(
            1.0F, 1.0F, 1.0F));
        view = glm::translate(view, glm::vec3(0.0F, 0.0F, 0.0F));
        auto proj = glm::ortho(0.0f, 1920.0f, -1080.0f, 0.0f);
        this->m_shader_program.set_mat4("u_view"sv, view);
        this->m_shader_program.set_mat4("u_proj"sv, proj);

        this->m_shader_program.activate_texture("u_tex0"sv, 0);
    }

    inline void render(const Sim& sim) noexcept
    {
        this->set_view(sim.view());

        this->set_vertex_array(sim.get_map());
        this->draw(this->m_map_tex);

        if (sim.get_panzer_shell().has_value())
        {
            this->set_vertex_array(sim.get_panzer_shell()->get_geometry());
            this->draw(this->m_tank_shell_tex);
        }

        if (sim.get_sherman_shell().has_value())
        {
            this->set_vertex_array(sim.get_sherman_shell()->get_geometry());
            this->draw(this->m_tank_shell_tex);
        }

        this->set_vertex_array(sim.get_panzer().get_hull_geometry());
        this->draw(this->m_panzer3_hull_tex);
        this->set_vertex_array(sim.get_panzer().get_turret_geometry());
        this->draw(this->m_panzer3_turret_tex);

        this->set_vertex_array(sim.get_sherman().get_hull_geometry());
        this->draw(this->m_m4_sherman_hull_tex);
        this->set_vertex_array(sim.get_sherman().get_turret_geometry());
        this->draw(this->m_m4_sherman_turret_tex);
    }

    inline void set_view(const eqx::lib::Polygon<float, 4>& view) noexcept
    {
        const auto width = std::abs(
            view.get_data()[0].get_x() - view.get_data()[1].get_x());
        const auto height = std::abs(
            view.get_data()[0].get_y() - view.get_data()[2].get_y());
        const auto xscale = 1920.0F / width;
        const auto yscale = 1080.0F / height;
        auto uview = glm::mat4{ 1.0F };
        uview = glm::scale(uview, glm::vec3(xscale, yscale, 1.0F));
        uview = glm::translate(uview, glm::vec3(
            -view.get_data()[1].get_x(), -view.get_data()[1].get_y(), 0.0F));
        this->m_shader_program.set_mat4("u_view"sv, uview);
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
    eqx::ogl::Texture m_map_tex;
    eqx::ogl::Texture m_panzer3_hull_tex;
    eqx::ogl::Texture m_panzer3_turret_tex;
    eqx::ogl::Texture m_m4_sherman_hull_tex;
    eqx::ogl::Texture m_m4_sherman_turret_tex;
    eqx::ogl::Texture m_tank_shell_tex;
};

class MGame
{
public:
    MGame(const MGame&) = default;
    MGame(MGame&&) = default;
    MGame& operator= (const MGame&) = default;
    MGame& operator= (MGame&&) = default;
    ~MGame() = default;

    explicit inline MGame() noexcept
        :
        m_window(1920, 1080, "eqx::ogl --- Test MGame"sv),
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
                "Test MGame --- "sv
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
        const auto [width, height] = get_wh(this->m_sim.view());
        const auto dimensions = this->m_window.get_dimensions();
        const auto click = this->m_window.get_cursor_location();
        const auto scale = eqx::lib::Point<float>{
            click.get_x() / dimensions.get_x(),
            click.get_y() / dimensions.get_y() };
        const auto offset = eqx::lib::Point<float>{
            width * scale.get_x(), height * scale.get_y() };
        const auto world_coordinate = eqx::lib::Point<float>::translate(
            this->m_sim.view().get_data()[1], offset);

        if (this->m_window.mouse_button_down(
            eqx::ogl::Window::Mouse_Button::Left))
        {
            this->m_sim.select_unit(world_coordinate);
        }

        if (this->m_window.mouse_button_down(
            eqx::ogl::Window::Mouse_Button::Right))
        {
            this->m_sim.move_unit(world_coordinate);
        }

        if (this->m_window.key_down(eqx::ogl::Window::Key::B))
        {
            this->m_sim.stop_unit();
        }

        auto view = eqx::lib::Point<float>{};

        if (this->m_window.key_down(eqx::ogl::Window::Key::A)
            && this->m_window.key_down(eqx::ogl::Window::Key::D))
        {
            view.set_x(0.0F);
        }
        else if (this->m_window.key_up(eqx::ogl::Window::Key::A)
            && this->m_window.key_up(eqx::ogl::Window::Key::D))
        {
            view.set_x(0.0F);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::A))
        {
            view.set_x(-1.0F);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::D))
        {
            view.set_x(1.0F);
        }

        if (this->m_window.key_down(eqx::ogl::Window::Key::W)
            && this->m_window.key_down(eqx::ogl::Window::Key::S))
        {
            view.set_y(0.0F);
        }
        else if (this->m_window.key_up(eqx::ogl::Window::Key::W)
            && this->m_window.key_up(eqx::ogl::Window::Key::S))
        {
            view.set_y(0.0F);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::W))
        {
            view.set_y(1.0F);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::S))
        {
            view.set_y(-1.0F);
        }

        const auto sy = eqx::ogl::Window::get_scroll_offset().get_y();
        eqx::ogl::Window::reset_scroll_offset();
        const auto zoom = eqx::lib::Math::near(sy, 0.0F)
            ? 0.0F : std::copysign(1.0F, -sy);

        this->m_sim.set_view_state(view.get_x(), view.get_y(), zoom);
    }

    eqx::ogl::Window m_window;
    Sim m_sim;
    Renderer m_renderer;
};

/*
    inline void handle_input(const float dt) noexcept
    {
        const auto sy = eqx::ogl::Window::get_scroll_offset().get_y();
        const auto zoom =
            this->m_window.key_up(eqx::ogl::Window::Key::Shift)
            ? -1.0F : -5.0F;

        this->m_sim.tick_view_zoom(sy * zoom, dt);
        eqx::ogl::Window::reset_scroll_offset();

        if (this->m_window.mouse_button_down(
            eqx::ogl::Window::Mouse_Button::Left))
        {
            const auto width = std::abs(this->m_sim.get_view().get_data()[0].get_x()
                - this->m_sim.get_view().get_data()[1].get_x());
            const auto height = std::abs(this->m_sim.get_view().get_data()[0].get_y()
                - this->m_sim.get_view().get_data()[3].get_y());
            const auto click = this->m_window.get_cursor_location();
            const auto scale = eqx::lib::Point<float>{
                click.get_x() / 1920.0F, click.get_y() / 1080.0F };
            const auto offset = eqx::lib::Point<float>{
                width * scale.get_x(), height * scale.get_y() };
            std::cout << offset.to_string() << '\n';
            auto box = eqx::lib::Polygon<float, 4>::rectangle(1.0F, 1.0F);
            box.move(eqx::lib::Point<float>{ this->m_sim.get_view().get_data()[1].get_x() + offset.get_x(),
                this->m_sim.get_view().get_data()[1].get_y() + offset.get_y() });

            std::cout << box.center().to_string() << '\n';
            std::cout << this->m_sim.get_panzer3().get_hull_geometry().center().to_string() << "*\n\n"sv;

            if (box.aabb(this->m_sim.get_panzer3().get_hull_geometry()))
            {
                this->m_sim.edit_panzer3().select();
            }
            else
            {
                this->m_sim.edit_panzer3().deselect();
            }
        }

        if (this->m_window.mouse_button_down(
            eqx::ogl::Window::Mouse_Button::Right))
        {
            if (this->m_sim.get_panzer3().get_selected())
            {
                this->m_sim.edit_panzer3().set_velocity(eqx::lib::Point<float>{
                    -20.0F, 0.0F });
            }
        }

        if (this->m_window.key_down(eqx::ogl::Window::Key::B))
        {
            this->m_sim.edit_panzer3().set_velocity(eqx::lib::Point<float>{
                0.0F, 0.0F });
        }

        /*
        this->m_sim.set_view_zoom(sy > 0.0F
            ? (sy * 0.04F) + 1.0F
            : ((sy + 100.0F) * 0.008F) + 0.2F);


        if (this->m_window.key_down(eqx::ogl::Window::Key::Up)
            && this->m_window.key_down(eqx::ogl::Window::Key::Down))
        {
            this->m_sim.tick_view_zoom(0.0F, dt);
        }
        else if (this->m_window.key_up(eqx::ogl::Window::Key::Up)
            && this->m_window.key_up(eqx::ogl::Window::Key::Down))
        {
            this->m_sim.tick_view_zoom(0.0F, dt);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::Up))
        {
            this->m_sim.tick_view_zoom(-zoom, dt);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::Down))
        {
            this->m_sim.tick_view_zoom(zoom, dt);
        }
        *//*

        auto view = eqx::lib::Point<float>::origin();
        const auto view_speed =
            this->m_window.key_up(eqx::ogl::Window::Key::Shift)
            ? 500.0F : 1'000.0F;

        if (this->m_window.key_down(eqx::ogl::Window::Key::A)
            && this->m_window.key_down(eqx::ogl::Window::Key::D))
        {
            view.set_x(0.0F);
        }
        else if (this->m_window.key_up(eqx::ogl::Window::Key::A)
            && this->m_window.key_up(eqx::ogl::Window::Key::D))
        {
            view.set_x(0.0F);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::A))
        {
            view.set_x(-view_speed);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::D))
        {
            view.set_x(view_speed);
        }

        if (this->m_window.key_down(eqx::ogl::Window::Key::W)
            && this->m_window.key_down(eqx::ogl::Window::Key::S))
        {
            view.set_y(0.0F);
        }
        else if (this->m_window.key_up(eqx::ogl::Window::Key::W)
            && this->m_window.key_up(eqx::ogl::Window::Key::S))
        {
            view.set_y(0.0F);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::W))
        {
            view.set_y(view_speed);
        }
        else if (this->m_window.key_down(eqx::ogl::Window::Key::S))
        {
            view.set_y(-view_speed);
        }

        this->m_sim.set_view_offset(view, dt);
    }

private:
    eqx::ogl::Window m_window;
    Sim m_sim;
    Renderer m_renderer;
};
*/

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    std::format_to(std::ostream_iterator<char>(std::cout), "Start\n\n"sv);

    eqx::ogl::init();

    {
        auto mgame = MGame{};
        mgame.run();
    }

    eqx::ogl::free();

    std::format_to(std::ostream_iterator<char>(std::cout), "\nEnd\n"sv);
    return EXIT_SUCCESS;
}
