#pragma once

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include "Graphics/Vulkan/VulkanRenderPass.h"
#include <Vulkan/Vulkan.h>

namespace Patchouli
{
	// Class representing a Vulkan Swapchain
	class VulkanSwapchain : public RefBase<VulkanSwapchain>
	{
	public:
		// Constructor to create a Vulkan Swapchain
		VulkanSwapchain(const GraphicsContextCreateInfo& graphicsInfo, Ref<VulkanRenderPass> renderPass, Ref<VulkanDevice> device, Ref<VulkanSurface> surface, Ref<VulkanAllocator> allocator);

		// Destructor to destroy a Vulkan Swapchain
		virtual ~VulkanSwapchain();

		void createFramebuffers(Ref<VulkanRenderPass> renderPass);
		VkFormat getFormat() const { return vkFormat; }
		std::size_t getImageCount() const { return vkImages.size(); }
		VkFramebuffer getFrameBuffer(std::size_t index) const { return vkFramebuffers[index]; }
		VkExtent2D getExtent() const { return vkExtent; }

		operator VkSwapchainKHR() const { return vkSwapchain; }

	private:
		// Friend declaration to allow access to struct VulkanSwapchainSupports
		template <GraphicsPolicy>
		friend struct VulkanSwapchainSettingsSelector;

		// Structure to hold information about Vulkan Swapchain supports
		struct VulkanSwapchainSupports
		{
			VkSurfaceCapabilitiesKHR surfaceCapabilities; // Surface capabilities
			std::vector<VkSurfaceFormatKHR> surfaceFormats; // Surface formats
			std::vector<VkPresentModeKHR> presentModes; // Present modes
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

		std::vector<VkImage> vkImages; // Vulkan images
		std::vector<VkImageView> vkImageViews; // Vulkan image views
		std::vector<VkFramebuffer> vkFramebuffers; // Vulkan frame buffers
	};
}
