#include "glfw.h"

#include <cassert>
#include <type_traits>

#include <GLFW/glfw3.h>
#include "spdlog/spdlog.h"

void Window::static_initialization() noexcept
{
    assert(is_static_initialized == false);
    glfwInit();
    is_static_initialized = true;
}
void Window::static_shutdown() noexcept
{
    assert(is_static_initialized == true);
    glfwTerminate();
    is_static_initialized = false;
}
bool Window::is_static_initialized = false;

void Window::poll_events()
{
    assert(is_static_initialized == true);
    glfwPollEvents();
}


template <typename Enumeration>
auto as_integer(Enumeration const value) -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

Window::Window(Window::CreateDetails create_details) noexcept
{
    assert(is_static_initialized == true);

    if (create_details.window_title == nullptr) create_details.window_title = "";
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    if (create_details.initial_mode == InitialMode::fullscreen)
    {
        GLFWmonitor* primary = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primary);

        window = glfwCreateWindow(mode->width, mode->height, create_details.window_title, primary, NULL);
        spdlog::info("Monitor Dimensions {{ {}, {} }}", mode->width, mode->height);
    }
    else
    {
        window = glfwCreateWindow(
            create_details.size.x, create_details.size.y, create_details.window_title, NULL, NULL);
        if (create_details.position != math::vec2i{ INT_MIN, INT_MIN })
        {
            glfwSetWindowPos(window, create_details.position.x, create_details.position.y);
        }
    }

    // Prepare window
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_handler);
    glfwSetWindowSizeCallback(window, window_resize_handler);
    glfwSetWindowIconifyCallback(window, window_iconify_handler);
    glfwSetWindowFocusCallback(window, window_focus_handler);
    glfwSetWindowCloseCallback(window, window_close_handler);
    glfwSetErrorCallback(error_handler);

    // Set input callbacks
    glfwSetKeyCallback(window, keyboard_handler);
    glfwSetCharCallback(window, char_input_handler);
    glfwSetMouseButtonCallback(window, mouse_button_handler);
    glfwSetCursorPosCallback(window, mouse_move_handler);
    glfwSetScrollCallback(window, mouse_scroll_handler);

    state._is_iconified = glfwGetWindowAttrib(window, GLFW_ICONIFIED);
    state._is_focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);

    state.current_window_size = get_window_size();

    math::vec2d pos = {};
    glfwGetCursorPos(window, &pos.x, &pos.y);
    frame().mouse_position = frame().mouse_position_previous = pos;

    set_mouse_control_status(create_details.mouse_mode);
}

Window::~Window() noexcept
{
    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }
}
Window::Window(Window&& other) noexcept : window(other.window), state(other.state)
{
    other.window = nullptr;
}
Window& Window::operator=(Window&& other) noexcept
{
    if (this != &other)
    {
        if (window)
        {
            glfwDestroyWindow(window);
        }
        window = other.window;
        state = other.state;
        other.window = nullptr;
    }
    return *this;
}

GLFWwindow* Window::handle() const { return window; }

void Window::show(bool show)
{
    if (show)
    {
        glfwShowWindow(window);
    }
    else
    {
        glfwHideWindow(window);
    }
}

void Window::set_size_limits(math::vec2i min_size, math::vec2i max_size)
{
    glfwSetWindowSizeLimits(window,
        min_size.x,
        min_size.y,
        max_size.x ? max_size.x : min_size.x,
        max_size.y ? max_size.y : min_size.y);
}

math::vec2i Window::get_window_size() const
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return math::vec2i(width, height);
}

bool Window::should_resize() const { return state.update_window_size; }

void Window::finish_resize() { state.update_window_size = false; }

bool Window::should_close() const { return state.should_close_window; }

void Window::set_close()
{
    state.should_close_window = true;
    glfwSetWindowShouldClose(window, true);
}

bool Window::is_iconified() const { return state._is_iconified; }

bool Window::is_focused() const { return state._is_focused; }

bool Window::get_key(Input::KeyCode code) const
{
    auto button_state = frame().keyboard_buttons[as_integer(code)];
    return button_state == Input::ButtonState::down || button_state == Input::ButtonState::held ||
           button_state == Input::ButtonState::repeat;
}

bool Window::get_key_down(Input::KeyCode code) const
{
    return frame().keyboard_buttons[as_integer(code)] == Input::ButtonState::down;
}

bool Window::get_key_up(Input::KeyCode code) const
{
    return frame().keyboard_buttons[as_integer(code)] == Input::ButtonState::up;
}

bool Window::get_key_repeat(Input::KeyCode code) const
{
    return frame().keyboard_buttons[as_integer(code)] == Input::ButtonState::repeat;
}

bool Window::get_mouse_button(int button) const
{
    auto button_state = frame().mouse_buttons[button];
    return button_state == Input::ButtonState::down || button_state == Input::ButtonState::held;
}
bool Window::get_mouse_button_pressed(int button) const
{
    return frame().mouse_buttons[button] == Input::ButtonState::down;
}
bool Window::get_mouse_button_released(int button) const
{
    return frame().mouse_buttons[button] == Input::ButtonState::up;
}
math::vec2d Window::get_mouse_position() const { return frame().mouse_position; }
math::vec2d Window::get_mouse_change_in_position() const
{
    return frame().mouse_change_in_position;
}
math::vec2d Window::get_mouse_scroll() const { return frame().mouse_scroll; }
void Window::set_text_input_mode() { frame().test_input_mode = true; }
void Window::reset_text_input_mode() { frame().test_input_mode = false; }
bool Window::get_text_input_mode() const { return frame().test_input_mode; }

Window::MouseControl Window::get_mouse_control_status() const
{
    return frame().mouse_control_status;
}

void Window::set_mouse_control_status(MouseControl value)
{
    frame().mouse_control_status = value;
    spdlog::info("Set mouse control status to {}", static_cast<int>(value));
    switch (frame().mouse_control_status)
    {
        default:
        case Window::MouseControl::normal:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case Window::MouseControl::disabled:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        case Window::MouseControl::hidden:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
    }
}

void Window::reset_released_input() { frame() = InputFrame{}; }
void Window::next_frame()
{
    for (auto& key : frame().keyboard_buttons)
    {
        // Down Keys get promoted to held after one frame
        if (key == Input::ButtonState::down) key = Input::ButtonState::held;
        // Up keys get promoted to none after one frame (eg released)
        if (key == Input::ButtonState::up) key = Input::ButtonState::none;
    }

    state.current_frame = (state.current_frame + 1) % TotalFrameCount;
}


void Window::key_event(int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods)
{
    switch (action)
    {
        case GLFW_PRESS:
            frame().keyboard_buttons[key] = Input::ButtonState::down;
            break;

        case GLFW_REPEAT:
            frame().keyboard_buttons[key] = Input::ButtonState::repeat;
            break;

        case GLFW_RELEASE:
            frame().keyboard_buttons[key] = Input::ButtonState::up;
            break;

        default:
            break;
    }
}

void Window::mouse_button_event(int button, int action, [[maybe_unused]] int mods)
{
    switch (action)
    {
        case GLFW_PRESS:
            frame().mouse_buttons[button] = Input::ButtonState::down;
            break;

        case GLFW_REPEAT:
            assert(false && "No support for repeated mouse events");
            break;

        case GLFW_RELEASE:
            frame().mouse_buttons[button] = Input::ButtonState::up;
            break;

        default:
            break;
    }
}

void Window::mouse_move_event(double xoffset, double yoffset)
{

    frame().mouse_position = math::vec2d(xoffset, yoffset);

    frame().mouse_change_in_position = frame().mouse_position_previous - frame().mouse_position;
    frame().mouse_change_in_position.x *= -1.0; // coordinates are reversed on y axis (top left vs bottom left)

    frame().mouse_position_previous = frame().mouse_position;
}

void Window::mouse_scroll_event(double xoffset, double yoffset)
{
    frame().mouse_scroll = math::vec2d(xoffset, yoffset);
}
void Window::error_handler(int error, const char* description)
{
    spdlog::error("GLFWwindow Error: Code {} - Description {}", error, description);
}

void Window::keyboard_handler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->key_event(key, scancode, action, mods);
    // ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

void Window::char_input_handler([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] uint32_t codePoint)
{
    // ImGui_ImplGlfw_CharCallback(window, codePoint);
}

void Window::mouse_button_handler(GLFWwindow* window, int button, int action, int mods)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->mouse_button_event(button, action, mods);
    // ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void Window::mouse_move_handler(GLFWwindow* window, double pos_x, double pos_y)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->mouse_move_event(pos_x, pos_y);
}

void Window::mouse_scroll_handler(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->mouse_scroll_event(xoffset, yoffset);
    // ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void Window::window_close_handler(GLFWwindow* window)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->set_close();
}

void Window::framebuffer_size_handler(GLFWwindow* window, [[maybe_unused]] int width, [[maybe_unused]] int height)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->state.update_window_size = true;
}

void Window::window_resize_handler(GLFWwindow* window, int width, int height)
{
    if (width == 0 || height == 0) return;

    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    // glfwSetWindowSize(window, width, height);
    w->state.update_window_size = true;
}

void Window::window_iconify_handler(GLFWwindow* window, int iconified)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->state._is_iconified = iconified == 1;
}

void Window::window_focus_handler(GLFWwindow* window, int focused)
{
    Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    w->state._is_focused = focused == 1;
}

Window::InputFrame& Window::frame() { return state._frame[state.current_frame]; };
Window::InputFrame const& Window::frame() const { return state._frame[state.current_frame]; };
