#include "Graphics/Vulkan/VulkanSwapchain.h"
#include "Core/Application.h"
#include <GLFW/glfw3.h>

namespace Patchouli
{
	VulkanSwapchain::VulkanSwapchain(Ref<VulkanDevice> device, Ref<VulkanSurface> surface,
		Ref<VulkanAllocator> allocator)
		: vkDevice(device), vkSurface(surface), vkAllocator(allocator)
	{
		SwapchainSupports supports = getSwapchainSupports();
		VkSurfaceFormatKHR surfaceFormat = selectSurfaceFormat(supports.surfaceFormats);
		VkPresentModeKHR presentMode = selectSurfacePresentMode(supports.presentModes);
		VkExtent2D extent = getExtent(supports.surfaceCapabilities);

		uint32_t nImages = supports.surfaceCapabilities.minImageCount + 1;
		if (supports.surfaceCapabilities.maxImageCount > 0 && nImages > supports.surfaceCapabilities.maxImageCount)
			nImages = supports.surfaceCapabilities.maxImageCount;

		std::vector<uint32_t> queueFamilies = {
			vkDevice->getQueueFamilies().graphics,
			vkDevice->getQueueFamilies().present
		};

		VkSwapchainCreateInfoKHR info = {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.pNext = nullptr,
			.flags = 0,
			.surface = *vkSurface,
			.minImageCount = nImages,
			.imageFormat = surfaceFormat.format,
			.imageColorSpace = surfaceFormat.colorSpace,
			.imageExtent = extent,
			.imageArrayLayers = 1, // TODO: 2 for VR device
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.imageSharingMode = vkDevice->getQueueFamilies().graphics == vkDevice->getQueueFamilies().present ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
			.queueFamilyIndexCount = vkDevice->getQueueFamilies().graphics == vkDevice->getQueueFamilies().present ? 0 : static_cast<uint32_t>(queueFamilies.size()),
			.pQueueFamilyIndices = vkDevice->getQueueFamilies().graphics == vkDevice->getQueueFamilies().present ? nullptr : queueFamilies.data(),
			.preTransform = supports.surfaceCapabilities.currentTransform,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = presentMode,
			.clipped = VK_TRUE
		};

		VkResult status = vkCreateSwapchainKHR(*vkDevice, &info, *vkAllocator, &vkSwapchain);
		assert(status == VK_SUCCESS);

		vkFormat = surfaceFormat.format;
		vkExtent = extent;
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		vkDestroySwapchainKHR(*vkDevice, vkSwapchain, *vkAllocator);
	}

	Patchouli::VulkanSwapchain::SwapchainSupports VulkanSwapchain::getSwapchainSupports() const
	{
		SwapchainSupports supports;

		// Get surface capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*vkDevice, *vkSurface, &supports.surfaceCapabilities);

		// Get surface format
		uint32_t nFormats = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(*vkDevice, *vkSurface, &nFormats, nullptr);
		supports.surfaceFormats.resize(nFormats);
		vkGetPhysicalDeviceSurfaceFormatsKHR(*vkDevice, *vkSurface, &nFormats, 
			supports.surfaceFormats.data());
		assert(nFormats > 0);

		// Get present modes
		uint32_t nPresentModes = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(*vkDevice, *vkSurface, &nPresentModes, nullptr);
		supports.presentModes.resize(nPresentModes);
		vkGetPhysicalDeviceSurfacePresentModesKHR(*vkDevice, *vkSurface, &nPresentModes,
			supports.presentModes.data());
		assert(nPresentModes > 0);

		return supports;
	}

	VkSurfaceFormatKHR VulkanSwapchain::selectSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats) const
	{
		assert(formats.size() > 0);
		
		constexpr VkSurfaceFormatKHR defaultFormat = { VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

		if (formats.size() <= 1 && formats[0].format == VK_FORMAT_UNDEFINED)
			return defaultFormat;

		for (const auto& format : formats)
		{
			if (format.colorSpace == defaultFormat.colorSpace && format.format == defaultFormat.format)
				return defaultFormat;
		}

		return formats[0];
	}

	VkPresentModeKHR VulkanSwapchain::selectSurfacePresentMode(const std::vector<VkPresentModeKHR>& presentModes) const
	{
		// TODO: Always FIFO mode for mobile device
		auto it = std::find(presentModes.begin(), presentModes.end(), VK_PRESENT_MODE_MAILBOX_KHR);
		if (it != presentModes.end())
			return *it;

		assert(std::find(presentModes.begin(), presentModes.end(), VK_PRESENT_MODE_FIFO_KHR) != presentModes.end());
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanSwapchain::getExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
	{
		if (capabilities.currentExtent.width != 0xffffffff)
			return capabilities.currentExtent;

		VkExtent2D extent;
		switch (Application::getInstance().getAppInfo().windowInfo.windowAPI)
		{
		case WindowAPI::GLFW:
			int width = 0, height = 0;
			glfwGetFramebufferSize(*(GLFWwindow**)Application::getInstance().getWindow()->getNative(),
				&width, &height);
			extent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		}

		// Constrain the extent within the image extent range
		assert(capabilities.minImageExtent.width <= capabilities.maxImageExtent.width);
		if (extent.width < capabilities.minImageExtent.width)
			extent.width = capabilities.minImageExtent.width;
		else if (extent.width > capabilities.maxImageExtent.width)
			extent.width = capabilities.maxImageExtent.width;

		assert(capabilities.minImageExtent.height <= capabilities.maxImageExtent.height);
		if (extent.height < capabilities.minImageExtent.height)
			extent.height = capabilities.minImageExtent.height;
		else if (extent.height > capabilities.maxImageExtent.height)
			extent.height = capabilities.maxImageExtent.height;

		return extent;
	}
}