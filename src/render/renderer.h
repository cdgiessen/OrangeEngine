#pragma once

#include <array>

#include "tl/optional.hpp"
#include "VkBootstrap.h"
#include "core/glfw.h"
#include "swapchain.h"
#include "vuk/vuk_fwd.hpp"

class Renderer
{
    public:
    struct CreateDetails
    {
        const char* app_name;
        const char* engine_name;
        bool enable_validation = true;
        Window* window;
    };

    Renderer(CreateDetails create_details);
    ~Renderer() noexcept;
    Renderer(Renderer const&) = delete;
    Renderer& operator=(Renderer const&) = delete;
    Renderer(Renderer&& other) noexcept;
    Renderer& operator=(Renderer&& other) noexcept;

    void update();

    void draw();


    private:
    vkb::Instance instance;
    VkSurfaceKHR surface;
    vkb::PhysicalDevice physical_device;
    vkb::Device device;
    uint32_t graphics_queue_index{};
    VkQueue graphics_queue{};

    std::unique_ptr<vkb::SwapchainManager> swapchain_manager;
    vkb::SwapchainInfo swap_info;
    vkb::DeletionQueue delete_queue;

    std::optional<vuk::Context> context;

    static const int frames_in_flight = 2;
    uint32_t current_index = 0;
    struct PerFrame
    {
        VkCommandPool command_pool;
        VkCommandBuffer command_buffer;
        VkFence fence;
    };

    std::array<PerFrame, frames_in_flight> per_frame_resources;
};
