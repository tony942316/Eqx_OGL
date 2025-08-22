// Shader_Source.cpp

export module Eqx.OGL.Shader_Source;

import <Eqx/std.hpp>;
import <Eqx/TPL/glad/glad.hpp>;

using namespace std::literals;

export namespace eqx::ogl
{
    static_assert(std::same_as<GLuint, unsigned int>);
    static_assert(std::same_as<decltype(GL_VERTEX_SHADER), int>);

    template <decltype(GL_VERTEX_SHADER) t_shader_type>
    class Shader_Source
    {
    public:
        Shader_Source(const Shader_Source&) = delete;
        Shader_Source& operator= (const Shader_Source&) = delete;

        explicit constexpr Shader_Source() noexcept
            :
            m_id(0u)
        {
        }

        explicit inline Shader_Source(const std::string_view source) noexcept
            :
            m_id(0u)
        {
            this->init(source);
        }

        constexpr Shader_Source(Shader_Source&& other) noexcept
            :
            m_id(std::exchange(other.m_id, 0u))
        {
        }

        constexpr Shader_Source& operator= (Shader_Source&& other) noexcept
        {
            std::swap(this->m_id, other.m_id);
            return *this;
        }

        constexpr ~Shader_Source() noexcept
        {
            if (this->valid()) [[likely]]
            {
                this->free();
            }
        }

        inline void init(const std::string_view source) noexcept
        {
            assert(!this->valid());

            this->m_id = glCreateShader(t_shader_type);

            assert(this->valid());

            auto source_data = source.data();
            glShaderSource(this->m_id, 1, &source_data, nullptr);
            glCompileShader(this->m_id);

            assert(this->ok());
        }

        [[nodiscard]] inline bool ok() const noexcept
        {
            assert(this->valid());

            auto result = GL_FALSE;
            glGetShaderiv(this->m_id, GL_COMPILE_STATUS, &result);

            auto log_len = 0;
            glGetShaderiv(this->m_id, GL_INFO_LOG_LENGTH, &log_len);

            auto log = std::string{};
            log.resize(log_len);
            std::ranges::fill(log, '\0');
            if (log_len > 1)
            {
                GLsizei written = 0;
                glGetShaderInfoLog(this->m_id, log_len, &written, log.data());
                std::format_to(std::ostream_iterator<char>(std::cerr),
                    "[Shader {}] {}\n"sv,
                    result == GL_TRUE ? "Warning"sv : "Error"sv, log);
            }
            return result;
        }

        [[nodiscard]] constexpr GLuint get_id() const noexcept
        {
            return this->m_id;
        }

        [[nodiscard]] constexpr bool valid() const noexcept
        {
            return this->m_id != 0u;
        }

        inline void free() noexcept
        {
            assert(this->valid());

            glDeleteShader(this->m_id);
            this->m_id = 0u;

            assert(!this->valid());
        }

        [[nodiscard]] static inline Shader_Source<t_shader_type> from_file(
            const std::filesystem::path& path) noexcept
        {
            assert(std::filesystem::exists(path));

            auto file = std::ifstream{ path, std::ios::in };
            assert(file.is_open());

            return Shader_Source<t_shader_type>{
                (std::stringstream{} << file.rdbuf()).str() };
        }

    private:
        GLuint m_id;
    };

    using Vertex_Shader_Source = Shader_Source<GL_VERTEX_SHADER>;
    using Fragment_Shader_Source = Shader_Source<GL_FRAGMENT_SHADER>;
}
