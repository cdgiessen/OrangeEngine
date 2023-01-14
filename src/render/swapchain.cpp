#include "swapchain.h"

#include <assert.h>

#if !defined(NDEBUG)
#define VKB_CHECK(x)                                                                               \
    do                                                                                             \
    {                                                                                              \
        VkResult result = x;                                                                       \
        assert(result == VK_SUCCESS);                                                              \
    } while (0)

#else
#define VKB_CHECK(x) x
#endif


namespace vkb
{
namespace detail
{
struct SwapchainManagerErrorCategory : std::error_category
{
    const char* name() const noexcept override { return "vkb_swapchain_manager"; }
    std::string message(int err) const override
    {
        return to_string(static_cast<SwapchainManagerError>(err));
    }
};
const SwapchainManagerErrorCategory swapchain_manager_error_category;
} // namespace detail
std::error_code make_error_code(SwapchainManagerError swapchain_manager_error)
{
    return { static_cast<int>(swapchain_manager_error), detail::swapchain_manager_error_category };
}

#define CASE_TO_STRING(CATEGORY, TYPE)                                                             \
    case CATEGORY::TYPE:                                                                           \
        return #TYPE;
const char* to_string(SwapchainManagerError err)
{
    switch (err)
    {
        CASE_TO_STRING(SwapchainManagerError, success)
        CASE_TO_STRING(SwapchainManagerError, swapchain_suboptimal)
        CASE_TO_STRING(SwapchainManagerError, swapchain_out_of_date)
        CASE_TO_STRING(SwapchainManagerError, surface_lost)
        CASE_TO_STRING(SwapchainManagerError, must_call_acquire_image_first)
        CASE_TO_STRING(SwapchainManagerError, acquire_next_image_error)
        CASE_TO_STRING(SwapchainManagerError, queue_present_error)
        CASE_TO_STRING(SwapchainManagerError, surface_handle_not_provided)
        CASE_TO_STRING(SwapchainManagerError, failed_query_surface_support_details)
        CASE_TO_STRING(SwapchainManagerError, failed_create_swapchain)
        CASE_TO_STRING(SwapchainManagerError, failed_get_swapchain_images)
        CASE_TO_STRING(SwapchainManagerError, failed_create_swapchain_image_views)
        default:
            return "";
    }
}

// Imageless framebuffer builder

ImagelessFramebufferBuilder::ImagelessFramebufferBuilder(Device const& device) noexcept
: device(device.device)
{
}
VkFramebuffer ImagelessFramebufferBuilder::build() noexcept
{
    std::vector<VkFramebufferAttachmentImageInfo> attachment_infos;
    for (auto& attachment : _attachments)
    {
        VkFramebufferAttachmentImageInfo attach_image_info{};
        attach_image_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO;
        attach_image_info.usage = attachment.usage_flags;
        attach_image_info.width = _width;
        attach_image_info.height = _height;
        attach_image_info.layerCount = _layers;
        attach_image_info.viewFormatCount = static_cast<uint32_t>(attachment.formats.size());
        attach_image_info.pViewFormats = attachment.formats.data();
        attachment_infos.push_back(attach_image_info);
    }

    VkFramebufferAttachmentsCreateInfo attach_create_info{};
    attach_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENTS_CREATE_INFO;
    attach_create_info.attachmentImageInfoCount = static_cast<uint32_t>(attachment_infos.size());
    attach_create_info.pAttachmentImageInfos = attachment_infos.data();

    VkFramebufferCreateInfo framebuffer_info = {};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.pNext = &attach_create_info;
    framebuffer_info.flags = VK_FRAMEBUFFER_CREATE_IMAGELESS_BIT;
    framebuffer_info.renderPass = _render_pass;
    framebuffer_info.attachmentCount = static_cast<uint32_t>(attachment_infos.size());
    framebuffer_info.width = _width;
    framebuffer_info.height = _height;
    framebuffer_info.layers = _layers;

    VkFramebuffer framebuffer = VK_NULL_HANDLE;
    if (vkCreateFramebuffer(device, &framebuffer_info, nullptr, &framebuffer) != VK_SUCCESS)
    {
        assert(false && "TODO");
    }
    return framebuffer;
}
ImagelessFramebufferBuilder& ImagelessFramebufferBuilder::set_renderpass(VkRenderPass render_pass)
{
    _render_pass = render_pass;
    return *this;
}
ImagelessFramebufferBuilder& ImagelessFramebufferBuilder::set_width(uint32_t width)
{
    _width = width;
    return *this;
}
ImagelessFramebufferBuilder& ImagelessFramebufferBuilder::set_height(uint32_t height)
{
    _height = height;
    return *this;
}
ImagelessFramebufferBuilder& ImagelessFramebufferBuilder::set_extent(VkExtent2D extent)
{
    _width = extent.width;
    _height = extent.height;
    return *this;
}
ImagelessFramebufferBuilder& ImagelessFramebufferBuilder::set_layers(uint32_t layer_count)
{
    _layers = layer_count;
    return *this;
}
ImagelessFramebufferBuilder& ImagelessFramebufferBuilder::add_attachment(
    VkImageUsageFlags usage_flags, VkFormat format)
{
    _attachments.push_back(Attachment{ usage_flags, std::vector<VkFormat>(1, format) });
    return *this;
}
ImagelessFramebufferBuilder& ImagelessFramebufferBuilder::add_attachment(
    VkImageUsageFlags usage_flags, std::vector<VkFormat> const& formats)
{
    _attachments.push_back(Attachment{ usage_flags, formats });
    return *this;
}

DeletionQueue ::DeletionQueue(Device const& device, uint32_t deletion_delay) noexcept
: DeletionQueue(device.device, deletion_delay)
{
}

DeletionQueue::DeletionQueue(VkDevice device, uint32_t queue_depth) noexcept
: device(device), queue_depth(queue_depth)
{
    assert(queue_depth <= MAX_SWAPCHAIN_IMAGE_COUNT &&
           "queue_depth cannot exceed the max swapchain image count (8)");
    for (uint32_t i = 0; i < queue_depth; i++)
    {
        sets[i].images.reserve(10);
        sets[i].views.reserve(10);
        sets[i].framebuffers.reserve(10);
        sets[i].swapchains.reserve(1);
    }
}
DeletionQueue::~DeletionQueue() noexcept { destroy(); }

DeletionQueue::DeletionQueue(DeletionQueue&& other) noexcept
{
    device = other.device;
    queue_depth = other.queue_depth;
    current_index = other.current_index;
    sets = std::move(other.sets);
    other.device = VK_NULL_HANDLE;
}
DeletionQueue& DeletionQueue::operator=(DeletionQueue&& other) noexcept
{
    destroy();
    device = other.device;
    queue_depth = other.queue_depth;
    current_index = other.current_index;
    sets = std::move(other.sets);
    other.device = VK_NULL_HANDLE;
    return *this;
}
void DeletionQueue::add_image(VkImage image) noexcept
{
    sets[current_index].images.push_back(image);
}
void DeletionQueue::add_images(uint32_t images_count, const VkImage* images) noexcept
{
    for (size_t i = 0; i < images_count; i++)
    {
        sets[current_index].images.push_back(images[i]);
    }
}
void DeletionQueue::add_image_view(VkImageView image_view) noexcept
{
    sets[current_index].views.push_back(image_view);
}
void DeletionQueue::add_image_views(uint32_t image_view_count, const VkImageView* image_view) noexcept
{
    for (size_t i = 0; i < image_view_count; i++)
    {
        sets[current_index].views.push_back(image_view[i]);
    }
}
void DeletionQueue::add_framebuffer(VkFramebuffer framebuffer) noexcept
{
    sets[current_index].framebuffers.push_back(framebuffer);
}
void DeletionQueue::add_framebuffers(uint32_t framebuffer_count, const VkFramebuffer* framebuffers) noexcept
{
    for (size_t i = 0; i < framebuffer_count; i++)
    {
        sets[current_index].framebuffers.push_back(framebuffers[i]);
    }
}
void DeletionQueue::add_swapchain(VkSwapchainKHR swapchain) noexcept
{
    sets[current_index].swapchains.push_back(swapchain);
}
void DeletionQueue::clear_set(vkb::DeletionQueue::DelaySets& set) noexcept
{
    for (auto const& image : set.images)
        vkDestroyImage(device, image, nullptr);
    for (auto const& image_view : set.views)
        vkDestroyImageView(device, image_view, nullptr);
    for (auto const& framebuffer : set.framebuffers)
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    for (auto const& swapchain : set.swapchains)
        vkDestroySwapchainKHR(device, swapchain, nullptr);
    set.images.clear();
    set.views.clear();
    set.framebuffers.clear();
    set.swapchains.clear();
}
void DeletionQueue::tick() noexcept
{
    assert(queue_depth != 0 &&
           "Must construct DeletionQueue with valid VkDevice and non-zero queue_depth!");
    current_index = (current_index + 1) % queue_depth;
    clear_set(sets[current_index]);
}

void DeletionQueue::destroy() noexcept
{
    if (device != VK_NULL_HANDLE)
    {
        for (auto& set : sets)
        {
            clear_set(set);
        }
        device = VK_NULL_HANDLE;
    }
}

// Swapchain Synchronization resources (semaphores, fences, & tracking)
SemaphoreManager::SemaphoreManager(VkDevice device, uint32_t image_count) noexcept : device(device)
{
    detail.swapchain_image_count = image_count;

    VkSemaphoreCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    for (uint32_t i = 0; i < detail.swapchain_image_count; i++)
    {
        VKB_CHECK(vkCreateSemaphore(device, &info, nullptr, &detail.active_acquire_semaphores[i]));
        VKB_CHECK(vkCreateSemaphore(device, &info, nullptr, &detail.active_submit_semaphores[i]));
    }
    // Make sure there are some semaphores to use when needed
    detail.idle_semaphores.resize(10);
    for (auto& semaphore : detail.idle_semaphores)
    {
        VKB_CHECK(vkCreateSemaphore(device, &info, nullptr, &semaphore));
    }
    for (uint32_t i = 0; i < FRAMES_IN_FLIGHT; i++)
        detail.expired_semaphores[i].reserve(10);
    detail.current_acquire_semaphore = get_fresh_semaphore();
}
SemaphoreManager::~SemaphoreManager() noexcept { destroy(); }

void SemaphoreManager::destroy() noexcept
{
    if (device != VK_NULL_HANDLE)
    {
        for (uint32_t i = 0; i < detail.swapchain_image_count; i++)
        {
            vkDestroySemaphore(device, detail.active_acquire_semaphores[i], nullptr);
            vkDestroySemaphore(device, detail.active_submit_semaphores[i], nullptr);
        }
        for (auto& set : detail.expired_semaphores)
            for (auto& semaphore : set)
                vkDestroySemaphore(device, semaphore, nullptr);
        for (auto& semaphore : detail.idle_semaphores)
            vkDestroySemaphore(device, semaphore, nullptr);
        vkDestroySemaphore(device, detail.current_acquire_semaphore, nullptr);
    }
    device = VK_NULL_HANDLE;
}

SemaphoreManager::SemaphoreManager(SemaphoreManager&& other) noexcept
{
    device = other.device;
    detail = other.detail;
    other.device = VK_NULL_HANDLE; // destroy only does stuff if device isn't null, therefore its a sentinel
}
SemaphoreManager& SemaphoreManager::operator=(SemaphoreManager&& other) noexcept
{
    destroy();
    device = other.device;
    detail = other.detail;
    other.device = VK_NULL_HANDLE; // destroy only does stuff if device isn't null, therefore its a sentinel
    return *this;
}

VkSemaphore SemaphoreManager::get_next_acquire_semaphore() noexcept
{
    return detail.current_acquire_semaphore;
}
void SemaphoreManager::update_current_semaphore_index(uint32_t index) noexcept
{
    detail.current_swapchain_index = index;
    std::swap(detail.active_acquire_semaphores[index], detail.current_acquire_semaphore);
    detail.in_use[index] = true;
    detail.current_submit_index = (detail.current_submit_index + 1) % FRAMES_IN_FLIGHT;
    detail.idle_semaphores.insert(detail.idle_semaphores.end(),
        detail.expired_semaphores[detail.current_submit_index].begin(),
        detail.expired_semaphores[detail.current_submit_index].end());
    detail.expired_semaphores[detail.current_submit_index].clear();
}
VkSemaphore SemaphoreManager::get_acquire_semaphore() noexcept
{
    return detail.active_acquire_semaphores[detail.current_swapchain_index];
}
VkSemaphore SemaphoreManager::get_submit_semaphore() noexcept
{
    return detail.active_submit_semaphores[detail.current_swapchain_index];
}

void SemaphoreManager::recreate_swapchain_resources() noexcept
{
    for (uint32_t i = 0; i < detail.swapchain_image_count; i++)
    {
        if (detail.in_use[i])
        {
            detail.expired_semaphores[detail.current_submit_index].push_back(
                detail.active_acquire_semaphores[i]);
            detail.expired_semaphores[detail.current_submit_index].push_back(
                detail.active_submit_semaphores[i]);
            detail.active_acquire_semaphores[i] = get_fresh_semaphore();
            detail.active_submit_semaphores[i] = get_fresh_semaphore();
        }
        detail.in_use[i] = false;
    }
    detail.expired_semaphores[detail.current_submit_index].push_back(detail.current_acquire_semaphore);
    detail.current_acquire_semaphore = get_fresh_semaphore();
    detail.current_swapchain_index = INDEX_MAX_VALUE;
}

VkSemaphore SemaphoreManager::get_fresh_semaphore() noexcept
{
    VkSemaphore semaphore{};
    if (detail.idle_semaphores.size() > 1)
    {
        semaphore = detail.idle_semaphores.back();
        detail.idle_semaphores.pop_back();
    }
    else
    {
        VkSemaphoreCreateInfo info{};
        info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VKB_CHECK(vkCreateSemaphore(device, &info, nullptr, &semaphore));
    }
    return semaphore;
}

Result<vkb::SwapchainManager> convert(Result<vkb::Swapchain> err)
{
    switch (err.error().value())
    {
        case (static_cast<int>(SwapchainError::surface_handle_not_provided)):
            return { make_error_code(SwapchainManagerError::surface_handle_not_provided), err.vk_result() };
        case (static_cast<int>(SwapchainError::failed_query_surface_support_details)):
            return { make_error_code(SwapchainManagerError::failed_query_surface_support_details),
                err.vk_result() };
        case (static_cast<int>(SwapchainError::failed_create_swapchain)):
            return { make_error_code(SwapchainManagerError::failed_create_swapchain), err.vk_result() };
        case (static_cast<int>(SwapchainError::failed_get_swapchain_images)):
            return { make_error_code(SwapchainManagerError::failed_get_swapchain_images), err.vk_result() };
        case (static_cast<int>(SwapchainError::failed_create_swapchain_image_views)):
            return { make_error_code(SwapchainManagerError::failed_create_swapchain_image_views),
                err.vk_result() };
        default:
            assert(false && "Should never reach this");
            return { make_error_code(SwapchainManagerError::surface_handle_not_provided) };
    }
}

Result<SwapchainManager> SwapchainManager::create(Device const& device, SwapchainBuilder const& builder) noexcept
{
    auto swapchain_ret = builder.build();
    if (!swapchain_ret.has_value())
    {
        return convert(swapchain_ret);
    }

    auto swapchain_resources_ret = SwapchainManager::create_swapchain_resources(swapchain_ret.value());
    if (!swapchain_resources_ret.has_value())
    {
        return swapchain_resources_ret.error();
    }
    return SwapchainManager(device, builder, swapchain_ret.value(), swapchain_resources_ret.value());
}

SwapchainManager::SwapchainManager(
    Device const& device, SwapchainBuilder const& builder, vkb::Swapchain swapchain, SwapchainResources resources) noexcept
: device(device.device),
  detail({ builder,
      swapchain,
      resources,
      SemaphoreManager(device, swapchain.image_count),
      DeletionQueue(device, swapchain.image_count) })
{

    detail.graphics_queue = device.get_queue(QueueType::graphics).value();
    detail.present_queue = device.get_queue(QueueType::present).value();
    update_swapchain_info();
}

void SwapchainManager::destroy() noexcept
{
    if (device != VK_NULL_HANDLE)
    {
        detail.semaphore_manager = SemaphoreManager{};
        detail.delete_queue.add_swapchain(detail.swapchain_resources.swapchain);
        detail.delete_queue.add_image_views(
            detail.current_swapchain.image_count, &detail.swapchain_resources.image_views.front());
        detail.delete_queue.destroy();
        device = VK_NULL_HANDLE;
    }
}
SwapchainManager::~SwapchainManager() noexcept { destroy(); }

SwapchainManager::SwapchainManager(SwapchainManager&& other) noexcept
: device(other.device), detail(std::move(other.detail))
{
    other.device = VK_NULL_HANDLE;
}
SwapchainManager& SwapchainManager::operator=(SwapchainManager&& other) noexcept
{
    destroy();
    device = other.device;
    detail = std::move(other.detail);
    other.device = VK_NULL_HANDLE;
    return *this;
}

SwapchainBuilder& SwapchainManager::get_builder() noexcept
{
    assert(detail.current_status != Status::destroyed && "SwapchainManager was destroyed!");
    return detail.builder;
}
Result<SwapchainInfo> SwapchainManager::get_info() noexcept
{
    assert(detail.current_status != Status::destroyed && "SwapchainManager was destroyed!");
    if (detail.current_status == Status::expired)
    {
        return make_error_code(SwapchainManagerError::swapchain_out_of_date);
    }
    return detail.current_info;
}
Result<SwapchainResources> SwapchainManager::get_swapchain_resources() noexcept
{
    assert(detail.current_status != Status::destroyed && "SwapchainManager was destroyed!");
    if (detail.current_status == Status::expired)
    {
        return make_error_code(SwapchainManagerError::swapchain_out_of_date);
    }
    return detail.swapchain_resources;
}

Result<SwapchainAcquireInfo> SwapchainManager::acquire_image() noexcept
{
    assert(detail.current_status != Status::destroyed && "SwapchainManager was destroyed!");
    if (detail.current_status == Status::expired)
    {
        return make_error_code(SwapchainManagerError::swapchain_out_of_date);
    }
    else if (detail.current_status == Status::ready_to_present)
    {
        // dont do anything
        SwapchainAcquireInfo out{};
        out.image_view = detail.swapchain_resources.image_views[detail.current_image_index];
        out.image_index = detail.current_image_index;
        out.wait_semaphore = detail.semaphore_manager.get_acquire_semaphore();
        out.signal_semaphore = detail.semaphore_manager.get_submit_semaphore();
        return out;
    }

    // reset the current image index in case acquiring fails
    detail.current_image_index = INDEX_MAX_VALUE;

    VkSemaphore acquire_semaphore = detail.semaphore_manager.get_next_acquire_semaphore();

    VkResult result = vkAcquireNextImageKHR(device,
        detail.swapchain_resources.swapchain,
        UINT64_MAX,
        acquire_semaphore,
        VK_NULL_HANDLE,
        &detail.current_image_index);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        detail.current_status = Status::expired;
        return { make_error_code(SwapchainManagerError::swapchain_out_of_date) };
    }
    else if (result == VK_SUBOPTIMAL_KHR)
    {
    }
    else if (result == VK_ERROR_SURFACE_LOST_KHR)
    {
        return { make_error_code(SwapchainManagerError::surface_lost) };
    }
    else if (result != VK_SUCCESS)
    {
        return { make_error_code(SwapchainManagerError::acquire_next_image_error), result };
    }
    detail.semaphore_manager.update_current_semaphore_index(detail.current_image_index);
    detail.current_status = Status::ready_to_present;
    SwapchainAcquireInfo out{};
    out.image_view = detail.swapchain_resources.image_views[detail.current_image_index];
    out.image_index = detail.current_image_index;
    out.wait_semaphore = detail.semaphore_manager.get_acquire_semaphore();
    out.signal_semaphore = detail.semaphore_manager.get_submit_semaphore();
    return out;
}

Result<detail::E> SwapchainManager::present() noexcept
{
    assert(detail.current_status != Status::destroyed && "SwapchainManager was destroyed!");
    if (detail.current_status == Status::expired)
    {
        return make_error_code(SwapchainManagerError::swapchain_out_of_date);
    }
    if (detail.current_status == Status::ready_to_acquire)
    {
        return make_error_code(SwapchainManagerError::must_call_acquire_image_first);
    }

    VkSemaphore wait_semaphores[1] = { detail.semaphore_manager.get_submit_semaphore() };

    VkPresentInfoKHR present_info = {};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = wait_semaphores;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &detail.swapchain_resources.swapchain;
    present_info.pImageIndices = &detail.current_image_index;

    VkResult result = vkQueuePresentKHR(detail.present_queue, &present_info);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        detail.current_status = Status::expired;
        return { make_error_code(SwapchainManagerError::swapchain_out_of_date) };
    }
    else if (result == VK_SUBOPTIMAL_KHR)
    {
    }
    else if (result == VK_ERROR_SURFACE_LOST_KHR)
    {
        return { make_error_code(SwapchainManagerError::surface_lost) };
    }
    else if (result != VK_SUCCESS)
    {
        return { make_error_code(SwapchainManagerError::queue_present_error), result };
    }
    else
    {
        detail.current_status = Status::ready_to_acquire;
    }

    // clean up old swapchain resources
    detail.delete_queue.tick();

    return detail::E{};
}

void SwapchainManager::cancel_acquire_frame() noexcept
{
    WaitForSemaphore(detail.semaphore_manager.get_acquire_semaphore());
    detail.current_status = Status::ready_to_acquire;
}
void SwapchainManager::cancel_present_frame() noexcept
{
    WaitForSemaphore(detail.semaphore_manager.get_submit_semaphore());
    detail.current_status = Status::ready_to_acquire;
}

Result<SwapchainInfo> SwapchainManager::recreate(uint32_t width, uint32_t height) noexcept
{
    assert(detail.current_status != Status::destroyed && "SwapchainManager was destroyed!");

    auto old_resources = detail.swapchain_resources;
    detail.swapchain_resources = {};
    detail.delete_queue.add_swapchain(old_resources.swapchain);
    detail.delete_queue.add_image_views(old_resources.image_count, &old_resources.image_views.front());
    auto new_swapchain_ret =
        detail.builder.set_old_swapchain(old_resources.swapchain).set_desired_extent(width, height).build();
    if (!new_swapchain_ret)
    {
        return new_swapchain_ret.error();
    }
    detail.current_swapchain = new_swapchain_ret.value();
    auto new_resources_ret = SwapchainManager::create_swapchain_resources(detail.current_swapchain);
    if (!new_resources_ret)
    {
        return new_resources_ret.error();
    }
    detail.swapchain_resources = new_resources_ret.value();
    detail.semaphore_manager.recreate_swapchain_resources();
    update_swapchain_info();
    detail.current_status = Status::ready_to_acquire;
    return detail.current_info;
}

void SwapchainManager::update_swapchain_info() noexcept
{
    detail.current_info.image_count = detail.current_swapchain.image_count;
    detail.current_info.image_format = detail.current_swapchain.image_format;
    detail.current_info.extent = detail.current_swapchain.extent;
    detail.current_info.image_usage_flags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
}

Result<SwapchainResources> SwapchainManager::create_swapchain_resources(vkb::Swapchain swapchain) noexcept
{

    SwapchainResources out{};
    out.swapchain = swapchain.swapchain;
    out.image_count = swapchain.image_count;
    assert(out.image_count <= MAX_SWAPCHAIN_IMAGE_COUNT);

    VkResult result = vkGetSwapchainImagesKHR(swapchain.device, swapchain.swapchain, &out.image_count, nullptr);
    if (result != VK_SUCCESS)
        return { make_error_code(SwapchainManagerError::failed_get_swapchain_images), result };

    result = vkGetSwapchainImagesKHR(
        swapchain.device, swapchain.swapchain, &out.image_count, out.images.data());
    if (result != VK_SUCCESS)
        return { make_error_code(SwapchainManagerError::failed_get_swapchain_images), result };

#if defined(VK_VERSION_1_1)
    VkImageViewUsageCreateInfo desired_flags{};
    desired_flags.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_USAGE_CREATE_INFO;
    desired_flags.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
#endif
    for (size_t i = 0; i < out.image_count; i++)
    {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = out.images[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapchain.image_format;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
#if defined(VK_VERSION_1_1)
        createInfo.pNext = &desired_flags;
#endif
        result = vkCreateImageView(swapchain.device, &createInfo, nullptr, &out.image_views[i]);
        if (result != VK_SUCCESS)
        {
            return { make_error_code(SwapchainManagerError::failed_create_swapchain_image_views), result };
        }
    }
    return out;
}
void SwapchainManager::WaitForSemaphore(VkSemaphore semaphore) noexcept
{
    VkPipelineStageFlags wait_stages[1] = { VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT };

    VkSubmitInfo submit_info = {};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &semaphore;
    submit_info.pWaitDstStageMask = wait_stages;

    VKB_CHECK(vkQueueSubmit(detail.graphics_queue, 1, &submit_info, VK_NULL_HANDLE));
}

} // namespace vkb
