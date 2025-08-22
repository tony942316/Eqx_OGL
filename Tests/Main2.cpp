import <Eqx/std.hpp>;

import <Eqx/TPL/glfw/glfw.hpp>;
import <Eqx/TPL/glad/glad.hpp>;
import <Eqx/TPL/glm/glm.hpp>;

import <Eqx/Lib/Macros.hpp>;
import Eqx.Lib;

import Eqx.OGL;

inline constexpr auto c_width = 1920;
inline constexpr auto c_height = 1080;

namespace eqx::exp
{
    constexpr float dot(const eqx::Point<float>& v,
        const eqx::Point<float>& u) noexcept
    {
        return v.x * u.x + v.y * u.y;
    }

    class Polygon
    {
    public:
        Polygon() = default;
        Polygon(const Polygon&) = default;
        Polygon(Polygon&&) = default;
        Polygon& operator= (const Polygon&) = default;
        Polygon& operator= (Polygon&&) = default;
        ~Polygon() = default;

        constexpr void translate(const float x, const float y) noexcept
        {
            std::ranges::transform(m_vertices, std::ranges::begin(m_vertices),
                [x, y](const eqx::Point<float>& point)
                {
                    return point + eqx::Point<float>{x, y};
                });
        }

        constexpr void translate(const eqx::Point<float>& point) noexcept
        {
            translate(point.x, point.y);
        }

        inline void rotate(const float angle) noexcept
        {
            auto center = get_center();
            set_center(eqx::Point<float>{0.0f, 0.0f});
            auto rad = angle * (std::numbers::pi_v<float> / 180.0f);
            std::ranges::transform(m_vertices, std::ranges::begin(m_vertices),
                [rad](const eqx::Point<float>& point)
                {
                    return eqx::Point<float>{
                        (point.x * std::cos(rad)) - (point.y * std::sin(rad)),
                        (point.x * std::sin(rad)) + (point.y * std::cos(rad))};
                });
            set_center(center);
        }

        constexpr const eqx::Point<float>& support(
            const eqx::Point<float>& direction) const noexcept
        {
            eqx::ENSURE_HARD(!std::ranges::empty(m_vertices),
                "No Support For Empty Polygon!"sv);
            return std::ranges::max(m_vertices, {},
                [&direction](const eqx::Point<float>& point)
                {
                    return dot(point, direction);
                });
        }

        constexpr void add_vertex(const eqx::Point<float>& point) noexcept
        {
            m_vertices.emplace_back(point);
        }

        constexpr void set_center(const eqx::Point<float>& point) noexcept
        {
            translate(point - get_center());
        }

        constexpr eqx::Point<float> get_center() const noexcept
        {
            return std::reduce(std::ranges::cbegin(m_vertices),
                std::ranges::cend(m_vertices)) / std::ranges::size(m_vertices);
        }

        constexpr std::string to_string() const noexcept
        {
            return eqx::toString(m_vertices);
        }

        constexpr std::vector<unsigned int> get_fan() const noexcept
        {
            return fan(std::ranges::size(m_vertices));
        }

        constexpr std::span<const eqx::Point<float>> get_vertices() const noexcept
        {
            return m_vertices;
        }

        static constexpr std::vector<unsigned int> fan(
            const std::size_t vertex_count) noexcept
        {
            auto result = std::vector<unsigned int>{};
            result.resize((vertex_count - 2_uz) * 3_uz);
            for (auto i = 2_uz; i < std::ranges::size(result); i += 3_uz)
            {
                result.at(i - 2_uz) = 0u;
                result.at(i - 1_uz) = 1u + (static_cast<unsigned int>(i) / 3u);
                result.at(i) = 2u + (static_cast<unsigned int>(i) / 3u);
            }
            return result;
        }

    private:
        std::vector<eqx::Point<float>> m_vertices;
    };
}

namespace eqx::pul
{
    class Color
    {
    public:
        Color() = default;
        Color(const Color&) = default;
        Color(Color&&) = default;
        Color& operator= (const Color&) = default;
        Color& operator= (Color&&) = default;
        ~Color() = default;

        constexpr void set_red(const float red) noexcept
        {
            m_red = red;
        }

        constexpr void set_green(const float green) noexcept
        {
            m_green = green;
        }

        constexpr void set_blue(const float blue) noexcept
        {
            m_blue = blue;
        }

        constexpr void set_alpha(const float alpha) noexcept
        {
            m_alpha = alpha;
        }

        constexpr float get_red() const noexcept
        {
            return m_red;
        }

        constexpr float get_green() const noexcept
        {
            return m_green;
        }

        constexpr float get_blue() const noexcept
        {
            return m_blue;
        }

        constexpr float get_alpha() const noexcept
        {
            return m_alpha;
        }

    private:
        float m_red, m_green, m_blue, m_alpha;
    };

    class Polygon
    {
    public:
        Polygon() = default;
        Polygon(const Polygon&) = default;
        Polygon(Polygon&&) = default;
        Polygon& operator= (const Polygon&) = default;
        Polygon& operator= (Polygon&&) = default;
        ~Polygon() = default;

    private:
        eqx::exp::Polygon m_polygon;
        Color m_color;
    };
}

constexpr std::vector<float> make_vertex_buffer(
    const eqx::exp::Polygon& poly) noexcept
{
    auto result = std::vector<float>{};
    result.reserve(std::ranges::size(poly.get_vertices()) * 6_uz);
    std::ranges::for_each(poly.get_vertices(),
        [&result](const eqx::Point<float>& point)
        {
            result.emplace_back(point.x);
            result.emplace_back(point.y);
            result.emplace_back(1.0f);
            result.emplace_back(0.0f);
            result.emplace_back(0.0f);
            result.emplace_back(1.0f);
        });
    return result;
}

void run() noexcept
{
    auto poly = eqx::exp::Polygon{};

    poly.add_vertex(eqx::Point<float>{100.0f, 100.0f});
    poly.add_vertex(eqx::Point<float>{50.0f, 150.0f});
    poly.add_vertex(eqx::Point<float>{0.0f, 100.0f});
    poly.add_vertex(eqx::Point<float>{0.0f, 0.0f});
    poly.add_vertex(eqx::Point<float>{100.0f, 0.0f});

    poly.set_center(eqx::Point<float>{0.0f, 0.0f});

    constexpr std::string_view vss = "#version 330 core\n"sv
        "layout (location = 0) in vec2 aPos;\n"sv
        "layout (location = 1) in vec4 aColor;\n"sv
        "out vec4 Color;\n"sv
        "uniform mat4 view;\n"sv
        "uniform mat4 proj;\n"sv
        "void main()\n"sv
        "{\n"sv
        "   Color = aColor;\n"sv
        "   gl_Position = proj * view * vec4(aPos, 0.0f, 1.0f);\n"sv
        "}\n\0"sv;
    constexpr std::string_view fss = "#version 330 core\n"sv
        "in vec4 Color;\n"sv
        "out vec4 FragColor;\n"sv
        "void main()\n"sv
        "{\n"sv
        "   FragColor = Color;\n"sv
        "}\n\0"sv;

    auto name = std::string{};
    auto view = ::glm::mat4{1.0F};
    auto proj = ::glm::mat4{1.0F};
    proj = ::glm::ortho(
        0.0f, static_cast<float>(c_width), static_cast<float>(-c_height), 0.0f);

    auto window = eqx::ogl::Window{c_width, c_height, "Test OGL"sv};

    auto context = eqx::ogl::Context{};
    context.set_shader(vss, fss);
    context.set_vertex_array(make_vertex_buffer(poly), poly.get_fan());
    context.set_uniform_mat4("view"sv, view);
    context.set_uniform_mat4("proj"sv, proj);

    std::cout << "Version: " << glGetString(GL_VERSION) << '\n';
    std::cout << "Max Textures: "
        << GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS << '\n';

    while (!window.should_close())
    {
        window.clear();

        name = "OGL Test --- Mouse Position: ";
        name += window.get_cursor_location().toString();

        poly.set_center(window.get_cursor_location());
        poly.rotate(1.0f);

        context.set_vertex_array(make_vertex_buffer(poly), poly.get_fan());
        window.set_name(name);

        context.draw();

        window.swap();
        window.poll();
    }

}

int main()
{
    std::cout << "Start\n"sv;

    eqx::ogl::init();

    run();

    eqx::ogl::close();

    std::cout << "\nEnd\n"sv;
    return 0;
}
