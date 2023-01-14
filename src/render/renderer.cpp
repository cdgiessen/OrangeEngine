#include "renderer.h"

#include "GLFW/glfw3.h"
#include "vuk/Context.hpp"
#include <spdlog/spdlog.h>
#include <string>
using namespace std::string_literals;

VKAPI_ATTR VkBool32 VKAPI_CALL vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
    [[maybe_unused]] void* user_data_ptr)
{
    auto ms = vkb::to_string_message_severity(message_severity);
    auto mt = vkb::to_string_message_type(message_type);

    switch (message_severity)
    {
        case (VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT):
            spdlog::debug("[{}: {}]\n{}\n", ms, mt, callback_data_ptr->pMessage);
            break;
        case (VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT):
            spdlog::info("[{}: {}]\n{}\n", ms, mt, callback_data_ptr->pMessage);
            break;
        case (VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT):
            spdlog::warn("[{}: {}]\n{}\n", ms, mt, callback_data_ptr->pMessage);
            break;
        case (VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT):
            spdlog::error("[{}: {}]\n{}\n", ms, mt, callback_data_ptr->pMessage);
            break;
    }
    return VK_FALSE;
}

Renderer::Renderer(Renderer::CreateDetails create_details)
{
    vkb::InstanceBuilder inst_builder;

    auto inst_ret = inst_builder.set_app_name(create_details.app_name)
                        .set_engine_name(create_details.engine_name)
                        .enable_validation_layers(create_details.enable_validation)
                        .require_api_version(1, 2)
                        .set_debug_callback(vulkan_debug_callback)
                        .build();
    if (!inst_ret)
    {
        throw std::runtime_error(
            "Failed to create renderer: InstanceBuilder failed with "s + inst_ret.error().message());
    };
    instance = inst_ret.value();

    auto surface_ret = glfwCreateWindowSurface(instance, create_details.window->handle(), nullptr, &surface);
    if (surface_ret != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create renderer: Failed to create VkSurfaceKHR");
    }

    vkb::PhysicalDeviceSelector phys_device_selector{ instance };
    phys_device_selector.set_surface(surface);
    auto phys_dev_ret = phys_device_selector.defer_surface_initialization().select();
    if (!phys_dev_ret)
    {
        throw std::runtime_error("Failed to create renderer: PhysicalDeviceSelector failed with "s +
                                 phys_dev_ret.error().message());
    }
    physical_device = phys_dev_ret.value();

    vkb::DeviceBuilder device_builder{ physical_device };
    auto dev_ret = device_builder.build();
    if (!dev_ret)
    {
        throw std::runtime_error(
            "Failed to create renderer: DeviceBuilder failed with "s + dev_ret.error().message());
    }
    device = dev_ret.value();

    auto graphics_queue_ret = device.get_queue(vkb::QueueType::graphics);
    if (!graphics_queue_ret)
        throw std::runtime_error("Failed to create renderer: Getting graphics queue failed with "s +
                                 graphics_queue_ret.error().message());
    // if we have a queue, we know we have the index
    graphics_queue = graphics_queue_ret.value();
    graphics_queue_index = device.get_queue_index(vkb::QueueType::graphics).value();

    delete_queue = vkb::DeletionQueue(device, frames_in_flight + 3);

    vkb::SwapchainBuilder swapchain_builder(device, surface);

    auto swapchain_ret = swapchain_builder.build();
    if (!swapchain_ret.has_value())
    {
        throw std::runtime_error("Failed to create renderer: Failed to create swapchain builder with "s +
                                 swapchain_ret.error().message());
    }

    auto swapchain_resources_ret = vkb::SwapchainManager::create_swapchain_resources(swapchain_ret.value());
    if (!swapchain_resources_ret.has_value())
    {
        throw std::runtime_error("Failed to create renderer: Failed to create swapchain manager with "s +
                                 swapchain_ret.error().message());
    }
    swapchain_manager = std::make_unique<vkb::SwapchainManager>(
        device, swapchain_builder, swapchain_ret.value(), swapchain_resources_ret.value());

    for (uint32_t i = 0; i < frames_in_flight; i++)
    {
        auto& frame = per_frame_resources[i];
        VkCommandPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.queueFamilyIndex = graphics_queue_index;
        auto pool_res = vkCreateCommandPool(device, &pool_info, nullptr, &frame.command_pool);
        if (pool_res != VK_SUCCESS) throw std::runtime_error("Failed to create command pool");

        VkCommandBufferAllocateInfo cmd_buf_alloc_info{};
        cmd_buf_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmd_buf_alloc_info.commandPool = frame.command_pool;
        cmd_buf_alloc_info.commandBufferCount = 1;
        auto cmd_alloc_res = vkAllocateCommandBuffers(device, &cmd_buf_alloc_info, &frame.command_buffer);
        if (cmd_alloc_res != VK_SUCCESS)
            throw std::runtime_error("Failed to allocate command buffer");

        VkFenceCreateInfo fence_info{};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        auto fence_ret = vkCreateFence(device, &fence_info, nullptr, &frame.fence);
        if (fence_ret != VK_SUCCESS) throw std::runtime_error("Failed to create fence");
    }
}

Renderer::~Renderer() noexcept
{
    vkQueueWaitIdle(graphics_queue);
    delete_queue.destroy();
    swapchain_manager->destroy();

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkb::destroy_device(device);
    vkb::destroy_instance(instance);
}
void Renderer::update() {}
void Renderer::draw()
{
    vkb::SwapchainAcquireInfo acquire_info;
    auto acquire_ret = swapchain_manager->acquire_image();
    if (!acquire_ret && acquire_ret.error().value() ==
                            static_cast<int>(vkb::SwapchainManagerError::swapchain_out_of_date))
    {
        return;
    }
    else if (!acquire_ret.has_value())
    {
        spdlog::error("failed to acquire swapchain image");
        return;
    }

    acquire_info = acquire_ret.value();

    vkWaitForFences(device, 1, &per_frame_resources[current_index].fence, VK_TRUE, UINT64_MAX);

    // record_command_buffer(renderer, command_buffers[current_index], acquire_info.image_view);

    VkSemaphore wait_semaphores[1] = { acquire_info.wait_semaphore };
    VkSemaphore signal_semaphores[1] = { acquire_info.signal_semaphore };
    VkPipelineStageFlags wait_stages[1] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &per_frame_resources[current_index].command_buffer;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    // only reset if we are going to submit, this prevents the issue where we reset the fence but try to wait on it again
    vkResetFences(device, 1, &per_frame_resources[current_index].fence);
    if (vkQueueSubmit(graphics_queue, 1, &submit_info, per_frame_resources[current_index].fence) != VK_SUCCESS)
    {
        spdlog::error("failed to submit command buffer");
        return;
    }
    current_index = (current_index + 1) % frames_in_flight;

    // No need to cancel, if a resize has started, then present will bail
    auto present_ret = swapchain_manager->present();
    if (!present_ret && present_ret.error().value() ==
                            static_cast<int>(vkb::SwapchainManagerError::swapchain_out_of_date))
    {
        return;
    }
    else if (!present_ret)
    {
        spdlog::error("failed to present swapchain image");
        return;
    }
    delete_queue.tick();
    return;
}
