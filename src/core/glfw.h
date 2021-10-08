#pragma once

#include "core/input.h"
#include "math/vector.h"

typedef struct GLFWwindow GLFWwindow;

class Window
{
    public:
    // Call at app startup and before shutdown
    static void static_initialization() noexcept;
    static void static_shutdown() noexcept;
    static bool is_static_initialized;

    // Poll for pending events, operates on all windows
    static void poll_events();

    enum class InitialMode
    {
        windowed,
        fullscreen
    };
    enum class MouseControl
    {
        normal,
        disabled,
        hidden
    };
    struct CreateDetails
    {
        InitialMode initial_mode = InitialMode::windowed;
        MouseControl mouse_mode = MouseControl::normal;
        const char* window_title = "";
        math::vec2i size = math::vec2i{ 200, 200 };
        math::vec2i position = math::vec2i{ 0, 0 };
    };

    Window(CreateDetails create_details) noexcept;
    ~Window() noexcept;
    Window(Window const&) = delete;
    Window& operator=(Window const&) = delete;
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    GLFWwindow* handle() const;

    void show(bool show = true);
    void set_size_limits(math::vec2i min_size, math::vec2i max_size);
    [[nodiscard]] math::vec2i get_window_size() const;

    [[nodiscard]] bool should_resize() const;
    void finish_resize();
    [[nodiscard]] bool should_close() const;
    void set_close();

    [[nodiscard]] bool is_iconified() const;
    [[nodiscard]] bool is_focused() const;

    [[nodiscard]] bool get_key(Input::KeyCode code) const;
    [[nodiscard]] bool get_key_down(Input::KeyCode code) const;
    [[nodiscard]] bool get_key_up(Input::KeyCode code) const;
    // For text input controls
    [[nodiscard]] bool get_key_repeat(Input::KeyCode code) const;

    [[nodiscard]] bool get_mouse_button(int button) const;
    [[nodiscard]] bool get_mouse_button_pressed(int button) const;
    [[nodiscard]] bool get_mouse_button_released(int button) const;

    [[nodiscard]] math::vec2d get_mouse_position() const;
    [[nodiscard]] math::vec2d get_mouse_change_in_position() const;

    [[nodiscard]] math::vec2d get_mouse_scroll() const;

    void set_text_input_mode();
    void reset_text_input_mode();
    [[nodiscard]] bool get_text_input_mode() const;

    [[nodiscard]] MouseControl get_mouse_control_status() const;
    void set_mouse_control_status(MouseControl value);

    void reset_released_input();

    void next_frame();

    private:
    static const uint32_t KeyboardButtonCount = 512;
    static const uint32_t MouseButtonCount = 15;
    static const uint32_t JoystickCount = 16;
    static const uint32_t JoystickAxisCount = 4;
    static const uint32_t JoystickButtonCount = 16;
    static const uint32_t TotalFrameCount = 1;

    struct InputFrame
    {
        Input::ButtonState keyboard_buttons[KeyboardButtonCount] = { { Input::ButtonState::none } };
        Input::ButtonState mouse_buttons[MouseButtonCount] = { { Input::ButtonState::none } };

        bool test_input_mode = false;
        MouseControl mouse_control_status = MouseControl::normal;
        math::vec2d mouse_position{};
        math::vec2d mouse_position_previous{};
        math::vec2d mouse_change_in_position{};
        math::vec2d mouse_scroll{};
    };
    void key_event(int key, int scancode, int action, int mods);
    void mouse_button_event(int button, int action, int mods);
    void mouse_move_event(double xoffset, double yoffset);
    void mouse_scroll_event(double xoffset, double yoffset);

    static void error_handler(int error, const char* description);
    static void keyboard_handler(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void char_input_handler(GLFWwindow* window, uint32_t codePoint);
    static void mouse_button_handler(GLFWwindow* window, int button, int action, int mods);
    static void mouse_move_handler(GLFWwindow* window, double posx, double posy);
    static void mouse_scroll_handler(GLFWwindow* window, double xoffset, double yoffset);
    static void framebuffer_size_handler(GLFWwindow* window, int width, int height);
    static void window_resize_handler(GLFWwindow* window, int width, int height);
    static void window_focus_handler(GLFWwindow* window, int focused);
    static void window_iconify_handler(GLFWwindow* window, int iconified);
    static void window_close_handler(GLFWwindow* window);

    InputFrame& frame();
    InputFrame const& frame() const;

    GLFWwindow* window;
    struct State
    {
        math::vec2i current_window_size;
        bool update_window_size = false;
        bool should_close_window = false;
        bool _is_iconified = false;
        bool _is_focused = false;

        uint32_t current_frame = 0;
        InputFrame _frame[TotalFrameCount];
    } state;
};
