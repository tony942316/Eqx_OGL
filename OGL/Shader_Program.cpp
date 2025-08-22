export module Eqx.OGL.Shader_Program;

import Eqx.OGL.Shader_Source;

import <Eqx/std.hpp>;
import <Eqx/TPL/glad/glad.hpp>;
import <Eqx/TPL/glm/glm.hpp>;

using namespace std::literals;

export namespace eqx::ogl
{
    static_assert(std::same_as<GLuint, unsigned int>);

    class Shader_Program
    {
    public:
        Shader_Program(const Shader_Program&) = delete;
        Shader_Program& operator= (const Shader_Program&) = delete;

        explicit constexpr Shader_Program() noexcept
            :
            m_id(0u)
        {
        }

        explicit inline Shader_Program(const Vertex_Shader_Source& vs,
            const Fragment_Shader_Source& fs) noexcept
            :
            m_id(0u)
        {
            this->init(vs, fs);
        }

        constexpr Shader_Program(Shader_Program&& other) noexcept
            :
            m_id(std::exchange(other.m_id, 0u))
        {
        }

        constexpr Shader_Program& operator= (Shader_Program&& other) noexcept
        {
            std::swap(this->m_id, other.m_id);
            return *this;
        }

        constexpr ~Shader_Program() noexcept
        {
            if (this->valid()) [[likely]]
            {
                this->free();
            }
        }

        inline void init(const Vertex_Shader_Source& vertex,
            const Fragment_Shader_Source& fragment) noexcept
        {
            assert(!this->valid());

            this->m_id = glCreateProgram();

            assert(this->valid());
            assert(vertex.valid());
            assert(fragment.valid());

            glAttachShader(this->m_id, vertex.get_id());
            glAttachShader(this->m_id, fragment.get_id());
            glLinkProgram(this->m_id);
        }

        inline void enable() const noexcept
        {
            assert(this->valid());

            glUseProgram(m_id);
        }

        inline void disable() const noexcept
        {
            assert(this->valid());

            glUseProgram(0u);
        }

        inline void set_mat4(const std::string_view name,
            const glm::mat4& mat) const noexcept
        {
            assert(this->valid());

            this->enable();
            glUniformMatrix4fv(glGetUniformLocation(this->m_id, name.data()),
                1, GL_FALSE, &mat[0][0]);
        }

        inline void activate_texture(const std::string_view name,
            const int tex) const noexcept
        {
            this->enable();
            glUniform1i(glGetUniformLocation(this->m_id, name.data()), tex);
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

            glDeleteProgram(m_id);
            this->m_id = 0u;

            assert(!this->valid());
        }

        [[nodiscard]] static inline Shader_Program from_source(
            const std::string_view vss, const std::string_view fss) noexcept
        {
            return Shader_Program{ Vertex_Shader_Source{ vss },
                Fragment_Shader_Source{ fss }};
        }

        [[nodiscard]] static inline Shader_Program from_files(
            const std::filesystem::path& vsf,
            const std::filesystem::path& fsf) noexcept
        {
            return Shader_Program{ Vertex_Shader_Source::from_file(vsf),
                Fragment_Shader_Source::from_file(fsf) };
        }

    private:
        GLuint m_id;
    };
}
