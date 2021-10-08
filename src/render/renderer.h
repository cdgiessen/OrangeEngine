#pragma once

#include <array>

#include "VkBootstrap.h"

class Renderer
{
    public:
    struct CreateDetails
    {
        const char* app_name = "";
        const char* engine_name = "";
        bool enable_validation = true;
    };

    Renderer(CreateDetails create_details);
    ~Renderer() noexcept;
    Renderer(Renderer const&) = delete;
    Renderer& operator=(Renderer const&) = delete;
    Renderer(Renderer&& other) noexcept;
    Renderer& operator=(Renderer&& other) noexcept;

    private:
    vkb::Instance instance;
    vkb::PhysicalDevice physical_device;
    vkb::Device device;

    uint32_t graphics_queue_index_ret;
    VkQueue graphics_queue{};


    struct PerFrame
    {
    };

    std::array<PerFrame, 2> per_frame_resources;
};
