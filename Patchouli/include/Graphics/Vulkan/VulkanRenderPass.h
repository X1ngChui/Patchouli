#pragma once

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	struct VulkanSubpassCreateInfo
	{
		std::vector<VkAttachmentReference> colorAttachmentRefs;
		std::vector<VkAttachmentReference> inputAttachmentRefs;
		VkAttachmentReference depthStencilAttachmentRef;
	};

	class VulkanSubpass
	{
	public:
		VulkanSubpass(const VulkanSubpassCreateInfo& info);
		~VulkanSubpass() = default;
		
		VkSubpassDescription getDescription() const { return vkSubpassDescription; }

	private:
		VkSubpassDescription vkSubpassDescription;
		VulkanSubpassCreateInfo info;
	};

	struct VulkanRenderPassCreateInfo
	{
		std::vector<VulkanSubpass> subpasses;
		std::vector<VkSubpassDependency> subpassDependencies;
		std::vector<VkAttachmentDescription> attachmentDescriptions;
	};

	class VulkanRenderPass : public RefBase<VulkanRenderPass>
	{
	public:
		VulkanRenderPass(const VulkanRenderPassCreateInfo& info, Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator);
		virtual ~VulkanRenderPass();

		inline operator VkRenderPass() const { return vkRenderPass; }
	private:
		VkRenderPass vkRenderPass = VK_NULL_HANDLE;

		Ref<VulkanDevice> device;
		Ref<VulkanAllocator> allocator;

		VulkanRenderPassCreateInfo info;
	};
}