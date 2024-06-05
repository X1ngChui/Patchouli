#include "Graphics/Vulkan/VulkanSwapchain.h"
#include <GLFW/glfw3.h>

namespace Patchouli
{
	// Structure to hold Vulkan Swapchain settings
	struct VulkanSwapchainSettings
	{
		VkSurfaceFormatKHR surfaceFormat; // Surface format for the swapchain images
		VkPresentModeKHR presentMode; // Present mode for displaying images
		VkExtent2D extent; // Extent (size) of the swapchain images
	};

	// Template struct to select Vulkan Swapchain surface format
	template <GraphicsPolicy P>
	struct VulkanSwapchainSurfaceFormatSelect
	{
		constexpr VulkanSwapchainSurfaceFormatSelect() = default;

		// Operator to select the surface format based on given formats
		VkSurfaceFormatKHR operator()(const std::vector<VkSurfaceFormatKHR>& formats) const;
	};

	// Template struct to select Vulkan Swapchain present mode
	template <GraphicsPolicy P>
	struct VulkanSwapchainPresentModeSelect
	{
		constexpr VulkanSwapchainPresentModeSelect() = default;

		// Operator to select the present mode based on given present modes
		VkPresentModeKHR operator()(const std::vector<VkPresentModeKHR>& presentModes) const;
	};

	// Template struct to select Vulkan Swapchain extent
	template <GraphicsPolicy P>
	struct VulkanSwapchainExtentSelect
	{
		constexpr VulkanSwapchainExtentSelect() = default;

		// Operator to select the extent based on given surface capabilities
		VkExtent2D operator()(const VkSurfaceCapabilitiesKHR& capabilities, const GraphicsContextCreateInfo& info) const;
	};

	// Template struct to select Vulkan Swapchain settings
	template <GraphicsPolicy P>
	struct VulkanSwapchainSettingsSelector
	{
		constexpr VulkanSwapchainSettingsSelector() = default;

		// Operator to select the swapchain settings based on the supported formats, present modes, and surface capabilities
		VulkanSwapchainSettings operator()(const VulkanSwapchain::VulkanSwapchainSupports& supports, const GraphicsContextCreateInfo& info) const
		{
			constexpr VulkanSwapchainSurfaceFormatSelect<P> surfaceFormatSelect{};
			constexpr VulkanSwapchainPresentModeSelect<P> presentModeSelect{};
			constexpr VulkanSwapchainExtentSelect<P> extentSelect{};

			return {
				.surfaceFormat = surfaceFormatSelect(supports.surfaceFormats),
				.presentMode = presentModeSelect(supports.presentModes),
				.extent = extentSelect(supports.surfaceCapabilities, info)
			};
		}
	};

	// Template specialization for Vulkan Swapchain surface format selection
	template<GraphicsPolicy P>
	VkSurfaceFormatKHR VulkanSwapchainSurfaceFormatSelect<P>::operator()
		(const std::vector<VkSurfaceFormatKHR>& formats) const
	{
		assert(formats.size() > 0);
		constexpr VkSurfaceFormatKHR defaultFormat = { VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

		// If only one format is available and it is undefined, return the default format
		if (formats.size() <= 1 && formats[0].format == VK_FORMAT_UNDEFINED)
			return defaultFormat;

		// Try to find the default format, otherwise return the first available format
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
		(const std::vector<VkPresentModeKHR>& presentModes) const
	{
		// Try to find the mailbox present mode, if not available, fallback to FIFO
		auto it = std::ranges::find(presentModes, VK_PRESENT_MODE_MAILBOX_KHR);
		if (it != presentModes.end())
			return *it;

		assert(std::ranges::find(presentModes, VK_PRESENT_MODE_FIFO_KHR) != presentModes.end());
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	// Template specialization for Vulkan Swapchain present mode selection with power-saving priority
	template<>
	VkPresentModeKHR VulkanSwapchainPresentModeSelect<GraphicsPolicy::PowerSavingPriority>::operator()
		(const std::vector<VkPresentModeKHR>& presentModes) const
	{
		// For power-saving priority, always use FIFO present mode
		assert(std::ranges::find(presentModes, VK_PRESENT_MODE_FIFO_KHR) != presentModes.end());
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	// Template method for selecting Vulkan Swapchain extent
	template<GraphicsPolicy P>
	VkExtent2D VulkanSwapchainExtentSelect<P>::operator()(const VkSurfaceCapabilitiesKHR& capabilities, const GraphicsContextCreateInfo& info) const
	{
		// If the current extent is defined, use it, otherwise, use the window framebuffer size
		if (capabilities.currentExtent.width != 0xffffffff)
			return capabilities.currentExtent;

		VkExtent2D extent;
		switch (info.window->getAPI())
		{
		case WindowAPI::GLFW:
			int width = 0, height = 0;
			glfwGetFramebufferSize(*(GLFWwindow**)info.window->getNativeData(),
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
	VulkanSwapchain::VulkanSwapchain(const GraphicsContextCreateInfo& graphicsInfo, Ref<VulkanRenderPass> renderPass, Ref<VulkanDevice> device,
		Ref<VulkanSurface> surface, Ref<VulkanAllocator> allocator)
		: device(device), surface(surface), allocator(allocator)
	{
		// Get swapchain supports and select appropriate settings based on the graphics policy
		VulkanSwapchainSupports supports = getSwapchainSupports();
		VulkanSwapchainSettings settings;
		switch (graphicsInfo.graphicsPolicy)
		{
		case GraphicsPolicy::PerformancePriority:
			constexpr VulkanSwapchainSettingsSelector<GraphicsPolicy::PerformancePriority> performancePrioritySelector;
			settings = performancePrioritySelector(supports, graphicsInfo);
			break;
		case GraphicsPolicy::PowerSavingPriority:
			constexpr VulkanSwapchainSettingsSelector<GraphicsPolicy::PowerSavingPriority> powerSavingPrioritySelector;
			settings = powerSavingPrioritySelector(supports, graphicsInfo);
			break;
		}

		// Determine the number of swapchain images
		uint32_t nImages = supports.surfaceCapabilities.minImageCount + 1;
		if (supports.surfaceCapabilities.maxImageCount > 0 && nImages > supports.surfaceCapabilities.maxImageCount)
			nImages = supports.surfaceCapabilities.maxImageCount;

		// Determine queue families for sharing or exclusive ownership
		std::vector<uint32_t> queueFamilies = {
			device->getQueueFamilies().graphics,
			device->getQueueFamilies().present
		};

		bool sharedQueue = device->getQueueFamilies().graphics == device->getQueueFamilies().present;

		// Create swapchain creation info
		VkSwapchainCreateInfoKHR info = {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.pNext = nullptr,
			.flags = 0,
			.surface = *surface,
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

		// Create the Vulkan swapchain
		VkResult status = vkCreateSwapchainKHR(*device, &info, *allocator, &vkSwapchain);
		assert(status == VK_SUCCESS);

		// Store swapchain format and extent
		vkFormat = settings.surfaceFormat.format;
		vkExtent = settings.extent;

		vkGetSwapchainImagesKHR(*device, vkSwapchain, &nImages, nullptr);
		vkImages.resize(nImages);
		vkGetSwapchainImagesKHR(*device, vkSwapchain, &nImages, vkImages.data());

		// Create image views
		vkImageViews.reserve(nImages);
		for (uint32_t i = 0; i < nImages; i++)
			vkImageViews.push_back(createImageView(vkImages[i], vkFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1));
	}

	// Destructor for Vulkan Swapchain class
	VulkanSwapchain::~VulkanSwapchain()
	{
		// Destory ecah Vulkan image view
		for (auto& imageView : vkImageViews)
			vkDestroyImageView(*device, imageView, *allocator);

		// Destroy each Vulkan framebuffer
		for (auto& framebuffer : vkFramebuffers)
			vkDestroyFramebuffer(*device, framebuffer, *allocator);

		// Destroy the Vulkan swapchain
		vkDestroySwapchainKHR(*device, vkSwapchain, *allocator);
	}

	void VulkanSwapchain::createFramebuffers(Ref<VulkanRenderPass> renderPass)
	{
		// Create frame buffers
		vkFramebuffers.resize(vkImageViews.size());
		for (uint32_t i = 0; i < vkImageViews.size(); i++)
		{
			std::array<VkImageView, 1> attachments = { vkImageViews[i] };

			VkFramebufferCreateInfo framebufferCreateInfo = {
				.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.renderPass = *renderPass,
				.attachmentCount = static_cast<uint32_t>(attachments.size()),
				.pAttachments = attachments.data(),
				.width = vkExtent.width,
				.height = vkExtent.height,
				.layers = 1
			};

			VkResult result = vkCreateFramebuffer(*device, &framebufferCreateInfo, *allocator, &vkFramebuffers[i]);
			assert(result == VK_SUCCESS);
		}
	}

	// Function to retrieve Vulkan Swapchain supports
	VulkanSwapchain::VulkanSwapchainSupports VulkanSwapchain::getSwapchainSupports() const
	{
		VulkanSwapchainSupports supports;

		// Get surface capabilities
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*device, *surface, &supports.surfaceCapabilities);

		// Get surface format
		uint32_t nFormats = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &nFormats, nullptr);
		supports.surfaceFormats.resize(nFormats);
		vkGetPhysicalDeviceSurfaceFormatsKHR(*device, *surface, &nFormats,
			supports.surfaceFormats.data());
		assert(nFormats > 0);

		// Get present modes
		uint32_t nPresentModes = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &nPresentModes, nullptr);
		supports.presentModes.resize(nPresentModes);
		vkGetPhysicalDeviceSurfacePresentModesKHR(*device, *surface, &nPresentModes,
			supports.presentModes.data());
		assert(nPresentModes > 0);

		return supports;
	}

	VkImageView VulkanSwapchain::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
	{
		VkImageViewCreateInfo info = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.image = image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = format,
			.subresourceRange = {
				.aspectMask = aspectFlags,
				.baseMipLevel = 0,
				.levelCount = mipLevels,
				.baseArrayLayer = 0,
				.layerCount = 1
			},
		};

		VkImageView imageView = VK_NULL_HANDLE;
		VkResult status = vkCreateImageView(*device, &info, *allocator, &imageView);
		assert(status == VK_SUCCESS);

		return imageView;
	}
}
