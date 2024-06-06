#include "Graphics/Vulkan/VulkanShader.h"
#include "Util/AssetFile.h"

namespace Patchouli
{
	VulkanShader::VulkanShader(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator,
		const std::filesystem::path& path, VkShaderStageFlagBits shaderStage)
		: device(device), allocator(allocator), vkShaderStage(shaderStage)
	{
		AssetFile shaderFile(path);

		VkShaderModuleCreateInfo shaderModuleInfo = {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.codeSize = shaderFile.size(),
			.pCode = (const uint32_t*)shaderFile.data()
		};

		VkResult status = vkCreateShaderModule(*device, &shaderModuleInfo, *allocator, &vkShaderModule);
		assert(status == VK_SUCCESS);
	}

	VulkanShader::~VulkanShader()
	{
		vkDestroyShaderModule(*device, vkShaderModule, *allocator);
	}
}