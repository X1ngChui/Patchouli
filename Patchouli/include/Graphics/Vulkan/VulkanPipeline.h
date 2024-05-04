#pragma once

#include "Core/PObject.h"
#include "Core/PObject.h"
#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanShader.h"
#include "Graphics/Vulkan/VulkanRenderPass.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	class VulkanPipeline : public PObject
	{
	public:
		VulkanPipeline(Ref<VulkanRenderPass> renderPass, Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator);
		virtual ~VulkanPipeline();

		void setShaders(const VulkanVector<Ref<VulkanShader>>& shaders) { this->shaders = shaders; }
		void setViewports(const VulkanVector<VkViewport>& viewports) { this->viewports = viewports; }
		void setScissors(const VulkanVector<VkRect2D>& scissors) { this->scissors = scissors; }

	public:
		VkPipelineVertexInputStateCreateInfo vkVertexInputStateCreateInfo;
		VkPipelineInputAssemblyStateCreateInfo vkInputAssemblyStateCreateInfo;
		VkPipelineViewportStateCreateInfo vkViewportStateCreateInfo;
		VkPipelineRasterizationStateCreateInfo vkRasterizationStateCreateInfo;
		VkPipelineMultisampleStateCreateInfo vkMultisampleStateCreateInfo;
		VulkanVector<VkPipelineColorBlendAttachmentState> vkColorBlendAttachmentStates;
		VkPipelineColorBlendStateCreateInfo vkColorBlendStateCreateInfo;
		VkPipelineDepthStencilStateCreateInfo vkDepthStencilStateCreateInfo;
		VkPipelineLayoutCreateInfo vkLayoutCreateInfo;

	private:
		VkPipeline vkPipeline = VK_NULL_HANDLE;
		VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;

		Ref<VulkanDevice> device = nullptr;
		Ref<VulkanAllocator> allocator = nullptr;
		VulkanVector<Ref<VulkanShader>> shaders;

		VulkanVector<VkViewport> viewports;
		VulkanVector<VkRect2D> scissors;
	};
}