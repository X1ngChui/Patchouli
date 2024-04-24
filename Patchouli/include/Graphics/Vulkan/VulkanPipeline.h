#pragma once

#include "Core/PObject.h"
#include "Core/PObject.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanShader.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanPipeline : public PObject
	{
	public:
		VulkanPipeline(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator);
		virtual ~VulkanPipeline();

		void setShaderGroup(const std::vector<Ref<VulkanShader>>& shaderGroup);

		void build();

	public:
		VkPipelineVertexInputStateCreateInfo vkVertexInputState;
		VkPipelineInputAssemblyStateCreateInfo vkInputAssemblyState;
		VkPipelineViewportStateCreateInfo vkViewportState;
		VkPipelineRasterizationStateCreateInfo vkRasterizationState;
		VkPipelineMultisampleStateCreateInfo vkMultisampleState;
		VkPipelineColorBlendAttachmentState vkBlendAttachmentState;
		VkPipelineColorBlendStateCreateInfo vkBlendState;
		VkPipelineDepthStencilStateCreateInfo vkDepthStencilState;
		VkPipelineLayoutCreateInfo vkPipelineLayoutState;

	private:
		VkPipeline vkPipeline = VK_NULL_HANDLE;
		VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

		Ref<VulkanDevice> vkDevice = nullptr;
		Ref<VulkanAllocator> vkAllocator = nullptr;
		std::vector<Ref<VulkanShader>> shaders;
	};
}