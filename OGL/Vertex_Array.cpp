// Vertex_Array.cpp

export module Eqx.OGL.Vertex_Array;

import Eqx.OGL.Buffer;

import <Eqx/std.hpp>;
import <Eqx/TPL/glad/glad.hpp>;

export namespace eqx::ogl
{
    static_assert(std::same_as<GLuint, unsigned int>);

    class Vertex_Array
    {
    public:
        Vertex_Array(const Vertex_Array&) = delete;
        Vertex_Array& operator= (const Vertex_Array&) = delete;

        explicit constexpr Vertex_Array() noexcept
            :
            m_id(0u),
            m_vertex_buffer(),
            m_index_buffer()
        {
        }

        explicit inline Vertex_Array(const std::span<const float> vertex_data,
            const std::span<const unsigned int> attribs,
            const std::span<const unsigned int> index_data) noexcept
            :
            m_id(0u),
            m_vertex_buffer(),
            m_index_buffer()
        {
            this->init(vertex_data, attribs, index_data);
        }

        constexpr Vertex_Array(Vertex_Array&& other) noexcept
            :
            m_id(std::exchange(other.m_id, 0u)),
            m_index_count(std::exchange(other.m_index_count, 0ull)),
            m_vertex_buffer(std::move(other.m_vertex_buffer)),
            m_index_buffer(std::move(other.m_index_buffer))
        {
        }

        constexpr Vertex_Array& operator= (Vertex_Array&& other) noexcept
        {
            std::swap(this->m_id, other.m_id);
            std::swap(this->m_index_count, other.m_index_count);
            this->m_vertex_buffer = std::move(other.m_vertex_buffer);
            this->m_index_buffer = std::move(other.m_index_buffer);

            return *this;
        }

        constexpr ~Vertex_Array() noexcept
        {
            if (this->valid()) [[likely]]
            {
                this->free();
            }
        }

        inline void init(const std::span<const float> vertex_data,
            const std::span<const unsigned int> attribs,
            const std::span<const unsigned int> index_data) noexcept
        {
            assert(!this->valid());

            glGenVertexArrays(1, &this->m_id);

            this->m_vertex_buffer.init();
            this->m_index_buffer.init();

            this->set_vertex_buffer(vertex_data, attribs);
            this->set_index_buffer(index_data);

            assert(this->valid());
        }

        inline void enable() const noexcept
        {
            assert(this->m_id != 0u);

            glBindVertexArray(m_id);
        }

        inline void disable() const noexcept
        {
            assert(this->valid());

            glBindVertexArray(0u);
        }

        inline void set_vertex_buffer(const std::span<const float> vertices,
            const std::span<const unsigned int> attribs) const noexcept
        {
            assert(std::ranges::size(attribs) <= 8);

            this->enable();
            m_vertex_buffer.buffer_data(vertices);

            const auto sum = std::reduce(std::ranges::begin(attribs),
                std::ranges::end(attribs));
            auto p_sums = std::array<unsigned int, 8>{};
            std::exclusive_scan(std::ranges::begin(attribs),
                std::ranges::end(attribs), std::ranges::begin(p_sums), 0);

            for (auto i = 0; i < std::ranges::size(attribs); ++i)
            {
                glEnableVertexAttribArray(i);
                glVertexAttribPointer(i, attribs[i], GL_FLOAT, GL_FALSE,
                    sum * sizeof(float), (void*)(p_sums.at(i) * sizeof(float)));
            }

            /*
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                (void*)(2 * sizeof(float)));
                */
        }

        inline void set_index_buffer(
            const std::span<const unsigned int> indices) noexcept
        {
            this->enable();
            this->m_index_buffer.buffer_data(indices);
            this->m_index_count = std::ranges::size(indices);
        }

        [[nodiscard]] constexpr GLuint get_id() const noexcept
        {
            return this->m_id;
        }

        [[nodiscard]] constexpr std::size_t get_index_count() const noexcept
        {
            return this->m_index_count;
        }

        [[nodiscard]] constexpr bool valid() const noexcept
        {
            return this->m_id != 0u
                && this->m_index_count != 0ull
                && this->m_vertex_buffer.valid()
                && this->m_index_buffer.valid();
        }

        inline void free() noexcept
        {
            assert(this->valid());

            glDeleteVertexArrays(1, &m_id);
            this->m_vertex_buffer.free();
            this->m_index_buffer.free();
            this->m_index_count = 0ull;

            assert(!this->valid());
        }

    private:
        GLuint m_id;
        std::size_t m_index_count;

        Vertex_Buffer m_vertex_buffer;
        Index_Buffer m_index_buffer;
    };
}
