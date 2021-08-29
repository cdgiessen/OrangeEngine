#pragma once

#include "math/vector.h"

typedef struct GLFWwindow GLFWwindow;

namespace Input
{
// Keycodes reflect ascii encodings, for simplicity's sake
enum class KeyCode
{
    SPACE = 32,
    APOSTROPHE = 39, /* ' */
    COMMA = 44,      /* , */
    MINUS = 45,      /* - */
    PERIOD = 46,     /* . */
    SLASH = 47,      /* / */
    DIGIT_0 = 48,
    DIGIT_1 = 49,
    DIGIT_2 = 50,
    DIGIT_3 = 51,
    DIGIT_4 = 52,
    DIGIT_5 = 53,
    DIGIT_6 = 54,
    DIGIT_7 = 55,
    DIGIT_8 = 56,
    DIGIT_9 = 57,
    SEMICOLON = 59, /* ; */
    EQUAL = 61,     /* = */
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    LEFT_BRACKET = 91,  /* [ */
    BACKSLASH = 92,     /* \ */
    RIGHT_BRACKET = 93, /* ] */
    GRAVE_ACCENT = 96,  /* ` */
    WORLD_1 = 161,      /* non-US #1 */
    WORLD_2 = 162,      /* non-US #2 */

    /* Function keys */
    ESCAPE = 256,
    ENTER = 257,
    TAB = 258,
    BACKSPACE = 259,
    INSERT = 260,
    DEL = 261,
    RIGHT = 262,
    LEFT = 263,
    DOWN = 264,
    UP = 265,
    PAGE_UP = 266,
    PAGE_DOWN = 267,
    HOME = 268,
    END = 269,
    CAPS_LOCK = 280,
    SCROLL_LOCK = 281,
    NUM_LOCK = 282,
    PRINT_SCREEN = 283,
    PAUSE = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,
    KP_0 = 320,
    KP_1 = 321,
    KP_2 = 322,
    KP_3 = 323,
    KP_4 = 324,
    KP_5 = 325,
    KP_6 = 326,
    KP_7 = 327,
    KP_8 = 328,
    KP_9 = 329,
    KP_DECIMAL = 330,
    KP_DIVIDE = 331,
    KP_MULTIPLY = 332,
    KP_SUBTRACT = 333,
    KP_ADD = 334,
    KP_ENTER = 335,
    KP_EQUAL = 336,
    LEFT_SHIFT = 340,
    LEFT_CONTROL = 341,
    LEFT_ALT = 342,
    LEFT_SUPER = 343,
    RIGHT_SHIFT = 344,
    RIGHT_CONTROL = 345,
    RIGHT_ALT = 346,
    RIGHT_SUPER = 347,
    MENU = 348,
};
}; // namespace Input
class Window
{
    public:
    // Call at app startup and before shutdown
    static void static_initialization() noexcept;
    static void static_shutdown() noexcept;
    static bool is_static_initialized;

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

    void show_window(bool show = true);
    void set_size_limits(math::vec2i min_size, math::vec2i max_size);

    GLFWwindow* handle() const;
    bool should_window_resize() const;
    void set_resize_done();
    bool should_close() const;
    void set_close();

    bool is_iconified() const;
    bool is_focused() const;

    math::vec2i get_window_size() const;

    bool get_key(Input::KeyCode code) const;
    bool get_key_down(Input::KeyCode code) const;
    bool get_key_up(Input::KeyCode code) const;

    bool get_mouse_button(int button) const;
    bool get_mouse_button_pressed(int button) const;
    bool get_mouse_button_released(int button) const;

    math::vec2d get_mouse_position() const;
    math::vec2d get_mouse_change_in_position() const;

    math::vec2d get_mouse_scroll() const;
    double get_mouse_scroll_x() const;
    double get_mouse_scroll_y() const;

    void set_text_input_mode();
    void reset_text_input_mode();
    bool get_text_input_mode() const;

    MouseControl get_mouse_control_status() const;
    void set_mouse_control_status(MouseControl value);

    void poll_events();
    void reset_released_input();

    void next_frame();

    private:
    static const uint32_t KeyboardButtonCount = 512;
    static const uint32_t MouseButtonCount = 15;
    static const uint32_t JoystickCount = 16;
    static const uint32_t JoystickAxisCount = 4;
    static const uint32_t JoystickButtonCount = 16;
    static const uint32_t TotalFrameCount = 2;

    enum class ButtonState
    {
        none,
        down,
        held,
        up
    };

    struct InputFrame
    {
        ButtonState keyboard_buttons[MouseButtonCount] = { { ButtonState::none } };
        ButtonState mouse_buttons[MouseButtonCount] = { { ButtonState::none } };

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
