#include "Graphics/Vulkan/VulkanPipeline.h"

namespace Patchouli
{
	VulkanPipeline::VulkanPipeline(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator)
		: vkDevice(device), vkAllocator(allocator)
	{
	}

	VulkanPipeline::~VulkanPipeline()
	{
		if (vkPipelineLayout != VK_NULL_HANDLE)
			vkDestroyPipelineLayout(*vkDevice, vkPipelineLayout, *vkAllocator);

		if (vkPipeline != VK_NULL_HANDLE)
			vkDestroyPipeline(*vkDevice, vkPipeline, *vkAllocator);
	}

	void VulkanPipeline::setShaderGroup(const std::vector<Ref<VulkanShader>>& shaderGroup)
	{
		shaders = shaderGroup;
	}

	void VulkanPipeline::build()
	{
	}
}