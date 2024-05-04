#pragma once

#include "Core/PObject.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	struct VulkanSubpassCreateInfo
	{
		VulkanVector<VkAttachmentReference> colorAttachmentRefs;
		VulkanVector<VkAttachmentReference> inputAttachmentRefs;
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
		VulkanVector<VulkanSubpass> subpasses;
		VulkanVector<VkSubpassDependency> subpassDependencies;
		VulkanVector<VkAttachmentDescription> attachmentDescriptions;
	};

	class VulkanRenderPass : public PObject
	{
	public:
		VulkanRenderPass(const VulkanRenderPassCreateInfo& info, Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator);
		virtual ~VulkanRenderPass();

		inline operator VkRenderPass() const { return vkRenderPass; }
	private:
		VkRenderPass vkRenderPass = VK_NULL_HANDLE;

		Ref<VulkanRenderPass> renderPass;
		Ref<VulkanDevice> device;
		Ref<VulkanAllocator> allocator;

		VulkanRenderPassCreateInfo info;
	};
}