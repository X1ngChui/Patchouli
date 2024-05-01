#pragma once

#include "Core/PObject.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanSubpass : public PObject
	{
	public:
	private:
		VkSubpassDescription vkSubpassDescription;
		// std::vector<VkAttachmentReference>
	};

	class VulkanRenderPass : public PObject
	{
	public:
		VulkanRenderPass() = default;
		~VulkanRenderPass() = default;
	private:
	};
}