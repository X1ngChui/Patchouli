#include "Graphics/Vulkan/VulkanPipeline.h"

namespace Patchouli
{
	VulkanPipeline::VulkanPipeline(Ref<VulkanDevice> device, Ref<VulkanAllocator> allocator)
		: vkDevice(device), vkAllocator(allocator)
	{
		VkViewport viewport = {
			.x = 0.0f,
			.y = 0.0f,
			.width = 1920.0f,
			.height = 1080.0f,
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		};

		VkRect2D scissor = {
			.offset = { 0, 0 },
			.extent = { 1920, 1080 }
		};

		this->setViewports({ viewport });
		this->setScissors({ scissor });

		// Load shaders
		Ref<VulkanShader> shaderVertex = makeRef<VulkanShader>(device, allocator, "assets/shaders/spirV/simpleShaderVert.spv", VK_SHADER_STAGE_VERTEX_BIT, "main");
		Ref<VulkanShader> shaderFragment = makeRef<VulkanShader>(device, allocator, "assets/shaders/spirV/simpleShaderFrag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, "main");

		this->setShaders({ shaderVertex, shaderFragment });

		std::vector<VkPipelineShaderStageCreateInfo> shaderStageCreateInfos;
		for (auto shader : shaders)
		{
			VkPipelineShaderStageCreateInfo shaderStageCreateInfo = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.stage = shader->getShaderStage(),
				.module = shader->getShaderModule(),
				.pName = shader->getEntryPoint().c_str(),
				.pSpecializationInfo = nullptr
			};
			shaderStageCreateInfos.push_back(shaderStageCreateInfo);
		}

		vkViewportStateCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.viewportCount = static_cast<uint32_t>(viewports.size()),
			.pViewports = viewports.data(),
			.scissorCount = static_cast<uint32_t>(scissors.size()),
			.pScissors = scissors.data()

		};

		vkVertexInputStateCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.vertexBindingDescriptionCount = 0,
			.pVertexBindingDescriptions = nullptr,
			.vertexAttributeDescriptionCount = 0,
			.pVertexAttributeDescriptions = nullptr,
		};

		vkInputAssemblyStateCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE,
		};

		vkRasterizationStateCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.polygonMode = VK_POLYGON_MODE_FILL,
			.cullMode = VK_CULL_MODE_BACK_BIT,
			.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
			.depthBiasEnable = VK_FALSE,
			.depthBiasConstantFactor = 0.0f,
			.depthBiasClamp = 0.0f,
			.depthBiasSlopeFactor = 0.0f,
			.lineWidth = 1.0f,
		};

		// Multisample in now disabled.
		vkMultisampleStateCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			.sampleShadingEnable = VK_FALSE,
			.minSampleShading = 1.0f,
			.pSampleMask = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable = VK_FALSE
		};

		// Depth and stencil test is now neglected.

		// Color bledning is now enabled.
		vkColorBlendAttachmentStates.push_back({
			.blendEnable = VK_FALSE,
			.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
			.colorBlendOp = VK_BLEND_OP_ADD,
			.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
			.alphaBlendOp = VK_BLEND_OP_ADD,
			.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		});

		vkColorBlendStateCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.attachmentCount = static_cast<uint32_t>(vkColorBlendAttachmentStates.size()),
			.pAttachments = vkColorBlendAttachmentStates.data(),
			.blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f },
		};

		vkLayoutCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.setLayoutCount = 0,
			.pSetLayouts = nullptr,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr
		};

		VkResult result = vkCreatePipelineLayout(*device, &vkLayoutCreateInfo, *allocator, &vkPipelineLayout);
		assert(result == VK_SUCCESS);

		VkGraphicsPipelineCreateInfo vkGraphicsPipelineCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.stageCount = static_cast<uint32_t>(shaderStageCreateInfos.size()),
			.pStages = shaderStageCreateInfos.data(),
			.pVertexInputState = &vkVertexInputStateCreateInfo,
			.pInputAssemblyState = &vkInputAssemblyStateCreateInfo,
			.pViewportState = &vkViewportStateCreateInfo,
			.pRasterizationState = &vkRasterizationStateCreateInfo,
			.pMultisampleState = &vkMultisampleStateCreateInfo,
			.pDepthStencilState = nullptr, // Depth and stencil test is now neglected
			.pColorBlendState = &vkColorBlendStateCreateInfo,
			.layout = vkPipelineLayout,
			.renderPass = VK_NULL_HANDLE,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1
		};

		result = vkCreateGraphicsPipelines(*device, VK_NULL_HANDLE, 1, &vkGraphicsPipelineCreateInfo, *allocator, &vkPipeline);
		assert(result == VK_SUCCESS);
	}

	VulkanPipeline::~VulkanPipeline()
	{
		if (vkPipelineLayout != VK_NULL_HANDLE)
			vkDestroyPipelineLayout(*vkDevice, vkPipelineLayout, *vkAllocator);

		if (vkPipeline != VK_NULL_HANDLE)
			vkDestroyPipeline(*vkDevice, vkPipeline, *vkAllocator);
	}
}