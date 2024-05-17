#include "Graphics/Vulkan/VulkanRenderPass.h"

namespace Patchouli
{
    VulkanSubpass::VulkanSubpass(const VulkanSubpassCreateInfo& info)
        : info(info)
    {
        vkSubpassDescription = {
            .flags = 0,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = static_cast<uint32_t>(info.inputAttachmentRefs.size()),
            .pInputAttachments = info.inputAttachmentRefs.data(),
            .colorAttachmentCount = static_cast<uint32_t>(info.colorAttachmentRefs.size()),
            .pColorAttachments = info.colorAttachmentRefs.data(),
            .pDepthStencilAttachment = (info.depthStencilAttachmentRef.layout == VK_IMAGE_LAYOUT_UNDEFINED
            || info.depthStencilAttachmentRef.layout == VK_IMAGE_LAYOUT_PREINITIALIZED
            || info.depthStencilAttachmentRef.layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) ?
            nullptr : &info.depthStencilAttachmentRef,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = nullptr
        };
    }

    VulkanRenderPass::VulkanRenderPass(const VulkanRenderPassCreateInfo& info, Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator)
        : info(info), device(device), allocator(allocator)
    {
        std::vector<VkSubpassDescription> subpassDescriptions;
        for (auto& subpass : info.subpasses)
            subpassDescriptions.push_back(subpass.getDescription());

        VkRenderPassCreateInfo renderPassCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .attachmentCount = static_cast<uint32_t>(info.attachmentDescriptions.size()),
            .pAttachments = info.attachmentDescriptions.data(),
            .subpassCount = static_cast<uint32_t>(subpassDescriptions.size()),
            .pSubpasses = subpassDescriptions.data(),
            .dependencyCount = static_cast<uint32_t>(info.subpassDependencies.size()),
            .pDependencies = info.subpassDependencies.data(),
        };

        VkResult result = vkCreateRenderPass(*device, &renderPassCreateInfo, *allocator, &vkRenderPass);
        assert(result == VK_SUCCESS);
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        vkDestroyRenderPass(*device, vkRenderPass, *allocator);
    }
}