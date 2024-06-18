#include "Graphics/Vulkan/VulkanShader.h"

namespace Patchouli
{
	VulkanVertexShader::VulkanVertexShader(const std::filesystem::path& path, Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator)
		: device(device), allocator(allocator)
	{
		std::ifstream file(path, std::ios::binary | std::ios::in);
		assert(file.is_open());

		std::vector<char> spirv{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

		VkShaderModuleCreateInfo shaderModuleInfo = {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.codeSize = spirv.size(),
			.pCode = (const uint32_t*)spirv.data()
		};

		VkResult status = vkCreateShaderModule(*device, &shaderModuleInfo, *allocator, &vkShaderModule);
		assert(status == VK_SUCCESS);
	}

	VulkanVertexShader::~VulkanVertexShader()
	{
		vkDestroyShaderModule(*device, vkShaderModule, *allocator);
	}

	VulkanFragmentShader::VulkanFragmentShader(const std::filesystem::path& path, Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator)
		: device(device), allocator(allocator)
	{
		std::ifstream file(path, std::ios::binary | std::ios::in);
		assert(file.is_open());

		std::vector<char> spirv{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

		VkShaderModuleCreateInfo shaderModuleInfo = {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.codeSize = spirv.size(),
			.pCode = (const uint32_t*)spirv.data()
		};

		VkResult status = vkCreateShaderModule(*device, &shaderModuleInfo, *allocator, &vkShaderModule);
		assert(status == VK_SUCCESS);
	}

	VulkanFragmentShader::~VulkanFragmentShader()
	{
		vkDestroyShaderModule(*device, vkShaderModule, *allocator);
	}
}