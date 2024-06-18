#pragma once

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanShader.h"
#include "Graphics/Vulkan/VulkanRenderPass.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	constexpr const char* VULKAN_SHADER_ENTRY = "main";
	constexpr std::size_t VULKAN_MAX_SHADERS = 16;
	constexpr std::size_t VULKAN_MAX_VIEWPORTS = 16;
	constexpr std::size_t VULKAN_MAX_SCISSORS = 16;

	class VulkanPipeline : public RefBase<VulkanPipeline>
	{
	public:
		VulkanPipeline(Ref<VulkanRenderPass> renderPass, Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator);
		virtual ~VulkanPipeline();

		operator VkPipeline() const { return vkPipeline; }

	public:
		VkPipelineVertexInputStateCreateInfo vkVertexInputStateCreateInfo;
		VkPipelineInputAssemblyStateCreateInfo vkInputAssemblyStateCreateInfo;
		VkPipelineViewportStateCreateInfo vkViewportStateCreateInfo;
		VkPipelineRasterizationStateCreateInfo vkRasterizationStateCreateInfo;
		VkPipelineMultisampleStateCreateInfo vkMultisampleStateCreateInfo;
		std::vector<VkPipelineColorBlendAttachmentState> vkColorBlendAttachmentStates;
		VkPipelineColorBlendStateCreateInfo vkColorBlendStateCreateInfo;
		VkPipelineDepthStencilStateCreateInfo vkDepthStencilStateCreateInfo;
		VkPipelineLayoutCreateInfo vkLayoutCreateInfo;

	private:
		VkPipeline vkPipeline = VK_NULL_HANDLE;
		VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

		Ref<VulkanDevice> device = nullptr;
		Ref<VulkanAllocator> allocator = nullptr;
		
		std::size_t nShaders = 0;
		std::array<Ref<VulkanShader>, VULKAN_MAX_SHADERS> shaders;
	};
}