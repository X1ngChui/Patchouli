#pragma once

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include "Graphics/Vulkan/VulkanRenderPass.h"
#include <Vulkan/Vulkan.h>

namespace Patchouli
{
	// Class representing a Vulkan Swapchain
	class VulkanSwapchain : public PObject
	{
	public:
		// Constructor to create a Vulkan Swapchain
		VulkanSwapchain(const GraphicsContextCreateInfo& graphicsInfo, Ref<VulkanRenderPass> renderPass, Ref<VulkanDevice> device, Ref<VulkanSurface> surface, Ref<VulkanAllocator> allocator);

		// Destructor to destroy a Vulkan Swapchain
		virtual ~VulkanSwapchain();

		void createFramebuffers(Ref<VulkanRenderPass> renderPass);
		VkFormat getFormat() const { return vkFormat; }

	private:
		// Friend declaration to allow access to struct VulkanSwapchainSupports
		template <GraphicsPolicy>
		friend struct VulkanSwapchainSettingsSelector;

		// Structure to hold information about Vulkan Swapchain supports
		struct VulkanSwapchainSupports
		{
			VkSurfaceCapabilitiesKHR surfaceCapabilities; // Surface capabilities
			VulkanVector<VkSurfaceFormatKHR> surfaceFormats; // Surface formats
			VulkanVector<VkPresentModeKHR> presentModes; // Present modes
		};

		// Function to retrieve Vulkan Swapchain supports
		VulkanSwapchainSupports getSwapchainSupports() const;

		// Function to create image view
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags,
			uint32_t mipLevels = 1);

	private:
		VkSwapchainKHR vkSwapchain; // Vulkan Swapchain handle
		Ref<VulkanDevice> device = nullptr; // Vulkan device
		Ref<VulkanSurface> surface = nullptr; // Vulkan surface
		Ref<VulkanAllocator> allocator = nullptr; // Vulkan allocator

		VkFormat vkFormat; // Vulkan image format
		VkExtent2D vkExtent; // Vulkan image extent

		VulkanVector<VkImage> vkImages; // Vulkan images
		VulkanVector<VkImageView> vkImageViews; // Vulkan image views
		VulkanVector<VkFramebuffer> vkFramebuffers; // Vulkan frame buffers
	};
}
