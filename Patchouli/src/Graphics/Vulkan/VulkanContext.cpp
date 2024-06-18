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

#ifdef PATCHOULI_VULKAN_VALIDATION_LAYER
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

        for (std::size_t i = 0; i < swapchain->getImageCount(); i++)
        {
            auto commandBuffer = commandPool->createPrimaryCommandBuffer();

            commandBuffer->begin();
            VkClearValue clearColor = { 1.0f, 1.0f, 1.0f, 1.0f };
            VkRenderPassBeginInfo beginInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .pNext = nullptr,
                .renderPass = *renderPass,
                .framebuffer = swapchain->getFrameBuffer(i),
                .renderArea = {.offset = {0, 0}, .extent = swapchain->getExtent()},
                .clearValueCount = 1,
                .pClearValues = &clearColor
            };
            commandBuffer->beginRenderPass(beginInfo, VK_SUBPASS_CONTENTS_INLINE);
            commandBuffer->bindGraphicsPipeline(*pipeline);
            commandBuffer->draw(3);
            commandBuffer->endRenderPass();
            commandBuffer->end();

            commandBuffers.push_back(commandBuffer);
        }

        for (std::size_t i = 0; i < swapchain->getImageCount(); i++)
        {
            imageAvailableSemaphores.push_back(makeRef<VulkanSemaphore>(device, allocator));
            renderFinishedSemaphores.push_back(makeRef<VulkanSemaphore>(device, allocator));
            fences.push_back(makeRef<VulkanFence>(device, allocator, true));
        }
    }

    void VulkanContext::render()
    {
        VkResult result = VK_SUCCESS;

        fences[currentFrame]->wait();

        uint32_t imageIndex = 0;
        result = vkAcquireNextImageKHR(*device, *swapchain, std::numeric_limits<uint64_t>::max(), *imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
        assert(result == VK_SUCCESS);

        std::array<VkSemaphore, 1> waitSemaphores = { *imageAvailableSemaphores[currentFrame] };
        std::array<VkPipelineStageFlags, 1> waitStages = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        std::array<VkCommandBuffer, 1> usingCommandBuffers = { *commandBuffers[imageIndex] };
        std::array<VkSemaphore, 1> signalSemaphores = { *renderFinishedSemaphores[currentFrame] };

        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = (uint32_t)waitSemaphores.size(),
            .pWaitSemaphores = waitSemaphores.data(),
            .pWaitDstStageMask = waitStages.data(),
            .commandBufferCount = 1,
            .pCommandBuffers = usingCommandBuffers.data(),
            .signalSemaphoreCount = (uint32_t)signalSemaphores.size(),
            .pSignalSemaphores = signalSemaphores.data()
        };

        fences[currentFrame]->reset();
        result = vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, *fences[currentFrame]);
        assert(result == VK_SUCCESS);

        std::array<VkSwapchainKHR, 1> swapchains = { *swapchain };
        VkPresentInfoKHR presentInfo = {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = (uint32_t)signalSemaphores.size(),
            .pWaitSemaphores = signalSemaphores.data(),
            .swapchainCount = (uint32_t)swapchains.size(),
            .pSwapchains = swapchains.data(),
            .pImageIndices = &imageIndex
        };

        result = vkQueuePresentKHR(device->getPresentQueue(), &presentInfo);
        assert(result == VK_SUCCESS);

        currentFrame = (currentFrame + 1) % swapchain->getImageCount();
    }
}