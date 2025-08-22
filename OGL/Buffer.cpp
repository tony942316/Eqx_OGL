// Buffer.cpp

export module Eqx.OGL.Buffer;

import <Eqx/std.hpp>;
import <Eqx/TPL/glad/glad.hpp>;

export namespace eqx::ogl
{
    static_assert(std::same_as<GLuint, unsigned int>);
    static_assert(std::same_as<decltype(GL_ARRAY_BUFFER), int>);
    static_assert(std::same_as<decltype(GL_ELEMENT_ARRAY_BUFFER), int>);

    template <int t_buffer>
    class Buffer
    {
    public:
        Buffer(const Buffer&) = delete;
        Buffer& operator= (const Buffer&) = delete;

        explicit constexpr Buffer() noexcept
            :
            m_id(0u)
        {
        }

        explicit inline Buffer(const std::span<const float> data) noexcept
            :
            m_id(0u)
        {
            this->init();
            this->buffer_data(data);
        }

        explicit inline Buffer(
            const std::span<const unsigned int> data) noexcept
            :
            m_id(0u)
        {
            this->init();
            this->buffer_data(data);
        }

        constexpr Buffer(Buffer&& other) noexcept
            :
            m_id(std::exchange(other.m_id, 0u))
        {
        }

        constexpr Buffer& operator= (Buffer&& other) noexcept
        {
            std::swap(this->m_id, other.m_id);
            return *this;
        }

        constexpr ~Buffer() noexcept
        {
            if (this->valid()) [[likely]]
            {
                this->free();
            }
        }

        inline void init() noexcept
        {
            assert(!this->valid());

            glGenBuffers(1, &m_id);

            assert(this->valid());
        }

        inline void enable() const noexcept
        {
            assert(this->valid());

            glBindBuffer(t_buffer, this->m_id);
        }

        inline void disable() const noexcept
        {
            assert(this->valid());

            glBindBuffer(t_buffer, 0u);
        }

        inline void buffer_data(
            const std::span<const float> data) const noexcept
        {
            static_assert(t_buffer == GL_ARRAY_BUFFER);
            assert(this->valid());

            this->enable();
            glBufferData(t_buffer, std::ranges::size(data) * sizeof(float),
                data.data(), GL_STATIC_DRAW);
        }

        inline void buffer_data(
            const std::span<const unsigned int> data) const noexcept
        {
            static_assert(t_buffer == GL_ELEMENT_ARRAY_BUFFER);
            assert(this->valid());

            this->enable();
            glBufferData(t_buffer,
                std::ranges::size(data) * sizeof(unsigned int), data.data(),
                GL_STATIC_DRAW);
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

            glDeleteBuffers(1, &m_id);
            this->m_id = 0u;

            assert(!this->valid());
        }

    private:
        GLuint m_id;
    };

    using Vertex_Buffer = Buffer<GL_ARRAY_BUFFER>;
    using Index_Buffer = Buffer<GL_ELEMENT_ARRAY_BUFFER>;
}
