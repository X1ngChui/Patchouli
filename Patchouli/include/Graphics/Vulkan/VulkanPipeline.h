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

		void setShaders(const std::vector<Ref<VulkanShader>>& shaders) { this->shaders = shaders; }
		void setViewports(const std::vector<VkViewport>& viewports) { this->viewports = viewports; }
		void setScissors(const std::vector<VkRect2D>& scissors) { this->scissors = scissors; }

		void build();

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

		Ref<VulkanDevice> vkDevice = nullptr;
		Ref<VulkanAllocator> vkAllocator = nullptr;
		std::vector<Ref<VulkanShader>> shaders;

		std::vector<VkViewport> viewports;
		std::vector<VkRect2D> scissors;
	};
}