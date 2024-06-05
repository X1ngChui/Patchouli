#include "Graphics/Vulkan/VulkanShader.h"
#include "Util/File.h"

namespace Patchouli
{
	VulkanShader::VulkanShader(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator,
		const std::filesystem::path& path, VkShaderStageFlagBits shaderStage)
		: device(device), allocator(allocator), vkShaderStage(shaderStage)
	{
		ReadOnlyFile file(path);

		VkShaderModuleCreateInfo shaderModuleInfo = {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.codeSize = file.size(),
			.pCode = (const uint32_t*)file.data()
		};

		VkResult status = vkCreateShaderModule(*device, &shaderModuleInfo, *allocator, &vkShaderModule);
		assert(status == VK_SUCCESS);
	}

	VulkanShader::~VulkanShader()
	{
		vkDestroyShaderModule(*device, vkShaderModule, *allocator);
	}
}