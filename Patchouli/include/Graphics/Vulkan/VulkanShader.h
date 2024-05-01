#pragma once

#include "PatchouliPch.h"
#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanShader : public PObject
	{
	public:
		VulkanShader(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator,
			const std::filesystem::path& path, VkShaderStageFlagBits shaderStage, const std::string& entryPoint);
		~VulkanShader();

		auto getShaderStage() const { return vkShaderStage; }
		auto getShaderModule() const { return vkShaderModule; }
		const std::string& getEntryPoint() const { return entryPoint; }

	private:
		VkShaderModule vkShaderModule;
		VkShaderStageFlagBits vkShaderStage;
		std::string entryPoint;

		Ref<VulkanDevice> vkDevice;
		Ref<VulkanAllocator> vkAllocator;
	};
}