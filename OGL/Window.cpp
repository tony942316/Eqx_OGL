// Window.cpp

export module Eqx.OGL.Window;

import <Eqx/std.hpp>;
import <Eqx/TPL/glad/glad.hpp>;
import <Eqx/TPL/glfw/glfw.hpp>;
import Eqx.Lib;

using namespace std::literals;

export namespace eqx::ogl
{
    inline void init() noexcept
    {
        [[maybe_unused]] const auto ec = glfwInit();
        assert(ec != GLFW_FALSE);

        glfwSetErrorCallback([](int code, const char* msg)
            {
                std::format_to(std::ostream_iterator<char>(std::cerr),
                    "glfw Error ->\n"sv
                    "Code: {}\n"sv
                    "Message: {}\n"sv, code, msg);

                assert(false);
            });

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    inline void free() noexcept
    {
        glfwTerminate();
    }

    inline void draw(const std::size_t index_count) noexcept
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_count),
            GL_UNSIGNED_INT, 0);
    }

    class Window
    {
    public:
        enum class Key
        {
            W = GLFW_KEY_W,
            A = GLFW_KEY_A,
            S = GLFW_KEY_S,
            D = GLFW_KEY_D,
            Up = GLFW_KEY_UP,
            Left = GLFW_KEY_LEFT,
            Down = GLFW_KEY_DOWN,
            Right = GLFW_KEY_RIGHT,
            R = GLFW_KEY_R,
            Space = GLFW_KEY_SPACE,
            Escape = GLFW_KEY_ESCAPE
        };

        enum class Key_State
        {
            Up = GLFW_RELEASE,
            Down = GLFW_PRESS
        };

        Window(const Window&) = delete;
        Window& operator= (const Window&) = delete;

        explicit constexpr Window() noexcept
            :
            m_window(nullptr)
        {
        }

        explicit inline Window(const int width, const int height,
            const std::string_view name) noexcept
            :
            m_window(nullptr)
        {
            this->init(width, height, name);
        }

        inline Window(Window&& other) noexcept
            :
            m_window(std::exchange(other.m_window, nullptr))
        {
        }

        inline Window& operator= (Window&& other) noexcept
        {
            std::swap(m_window, other.m_window);
        }

        inline ~Window() noexcept
        {
            if (m_window != nullptr)
            {
                this->free();
            }
        }

        inline void init(const int width, const int height,
            const std::string_view name) noexcept
        {
            assert(m_window == nullptr);

            m_window =
                glfwCreateWindow(width, height, name.data(), nullptr, nullptr);
            assert(m_window != nullptr);

            glfwMakeContextCurrent(m_window);
            [[maybe_unused]] const auto ec = gladLoadGLLoader(
                reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
            assert(ec != 0);

            glViewport(0, 0, width, height);
            glfwSetFramebufferSizeCallback(m_window,
                []([[maybe_unused]] GLFWwindow* window, int width, int height)
                {
                    glViewport(0, 0, width, height);
                });
            this->vsync(true);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        }

        inline void free() noexcept
        {
            assert(m_window != nullptr);

            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }

        inline eqx::lib::Point<float> get_cursor_location() const noexcept
        {
            assert(m_window != nullptr);

            auto x = 0.0;
            auto y = 0.0;
            glfwGetCursorPos(m_window, &x, &y);
            return eqx::lib::Point<float>{ static_cast<float>(x),
                static_cast<float>(-y) };
        }

        [[nodiscard]] inline Key_State key_state(Key key) const noexcept
        {
            assert(m_window != nullptr);

            return static_cast<Key_State>(
                glfwGetKey(m_window, static_cast<decltype(GLFW_KEY_W)>(key)));
        }

        [[nodiscard]] inline bool key_down(Key key) const noexcept
        {
            return this->key_state(key) == Key_State::Down;
        }

        [[nodiscard]] inline bool key_up(Key key) const noexcept
        {
            return !this->key_down(key);
        }

        inline void set_name(const std::string_view name) const noexcept
        {
            assert(m_window != nullptr);

            glfwSetWindowTitle(m_window, name.data());
        }

        inline void vsync(const bool on) const noexcept
        {
            glfwSwapInterval(on == true ? 1 : 0);
        }

        inline void clear() const noexcept
        {
            assert(m_window != nullptr);

            glClear(GL_COLOR_BUFFER_BIT);
        }

        inline void swap() const noexcept
        {
            assert(m_window != nullptr);

            glfwSwapBuffers(m_window);
        }

        inline void poll() const noexcept
        {
            assert(m_window != nullptr);

            glfwPollEvents();
        }

        inline bool should_close() const noexcept
        {
            assert(m_window != nullptr);

            return glfwWindowShouldClose(m_window) == GLFW_TRUE;
        }

        inline void close() const noexcept
        {
            assert(m_window != nullptr);

            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }

    private:
        GLFWwindow* m_window;
    };

    class Frame_Timer
    {
    public:
        Frame_Timer(const Frame_Timer&) = default;
        Frame_Timer(Frame_Timer&&) = default;
        Frame_Timer& operator= (const Frame_Timer&) = default;
        Frame_Timer& operator= (Frame_Timer&&) = default;
        ~Frame_Timer() = default;

        explicit inline Frame_Timer() noexcept
            :
            m_timer(),
            m_fps(0.0F),
            m_frames(0ULL)
        {
            m_timer.start();
        }

        inline void update() noexcept
        {
            ++this->m_frames;
            if (this->m_timer.readf() >= 1.0F) [[unlikely]]
            {
                this->m_fps = this->m_frames / m_timer.secondsf();
                this->m_frames = 0ULL;
                this->m_timer.start();
            }
        }

        [[nodiscard]] constexpr float get_fps() const noexcept
        {
            return this->m_fps;
        }

        [[nodiscard]] constexpr float get_frames() const noexcept
        {
            return this->m_frames;
        }

    private:
        eqx::lib::Timer m_timer;
        float m_fps;
        unsigned long long m_frames;
    };
}
