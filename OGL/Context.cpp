export module Eqx.OGL.Context;

import <Eqx/std.hpp>;

import <Eqx/TPL/glad/glad.hpp>;
import <Eqx/TPL/glm/glm.hpp>;

import Eqx.Lib;

import Eqx.OGL.Vertex_Array;
import Eqx.OGL.Shader_Program;

export namespace eqx::ogl
{
    class Context
    {
    public:
        Context() = default;
        Context(const Context&) = default;
        Context(Context&&) = default;
        Context& operator= (const Context&) = default;
        Context& operator= (Context&&) = default;
        ~Context() = default;

        inline void set_shader(const std::string_view vss,
            const std::string_view fss) const noexcept
        {
            m_shader_program.link(vss, fss);
        }

        inline void set_vertex_array(const std::span<const float> vertices,
            const std::span<const unsigned int> indices)
        {
            m_vertex_array.set_vertex_buffer(vertices,
                std::array<std::uint8_t, 2_uz>{ 2_u8, 4_u8 });
            m_vertex_array.set_index_buffer(indices);
        }

        inline void set_uniform_mat4(const std::string_view name,
            const glm::mat4& mat) const noexcept
        {
            m_shader_program.set_mat4(name, mat);
        }

        inline void draw() const noexcept
        {
            m_vertex_array.enable();
            m_shader_program.enable();

            glDrawElements(GL_TRIANGLES, m_vertex_array.get_index_count(),
                GL_UNSIGNED_INT, 0);
        }

    private:
        Vertex_Array m_vertex_array;
        Shader_Program m_shader_program;
    };
}
