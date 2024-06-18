#pragma once

#include "Graphics/Vulkan/VulkanAllocator.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanShader : public RefBase<VulkanShader>
	{
	public:
		virtual ~VulkanShader() = default;

		virtual VkShaderStageFlagBits getShaderStage() const = 0; 
		virtual VkShaderModule getShaderModule() const = 0;
	};

	class VulkanVertexShader : public VulkanShader
	{
	public:
		VulkanVertexShader(const std::filesystem::path& path, Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator);
		virtual ~VulkanVertexShader();

		virtual VkShaderStageFlagBits getShaderStage() const override { return VK_SHADER_STAGE_VERTEX_BIT; }
		virtual VkShaderModule getShaderModule() const override { return vkShaderModule; }
	private:
		VkShaderModule vkShaderModule;
		Ref<VulkanDevice> device;
		Ref<VulkanAllocator> allocator;
	};

	class VulkanFragmentShader : public VulkanShader
	{
	public:
		VulkanFragmentShader(const std::filesystem::path& path, Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator);
		virtual ~VulkanFragmentShader();
	
		virtual VkShaderStageFlagBits getShaderStage() const override { return VK_SHADER_STAGE_FRAGMENT_BIT; }
		virtual VkShaderModule getShaderModule() const override { return vkShaderModule; }
	private:
		VkShaderModule vkShaderModule;
		Ref<VulkanDevice> device;
		Ref<VulkanAllocator> allocator;
	};
}