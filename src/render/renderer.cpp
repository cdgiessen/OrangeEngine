#include "renderer.h"

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
                        .desire_api_version(1, 2)
                        .set_debug_callback(vulkan_debug_callback)
                        .build();
    if (!inst_ret)
    {
        throw std::runtime_error(
            "Failed to create renderer: InstanceBuilder failed with "s + inst_ret.error().message());
    };
    instance = inst_ret.value();

    vkb::PhysicalDeviceSelector phys_device_selector{ instance };
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
    uint32_t graphics_queue_index_ret = device.get_queue_index(vkb::QueueType::graphics).value();
}