#include "shaderpipes.h"

// prototype definitions
unsigned char createPipeline(vksdl_shit* vksdlshit, char* vertShader, char* fragShader)
{
	// INFO //
	// shader inputs must be the file location relative to the executable

	// load shaders
	shaderHandle vert = loadShader(vertShader);
	shaderHandle frag = loadShader(fragShader);
	
	// create shader modules
	VkShaderModule vertModule;
	VkShaderModuleCreateInfo vertModuleInfo;
	vertModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vertModuleInfo.codeSize = vert.size;
	vertModuleInfo.pCode = (uint32_t*)vert.code;
	vertModuleInfo.pNext = NULL;
	vertModuleInfo.flags = 0;

	VkShaderModule fragModule;
	VkShaderModuleCreateInfo fragModuleInfo;
	fragModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	fragModuleInfo.codeSize = frag.size;
	fragModuleInfo.pCode = (uint32_t*)frag.code;
	fragModuleInfo.pNext = NULL;
	fragModuleInfo.flags = 0;

	if (vkCreateShaderModule(vksdlshit->logicalDevice, &vertModuleInfo, NULL, &vertModule) != VK_SUCCESS)
	{
		printf("Failed to create vertex module.\n");
		return 0;
	}
	if (vkCreateShaderModule(vksdlshit->logicalDevice, &fragModuleInfo, NULL, &fragModule) != VK_SUCCESS)
	{
		printf("Failed to create fragment module.\n");
		return 0;
	}
	free(vert.code);
	vert.code = NULL;
	free(frag.code);
	frag.code = NULL;

	// begin pipeline creation
	VkPipelineShaderStageCreateInfo vertShaderStageInfo;
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertModule;
	vertShaderStageInfo.pName = "main";
	vertShaderStageInfo.pSpecializationInfo = NULL;
	vertShaderStageInfo.pNext = NULL;
	vertShaderStageInfo.flags = 0;

	VkPipelineShaderStageCreateInfo fragShaderStageInfo;
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragModule;
	fragShaderStageInfo.pName = "main";
	fragShaderStageInfo.pSpecializationInfo = NULL;
	fragShaderStageInfo.pNext = NULL;
	fragShaderStageInfo.flags = 0;

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	// handle vertex input
	VkVertexInputBindingDescription bindingDescription;
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(float) * 6; // 3 for pos, 3 for color
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	// vertex attributes
	VkVertexInputAttributeDescription attributeDescriptions[2];
	// position
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = 0;
	// color
	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = sizeof(float) * 3;

	VkPipelineVertexInputStateCreateInfo vertexInputInfo;
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.flags = 0;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = 2;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

	// uniforms
	VkDescriptorSetLayout descriptorSetLayout;

	VkDescriptorSetLayoutBinding bindingLayout;
	bindingLayout.binding = 0;
	bindingLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	bindingLayout.descriptorCount = 1;
	bindingLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	bindingLayout.pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo layoutInfo;
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &bindingLayout;

	if (vkCreateDescriptorSetLayout(vksdlshit->logicalDevice, &layoutInfo, NULL, &descriptorSetLayout) != VK_SUCCESS)
	{
		printf("Failed to create descriptor set layout.\n");
		return 0;
	}
	printf("Descriptor set layout created.\n");


	VkDescriptorSetLayout setLayouts[] = { descriptorSetLayout };
	VkDescriptorSetAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = vksdlshit->descriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = setLayouts;

	if (vkAllocateDescriptorSets(vksdlshit->logicalDevice, &allocInfo, &vksdlshit->descriptorSet) != VK_SUCCESS)
	{
		printf("Failed to allocate descriptor set.\n");
		return 0;
	}
	printf("Descriptor sets allocated.\n");

	VkDescriptorBufferInfo bufferInfo;
	bufferInfo.buffer = vksdlshit->uniformBuffer;
	bufferInfo.offset = 0;
	bufferInfo.range = sizeof(transform);

	VkWriteDescriptorSet descriptorWrite;
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;;
	descriptorWrite.dstSet = vksdlshit->descriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pBufferInfo = &bufferInfo;
	descriptorWrite.pImageInfo = NULL; // optional
	descriptorWrite.pTexelBufferView = NULL; // optional

	vkUpdateDescriptorSets(vksdlshit->logicalDevice, 1, &descriptorWrite, 0, NULL);

	// input assembler
	VkPipelineInputAssemblyStateCreateInfo inputAssembly;
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.flags = 0;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	//viewport.width = vksdlshit->extent.width;
	viewport.width = WIDTH;
	//viewport.height = vksdlshit->extent.height;
	viewport.height = HEIGHT;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor;
	VkOffset2D offset = { 0, 0 };
	//VkExtent2D extent = { vksdlshit->extent.width, vksdlshit->extent.height };
	VkExtent2D extent = { WIDTH, HEIGHT };
	scissor.offset = offset;
	scissor.extent = extent;

	VkPipelineViewportStateCreateInfo viewportState;
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	// rasterizer
	VkPipelineRasterizationStateCreateInfo rasterizer;
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	// multisampling
	VkPipelineMultisampleStateCreateInfo multisampling;
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = NULL; /// Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE;
	

	// depth and stencil testing
	// NULL for now

	// color blending (both disabled for now)
	VkPipelineColorBlendAttachmentState colorBlendAttachment;
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
	

	VkPipelineColorBlendStateCreateInfo colorBlending;
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	// dynamic state
	VkDynamicState dynamicStates[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};

	VkPipelineDynamicStateCreateInfo dynamicState;
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = 0;
	dynamicState.pDynamicStates = NULL;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo;
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.flags = 0;
	pipelineLayoutInfo.setLayoutCount = 1; // Optional
	pipelineLayoutInfo.pSetLayouts = setLayouts; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = 0;

	if (vkCreatePipelineLayout(vksdlshit->logicalDevice, &pipelineLayoutInfo, VK_NULL_HANDLE, &vksdlshit->pipelineLayout) != VK_SUCCESS)
	{
		printf("Failed to create pipeline layout.\n");
		return 0;
	}

	// create the fucking pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo;
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.flags = 0;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = VK_NULL_HANDLE; // Optional
	pipelineInfo.pTessellationState = VK_NULL_HANDLE; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = VK_NULL_HANDLE; // Optional
	pipelineInfo.layout = vksdlshit->pipelineLayout;
	pipelineInfo.renderPass = vksdlshit->renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional, but will be very useful in the future. set VK_PIPELINE_CREATE_DERIVATIVE_BIT in flags field
	pipelineInfo.basePipelineIndex = -1; // see above

	if (vkCreateGraphicsPipelines(vksdlshit->logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &vksdlshit->pipeline) != VK_SUCCESS)
	{
		printf("Failed to create pipeline.\n");
		return 0;
	}
	printf("pipeline created\n");

	// clean up
	vkDestroyShaderModule(vksdlshit->logicalDevice, vertModule, NULL);
	vkDestroyShaderModule(vksdlshit->logicalDevice, fragModule, NULL);

	return 1;
}
shaderHandle loadShader(char* shaderLoc)
{
	// handles
	shaderHandle handle;

	// acquire file size
	FILE* shaderFile = fopen(shaderLoc, "rb");
	fseek(shaderFile, 0, SEEK_END);
	handle.size = (size_t)ftell(shaderFile);

	// allocate memory
	handle.code = malloc(handle.size);

	// copy data
	rewind(shaderFile);
	fread(handle.code, 1, handle.size, shaderFile);

	fclose(shaderFile);

	// return data
	return handle;
}