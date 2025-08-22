// Texture.cpp

export module Eqx.OGL.Texture;

import <Eqx/std.hpp>;
import <Eqx/TPL/glad/glad.hpp>;
import <Eqx/TPL/stb/stb_image.hpp>;

using namespace std::literals;

export namespace eqx::ogl
{
    static_assert(std::same_as<GLuint, unsigned int>);

    class Texture
    {
    public:
        Texture(const Texture&) = delete;
        Texture& operator= (const Texture&) = delete;

        explicit constexpr Texture() noexcept
            :
            m_id(0u)
        {
        }

        explicit inline Texture(const std::filesystem::path& path) noexcept
            :
            m_id(0u)
        {
            this->init(path);
        }

        constexpr Texture(Texture&& other) noexcept
            :
            m_id(std::exchange(other.m_id, 0u))
        {
        }

        constexpr Texture& operator= (Texture&& other) noexcept
        {
            std::swap(this->m_id, other.m_id);
            return *this;
        }

        constexpr ~Texture() noexcept
        {
            if (this->valid()) [[likely]]
            {
                this->free();
            }
        }

        inline void init(const std::filesystem::path& path) noexcept
        {
            auto w = 0;
            auto h = 0;
            auto ch = 0;

            unsigned char* data = stbi_load(path.string().c_str(),
                &w, &h, &ch, 0);
            assert(data != nullptr);

            auto fmt = GL_RGBA;
            switch(ch)
            {
            case 1:
                fmt = GL_RED;
                break;
            case 3:
                fmt = GL_RGB;
                break;
            default:
                fmt = GL_RGBA;
                break;
            }

            glGenTextures(1, &this->m_id);
            assert(this->valid());
            this->enable(0);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glTexImage2D(GL_TEXTURE_2D, 0, (fmt==GL_RGB?GL_RGB8:GL_RGBA8), w, h,
                0, fmt, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            stbi_image_free(data);
        }

        inline void enable(const int i) const noexcept
        {
            assert(this->valid());

            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, this->m_id);
        }

        inline void disable(const int i) const noexcept
        {
            assert(this->valid());

            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0u);
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

            glDeleteTextures(1, &this->m_id);
            this->m_id = 0u;

            assert(!this->valid());
        }

    private:
        GLuint m_id;
    };
}
