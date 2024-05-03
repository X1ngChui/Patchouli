#include "Graphics/Vulkan/VulkanRenderPass.h"

namespace Patchouli
{
    void VulkanSubpass::addColorAttachmentReference(const VkAttachmentReference& ref)
    {
        colorAttachmentRefs.push_back(ref);
    }

    void VulkanSubpass::addInputAttachmentReference(const VkAttachmentReference& ref)
    {
        inputAttachmentRefs.push_back(ref);
    }

    void VulkanSubpass::setDepthStencilAttachmentReference(const VkAttachmentReference& ref)
    {
        depthStencilAttachmentRef = ref;
    }

    void VulkanSubpass::build()
    {
        assert(!colorAttachmentRefs.empty());
        vkSubpassDescription = {
            .flags = 0,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = static_cast<uint32_t>(inputAttachmentRefs.size()),
            .pInputAttachments = inputAttachmentRefs.data(),
            .colorAttachmentCount = static_cast<uint32_t>(colorAttachmentRefs.size()),
            .pColorAttachments = colorAttachmentRefs.data(),
            .pDepthStencilAttachment = &depthStencilAttachmentRef,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = nullptr
        };
    }

    VulkanRenderPass::VulkanRenderPass(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator)
        : device(device), allocator(allocator)
    {
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        if (vkRenderPass != VK_NULL_HANDLE)
            vkDestroyRenderPass(*device, vkRenderPass, *allocator);
    }

    void VulkanRenderPass::addSubpass(const VulkanSubpass& subpass)
    {
        subpasses.push_back(subpass);
    }

    void VulkanRenderPass::addDependency(const VkSubpassDependency& dependency)
    {
        subpassDependencies.push_back(dependency);
    }

    void VulkanRenderPass::addAttachmentDescripion(const VkAttachmentDescription& description)
    {
        attachmentDescriptions.push_back(description);
    }

    void VulkanRenderPass::build()
    {
        assert(!subpasses.empty() && !attachmentDescriptions.empty() && !subpassDependencies.empty());

        VulkanVector<VkSubpassDescription> subpassDescriptions;
        for (auto& subpass : subpasses)
            subpassDescriptions.push_back(subpass.getDescription());

        VkRenderPassCreateInfo renderPassCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size()),
            .pAttachments = attachmentDescriptions.data(),
            .subpassCount = static_cast<uint32_t>(subpassDescriptions.size()),
            .pSubpasses = subpassDescriptions.data(),
            .dependencyCount = static_cast<uint32_t>(subpassDependencies.size()),
            .pDependencies = subpassDependencies.data(),
        };

        VkResult result = vkCreateRenderPass(*device, &renderPassCreateInfo, *allocator, &vkRenderPass);
        assert(result == VK_SUCCESS);
    }
}