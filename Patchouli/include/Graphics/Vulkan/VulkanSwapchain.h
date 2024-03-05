#pragma once

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanSurface.h"
#include <Vulkan/Vulkan.h>

namespace Patchouli
{
	class VulkanSwapchain : public GraphicsObject
	{
	public:
		VulkanSwapchain(Ref<VulkanDevice> device, Ref<VulkanSurface> surface, Ref<VulkanAllocator> allocator);
		virtual ~VulkanSwapchain();
	private:
		struct SwapchainSupports
		{
			VkSurfaceCapabilitiesKHR surfaceCapabilities;
			std::vector<VkSurfaceFormatKHR> surfaceFormats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		SwapchainSupports getSwapchainSupports() const;

		VkSurfaceFormatKHR selectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const;

		VkPresentModeKHR selectSurfacePresentMode(const std::vector<VkPresentModeKHR>& presentModes) const;

		VkExtent2D getExtent(const VkSurfaceCapabilitiesKHR& capabilities) const;
	private:
		VkSwapchainKHR vkSwapchain;
		Ref<VulkanDevice> vkDevice = nullptr;
		Ref<VulkanSurface> vkSurface = nullptr;
		Ref<VulkanAllocator> vkAllocator = nullptr;

		VkFormat vkFormat;
		VkExtent2D vkExtent;

		std::vector<VkImage> vkImages;
		std::vector<VkImageView> vkImageVies;
	};
}