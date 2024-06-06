#include "Graphics/Vulkan/VulkanContext.h"
#include <GLFW/glfw3.h>

namespace Patchouli
{
    // Constructor for VulkanContext.
    // It initializes the Vulkan allocator and instance based on the provided GraphicsInfo object.
    VulkanContext::VulkanContext(const GraphicsContextCreateInfo& info)
        : graphicsContextInfo(info)
    {
        // Initialize Vulkan allocator
        allocator = makeRef<VulkanAllocator>();

        // Create Vulkan instance using the provided GraphicsInfo object and Vulkan allocator
        instance = makeRef<VulkanInstance>(allocator, graphicsContextInfo);

#ifdef VULKAN_VALIDATION_LAYER
        // Create Vulkan debug messenger for validation purposes, if enabled
        debugMessenger = makeRef<VulkanDebugMessenger>(instance, allocator);
#endif
        // Create Vulkan surface for displaying
        surface = makeRef<VulkanSurface>(instance, allocator, graphicsContextInfo.window);

        // Select and create graphics Device
        auto devices = VulkanDevice::getDevices(instance);
        device = std::static_pointer_cast<VulkanDevice>(graphicsContextInfo.deviceSelector(devices.data(), devices.size()));
        device->onSelect(allocator, surface, graphicsContextInfo);

        swapchain = makeRef<VulkanSwapchain>(graphicsContextInfo, renderPass, device, surface, allocator);
        
        // Create Vulkan render pass
        VkAttachmentReference attachmentReference = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };
        VkSubpassDependency dependency = {
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,
            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .srcAccessMask = 0,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            .dependencyFlags = 0
        };
        VulkanSubpassCreateInfo subpassCreateInfo = {
            .colorAttachmentRefs = { attachmentReference }
        };
        VulkanSubpass subpass(subpassCreateInfo);

        VkAttachmentDescription attachmentDescription = {
           .flags = 0,
           .format = swapchain->getFormat(),
           .samples = VK_SAMPLE_COUNT_1_BIT,
           .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
           .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
           .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
           .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
           .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
           .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
        };
        VulkanRenderPassCreateInfo renderPassCreateInfo = {
            .subpasses = { subpass },
            .subpassDependencies = { dependency },
            .attachmentDescriptions = { attachmentDescription }
        };
        renderPass = makeRef<VulkanRenderPass>(renderPassCreateInfo, device, allocator);
        swapchain->createFramebuffers(renderPass);

        pipeline = makeRef<VulkanPipeline>(renderPass, device, allocator);

        commandPool = makeRef<VulkanCommandPool>(device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, allocator);
    }
}