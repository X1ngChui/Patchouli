#pragma once

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include <Vulkan/Vulkan.h>

namespace Patchouli
{
	// Class representing a Vulkan Swapchain
	class VulkanSwapchain : public GraphicsObject
	{
	public:
		// Constructor to create a Vulkan Swapchain
		VulkanSwapchain(const GraphicsInfo& graphicsInfo, Ref<VulkanDevice> device, Ref<VulkanSurface> surface, Ref<VulkanAllocator> allocator);

		// Destructor to destroy a Vulkan Swapchain
		virtual ~VulkanSwapchain();

	private:
		// Friend declaration to allow access to struct VulkanSwapchainSupports
		template <GraphicsPolicy>
		friend struct VulkanSwapchainSettingsSelect;

		// Structure to hold information about Vulkan Swapchain supports
		struct VulkanSwapchainSupports
		{
			VkSurfaceCapabilitiesKHR surfaceCapabilities; // Surface capabilities
			std::vector<VkSurfaceFormatKHR> surfaceFormats; // Surface formats
			std::vector<VkPresentModeKHR> presentModes; // Present modes
		};

		// Function to retrieve Vulkan Swapchain supports
		VulkanSwapchainSupports getSwapchainSupports() const;

	private:
		VkSwapchainKHR vkSwapchain; // Vulkan Swapchain handle
		Ref<VulkanDevice> vkDevice = nullptr; // Vulkan device
		Ref<VulkanSurface> vkSurface = nullptr; // Vulkan surface
		Ref<VulkanAllocator> vkAllocator = nullptr; // Vulkan allocator

		VkFormat vkFormat; // Vulkan image format
		VkExtent2D vkExtent; // Vulkan image extent

		std::vector<VkImage> vkImages; // Vulkan images
		std::vector<VkImageView> vkImageViews; // Vulkan image views
	};
}
