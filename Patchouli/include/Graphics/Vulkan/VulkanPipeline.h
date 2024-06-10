#pragma once

#include "Graphics/Vulkan/VulkanDevice.h"
#include "Graphics/Vulkan/VulkanShader.h"
#include "Graphics/Vulkan/VulkanRenderPass.h"
#include <vulkan/vulkan.h>

namespace Patchouli
{
	constexpr const char* VULKAN_SHADER_ENTRY = "main";

	class VulkanPipeline : public RefBase<VulkanPipeline>
	{
	public:
		VulkanPipeline(Ref<VulkanRenderPass> renderPass, Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator);
		virtual ~VulkanPipeline();

		void setShaders(const std::vector<Ref<VulkanShader>>& shaders) { this->shaders = shaders; }
		void setViewports(const std::vector<VkViewport>& viewports) { this->viewports = viewports; }
		void setScissors(const std::vector<VkRect2D>& scissors) { this->scissors = scissors; }

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
		std::vector<Ref<VulkanShader>> shaders;

		std::vector<VkViewport> viewports;
		std::vector<VkRect2D> scissors;
	};
}