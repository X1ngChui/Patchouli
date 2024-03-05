#include "Graphics/Vulkan/VulkanSwapchain.h"
#include "Core/Application.h"
#include <GLFW/glfw3.h>

namespace Patchouli
{
	// Structure to hold Vulkan Swapchain settings
	struct VulkanSwapchainSettings
	{
		VkSurfaceFormatKHR surfaceFormat;
		VkPresentModeKHR presentMode;
		VkExtent2D extent;
	};

	// Template struct to select Vulkan Swapchain surface format
	template <GraphicsPolicy P>
	struct VulkanSwapchainSurfaceFormatSelect
	{
		constexpr VulkanSwapchainSurfaceFormatSelect() = default;

		VkSurfaceFormatKHR operator()(const std::vector<VkSurfaceFormatKHR>& formats);
	};

	// Template struct to select Vulkan Swapchain present mode
	template <GraphicsPolicy P>
	struct VulkanSwapchainPresentModeSelect
	{
		constexpr VulkanSwapchainPresentModeSelect() = default;

		VkPresentModeKHR operator()(const std::vector<VkPresentModeKHR>& presentModes);
	};

	// Template struct to select Vulkan Swapchain extent
	template <GraphicsPolicy P>
	struct VulkanSwapchainExtentSelect
	{
		constexpr VulkanSwapchainExtentSelect() = default;

		VkExtent2D operator()(const VkSurfaceCapabilitiesKHR& capabilities);
	};

	// Template struct to select Vulkan Swapchain settings
	template <GraphicsPolicy P>
	struct VulkanSwapchainSettingsSelect
	{
		constexpr VulkanSwapchainSettingsSelect() = default;

		VulkanSwapchainSettings operator()(const VulkanSwapchain::VulkanSwapchainSupports& supports)
		{
			VulkanSwapchainSurfaceFormatSelect<P> surfaceFormatSelect;
			VulkanSwapchainPresentModeSelect<P> presentModeSelect;
			VulkanSwapchainExtentSelect<P> extentSelect;

			return {
				.surfaceFormat = surfaceFormatSelect(supports.surfaceFormats),
				.presentMode = presentModeSelect(supports.presentModes),
				.extent = extentSelect(supports.surfaceCapabilities)
			};
		}
	};

	// Template specialization for Vulkan Swapchain surface format selection
	template<GraphicsPolicy P>
	VkSurfaceFormatKHR VulkanSwapchainSurfaceFormatSelect<P>::operator()
		(const std::vector<VkSurfaceFormatKHR>& formats)
	{
		assert(formats.size() > 0);
		constexpr VkSurfaceFormatKHR defaultFormat = { VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

		if (formats.size() <= 1 && formats[0].format == VK_FORMAT_UNDEFINED)
			return defaultFormat;

		auto it = std::ranges::find_if(formats, [&](const auto& format) {
			return format.colorSpace == defaultFormat.colorSpace && format.format == defaultFormat.format;
		});
		if (it != formats.end())
			return *it;

		return formats[0];
	}

	// Template specialization for Vulkan Swapchain present mode selection with performance priority
	template<>
	VkPresentModeKHR VulkanSwapchainPresentModeSelect<GraphicsPolicy::PerformancePriority>::operator()
		(const std::vector<VkPresentModeKHR>& presentModes)
	{
		auto it = std::ranges::find(presentModes, VK_PRESENT_MODE_MAILBOX_KHR);
		if (it != presentModes.end())
			return *it;

		assert(std::ranges::find(presentModes, VK_PRESENT_MODE_FIFO_KHR) != presentModes.end());
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	// Template specialization for Vulkan Swapchain present mode selection with power-saving priority
	template<>
	VkPresentModeKHR VulkanSwapchainPresentModeSelect<GraphicsPolicy::PowerSavingPriority>::operator()
		(const std::vector<VkPresentModeKHR>& presentModes)
	{
		assert(std::ranges::find(presentModes, VK_PRESENT_MODE_FIFO_KHR) != presentModes.end());
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	// Template method for selecting Vulkan Swapchain extent
	template<GraphicsPolicy P>
	VkExtent2D VulkanSwapchainExtentSelect<P>::operator()(const VkSurfaceCapabilitiesKHR& capabilities)
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

	// Constructor for Vulkan Swapchain class
	VulkanSwapchain::VulkanSwapchain(const GraphicsInfo& graphicsInfo, Ref<VulkanDevice> device,
		Ref<VulkanSurface> surface, Ref<VulkanAllocator> allocator)
		: vkDevice(device), vkSurface(surface), vkAllocator(allocator)
	{
		VulkanSwapchainSupports supports = getSwapchainSupports();
		VulkanSwapchainSettings settings;
		switch (graphicsInfo.graphicsPolicy)
		{
		case GraphicsPolicy::PerformancePriority:
			VulkanSwapchainSettingsSelect<GraphicsPolicy::PerformancePriority> performancePrioritySelect;
			settings = performancePrioritySelect(supports);
			break;
		case GraphicsPolicy::PowerSavingPriority:
			VulkanSwapchainSettingsSelect<GraphicsPolicy::PowerSavingPriority> powerSavingPrioritySelect;
			settings = powerSavingPrioritySelect(supports);
			break;
		}

		uint32_t nImages = supports.surfaceCapabilities.minImageCount + 1;
		if (supports.surfaceCapabilities.maxImageCount > 0 && nImages > supports.surfaceCapabilities.maxImageCount)
			nImages = supports.surfaceCapabilities.maxImageCount;

		std::vector<uint32_t> queueFamilies = {
			vkDevice->getQueueFamilies().graphics,
			vkDevice->getQueueFamilies().present
		};

		bool sharedQueue = vkDevice->getQueueFamilies().graphics == vkDevice->getQueueFamilies().present;

		VkSwapchainCreateInfoKHR info = {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.pNext = nullptr,
			.flags = 0,
			.surface = *vkSurface,
			.minImageCount = nImages,
			.imageFormat = settings.surfaceFormat.format,
			.imageColorSpace = settings.surfaceFormat.colorSpace,
			.imageExtent = settings.extent,
			.imageArrayLayers = 1,
			.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.imageSharingMode = sharedQueue ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
			.queueFamilyIndexCount = sharedQueue ? 0 : static_cast<uint32_t>(queueFamilies.size()),
			.pQueueFamilyIndices = sharedQueue ? nullptr : queueFamilies.data(),
			.preTransform = supports.surfaceCapabilities.currentTransform,
			.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.presentMode = settings.presentMode,
			.clipped = VK_TRUE
		};

		VkResult status = vkCreateSwapchainKHR(*vkDevice, &info, *vkAllocator, &vkSwapchain);
		assert(status == VK_SUCCESS);

		vkFormat = settings.surfaceFormat.format;
		vkExtent = settings.extent;
	}

	// Destructor for Vulkan Swapchain class
	VulkanSwapchain::~VulkanSwapchain()
	{
		vkDestroySwapchainKHR(*vkDevice, vkSwapchain, *vkAllocator);
	}

	// Function to retrieve Vulkan Swapchain supports
	VulkanSwapchain::VulkanSwapchainSupports VulkanSwapchain::getSwapchainSupports() const
	{
		VulkanSwapchainSupports supports;

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
}
