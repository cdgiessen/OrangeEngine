#include "core/glfw.h"
#include "spdlog/spdlog.h"

#include "render/renderer.h"
struct StaticInit
{
    StaticInit() { Window::static_initialization(); }
    ~StaticInit() { Window::static_shutdown(); }
};

int main()
{
    StaticInit static_init{};
    Window main_win{ Window::CreateDetails{
        .window_title = "TestWindow", .size = math::vec2i{ 800, 600 }, .position = math::vec2i{ 100, 100 } } };

    Renderer renderer{ Renderer::CreateDetails{
        .app_name = "Test", .enable_validation = true, .window = &main_win } };

    while (!main_win.should_close())
    {
        Window::poll_events();

        if (main_win.get_key(Input::KeyCode::ESCAPE))
        {
            main_win.set_close();
        }
        renderer.update();
        renderer.draw();

        main_win.next_frame();
    }
}