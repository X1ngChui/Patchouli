#pragma once

#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanShader : public RefBase<VulkanShader>
	{
	public:
		VulkanShader(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator,
			const std::filesystem::path& path, VkShaderStageFlagBits shaderStage);
		~VulkanShader();

		auto getShaderStage() const { return vkShaderStage; }
		auto getShaderModule() const { return vkShaderModule; }

	private:
		VkShaderModule vkShaderModule;
		VkShaderStageFlagBits vkShaderStage;

		Ref<VulkanDevice> device;
		Ref<VulkanAllocator> allocator;
	};
}