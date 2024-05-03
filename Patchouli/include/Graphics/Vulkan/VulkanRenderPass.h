#pragma once

#include "Core/PObject.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanSubpass
	{
	public:
		VulkanSubpass() = default;
		~VulkanSubpass() = default;

		void addColorAttachmentReference(const VkAttachmentReference& ref);
		void addInputAttachmentReference(const VkAttachmentReference& ref);
		void setDepthStencilAttachmentReference(const VkAttachmentReference& ref);
		void build();
		VkSubpassDescription getDescription() const { return vkSubpassDescription; }
	private:
		VkSubpassDescription vkSubpassDescription;
		VulkanVector<VkAttachmentReference> colorAttachmentRefs;
		VulkanVector<VkAttachmentReference> inputAttachmentRefs;
		VkAttachmentReference depthStencilAttachmentRef;
	};

	class VulkanRenderPass : public PObject
	{
	public:
		VulkanRenderPass(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator);
		virtual ~VulkanRenderPass();

		void addSubpass(const VulkanSubpass& subpass);
		void addDependency(const VkSubpassDependency& dependency);
		void addAttachmentDescripion(const VkAttachmentDescription& description);
		void build();

	private:
		VkRenderPass vkRenderPass = VK_NULL_HANDLE;

		VulkanVector<VulkanSubpass> subpasses;
		VulkanVector<VkSubpassDependency> subpassDependencies;
		VulkanVector<VkAttachmentDescription> attachmentDescriptions;

		Ref<VulkanDevice> device;
		Ref<VulkanAllocator> allocator;
	};
}