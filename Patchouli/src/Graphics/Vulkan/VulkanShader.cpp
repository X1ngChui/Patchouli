#include "Graphics/Vulkan/VulkanShader.h"

namespace Patchouli
{
	 VulkanShader::VulkanShader(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator, const std::filesystem::path& path, VkShaderStageFlagBits shaderStage, const std::string& entryPoint)
		: device(device), allocator(allocator), vkShaderStage(shaderStage), entryPoint(entryPoint)
	{
		std::ifstream file(path, std::ios::binary | std::ios::in);
		assert(file.is_open());

		VulkanVector<char> spirv{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

		VkShaderModuleCreateInfo shaderModuleInfo = {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.codeSize = spirv.size(),
			.pCode = (uint32_t*)spirv.data()
		};

		VkResult status = vkCreateShaderModule(*device, &shaderModuleInfo, *allocator, &vkShaderModule);
		assert(status == VK_SUCCESS);
	}

	VulkanShader::~VulkanShader()
	{
		vkDestroyShaderModule(*device, vkShaderModule, *allocator);
	}
}