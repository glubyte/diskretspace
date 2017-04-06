// local
#include "vksdl.h"
#include "shaderpipes.h"

// global definitions
int WIDTH = 1600;
int HEIGHT = 900;

// temporary spot for vertex data
float vertices[] = {
	/*
	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f
	 */
	 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};
uint16_t indices[] = {
	/*
	0, 1, 2,
	2, 3, 0
	*/
	0, 1, 2,
	2, 3, 0,
	4, 5, 6,
	6, 7, 4,
	0, 4, 7,
	7, 3, 0,
	1, 5, 6,
	6, 2, 1,
	3, 2, 6,
	6, 7, 3,
	0, 1, 5,
	5, 4, 0
};

// prototype definitions
unsigned char initVkSDL(vksdl_shit* vksdlshit)
{
	vksdlshit->swapChain = VK_NULL_HANDLE;

	if (!createInstance(vksdlshit))
		return 0;
	if (!initSDL(vksdlshit))
		return 0;
	if (!createVulkanSurface(vksdlshit))
		return 0;
	if (!pickPhysicalDevice(vksdlshit))
		return 0;
	if (!createLogicalDevice(vksdlshit))
		return 0;
	if (!createSwapChain(vksdlshit))
		return 0;
	if (!createImageViews(vksdlshit))
		return 0;
	if (!createRenderPass(vksdlshit))
		return 0;
	if (!createFramebuffers(vksdlshit))
		return 0;
	if (!createCommandPool(vksdlshit))
		return 0;
	if (!createVertexBuffer(vksdlshit))
		return 0;
	if (!createIndexBuffer(vksdlshit))
		return 0;
	if (!createUniformBuffer(vksdlshit))
		return 0;
	if (!createDescriptorPool(vksdlshit))
		return 0;
	if (!createCommandBuffers(vksdlshit))
		return 0;
	if (!createSemaphores(vksdlshit))
		return 0;

	return 1;
}
unsigned char createInstance(vksdl_shit* vksdlshit)
{
#if defined(SDL_VIDEO_DRIVER_WINDOWS) && defined(VK_USE_PLATFORM_WIN32_KHR)
	const char* extensionNames[] = { VK_KHR_SURFACE_EXTENSION_NAME , VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
#endif

#if defined(SDL_VIDEO_DRIVER_X11) && defined(VK_USE_PLATFORM_XLIB_KHR)
	const char* extensionNames[] = { VK_KHR_SURFACE_EXTENSION_NAME, "VK_KHR_xlib_surface" };
#endif

	VkResult result;
	const char* validationLayers[] = { "VK_LAYER_LUNARG_standard_validation" };

	// info shit
	VkApplicationInfo appInfo = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Diskretspace",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "No Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_0
	};

	// more info shit
	VkInstanceCreateInfo instInfo = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = 2,
		.ppEnabledExtensionNames = extensionNames
	};

	result = vkCreateInstance(&instInfo, NULL, &vksdlshit->instance);
	if (result == VK_ERROR_INCOMPATIBLE_DRIVER)
	{
		printf("Unable to find compatible Vulkan driver, organism.\n");
		return 0;
	}
	else if (result)
	{
		printf("Could not create a Vulkan instance (unknown).\n");
		return 0;
	}

	return 1;
}
unsigned char initSDL(vksdl_shit* vksdlshit)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("Could not initialize SDL. SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	vksdlshit->window = SDL_CreateWindow("Diskretspace", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (vksdlshit->window == NULL)
	{
		printf("Could not create SDL window. SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	return 1;
}
unsigned char createVulkanSurface(vksdl_shit* vksdlshit)
{
	SDL_SysWMinfo windowInfo;
	SDL_VERSION(&windowInfo.version);
	if (!SDL_GetWindowWMInfo(vksdlshit->window, &windowInfo))
	{
		printf("SDL window manager info is not available. SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	switch (windowInfo.subsystem) {

#if defined(SDL_VIDEO_DRIVER_WINDOWS) && defined(VK_USE_PLATFORM_WIN32_KHR)
	case SDL_SYSWM_WINDOWS: {
		VkWin32SurfaceCreateInfoKHR surfaceInfo = {
			.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
			.hinstance = GetModuleHandle(NULL),
			.hwnd = windowInfo.info.win.window
		};

		VkResult result = vkCreateWin32SurfaceKHR(vksdlshit->instance, &surfaceInfo, NULL, &vksdlshit->surface);
		if (result != VK_SUCCESS)
		{
			printf("Failed to create Win32 surface.");
			return 0;
		}
		return 1;
	}
#endif

#if defined(SDL_VIDEO_DRIVER_X11) && defined(VK_USE_PLATFORM_XLIB_KHR)
	case SDL_SYSWM_X11: {
		VkXlibSurfaceCreateInfoKHR surfaceInfo = {
			.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
			.dpy = windowInfo.info.x11.display,
			.window = windowInfo.info.x11.window
		};

		VkResult result = vkCreateXlibSurfaceKHR(vksdlshit->instance, &surfaceInfo, NULL, &vksdlshit->surface);
		if (result != VK_SUCCESS) {
			std::cout << "Failed to create X11 surface." << std::endl;
			return 0;
		}
		return 1;
	}
#endif

	default:
		printf("Unsupported window manager.");
		return 0;
	}
}
unsigned char pickPhysicalDevice(vksdl_shit* vksdlshit)
{
	unsigned char i;
	uint32_t deviceCount = 0;

	vksdlshit->physicalDevice = VK_NULL_HANDLE;
	vkEnumeratePhysicalDevices(vksdlshit->instance, &deviceCount, NULL);
	if (deviceCount == 0)
	{
		printf("Failed to find GPU with Vulkan support.\nGotta get modern, boy.\n");
		return 0;
	}
	VkPhysicalDevice *devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
	vkEnumeratePhysicalDevices(vksdlshit->instance, &deviceCount, devices);
	VkPhysicalDeviceProperties gpuProperties;
	printf("Select a GPU, organism.\n");
	for (i = 0; i < deviceCount; ++i)
	{
		vkGetPhysicalDeviceProperties(devices[i], &gpuProperties);
		printf("%i. %s\n", i + 1, gpuProperties.deviceName);
	}
	scanf("%i", &i);
	while (i < 1 || i > deviceCount)
	{
		printf("That GPU does not exist, organism.\n");
		scanf("%i", &i);
	}
	printf("Understood. Successive initializations will now retrieve this information from the interface configuration file.\n");
	vksdlshit->physicalDevice = devices[i - 1];
	vkGetPhysicalDeviceProperties(vksdlshit->physicalDevice, &vksdlshit->gpuProperties);
	free(devices);
	devices = NULL;



	// queue family selection
	uint32_t queueFamilyCount = 0;
	VkQueueFamilyProperties *queueProperties;
	vkGetPhysicalDeviceQueueFamilyProperties(vksdlshit->physicalDevice, &queueFamilyCount, NULL);
	queueProperties = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(vksdlshit->physicalDevice, &queueFamilyCount, queueProperties);
	VkBool32 presentSupport = FALSE;
	for (i = 0; i < queueFamilyCount; ++i)
	{
		if (queueProperties[i].queueCount > 0 && queueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			vksdlshit->queueIndex = i;
		}
	}
	free(queueProperties);
	queueProperties = NULL;

	// check for surface support
	VkBool32 supported;
	vkGetPhysicalDeviceSurfaceSupportKHR(vksdlshit->physicalDevice, vksdlshit->queueIndex, vksdlshit->surface, &supported);
	if (supported == FALSE)
	{
		printf("Physical device does not support surface.\n");
		return 0;
	}

	return 1;
}
unsigned char createLogicalDevice(vksdl_shit* vksdlshit)
{
	vksdlshit->logicalDevice = VK_NULL_HANDLE;
	float priorities[] = {0.0};
	const char *deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	const char *validationLayers[] = { "VK_LAYER_LUNARG_standard_validation" };

	VkDeviceQueueCreateInfo queueInfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.queueFamilyIndex = vksdlshit->queueIndex,
		.queueCount = 1,
		.pQueuePriorities = priorities
	};

	VkPhysicalDeviceFeatures deviceFeatures; // to be modulated later

	VkDeviceCreateInfo deviceInfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.pQueueCreateInfos = &queueInfo,
		.queueCreateInfoCount = 1,
		.pEnabledFeatures = NULL,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = 1,
		.ppEnabledExtensionNames = deviceExtensions
	};

	if (vkCreateDevice(vksdlshit->physicalDevice, &deviceInfo, NULL, &vksdlshit->logicalDevice) != VK_SUCCESS)
	{
		printf("Failed to create logical device.\n");
		return 0;
	}

	// create queue handle
	vkGetDeviceQueue(vksdlshit->logicalDevice, vksdlshit->queueIndex, 0, &vksdlshit->queue);

	return 1;
}
unsigned char createSwapChain(vksdl_shit* vksdlshit)
{
	char i;
	vksdlshit->imageCount = 0;
	VkSwapchainKHR oldSwapChain = vksdlshit->swapChain;

	// actually check for swap chain support
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(vksdlshit->physicalDevice, NULL, &extensionCount, NULL);
	VkExtensionProperties* extensionProperties = malloc(sizeof(VkExtensionProperties) * extensionCount);
	vkEnumerateDeviceExtensionProperties(vksdlshit->physicalDevice, NULL, &extensionCount, extensionProperties);
	for (i = 0; i < extensionCount; ++i)
	{
		if (extensionProperties[i].extensionName == "VK_KHR_swapchain");
		{
			break;
		}
	}
	free(extensionProperties);
	extensionProperties = NULL;

	// get surface capabilities
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vksdlshit->physicalDevice, vksdlshit->surface, &surfaceCapabilities);
	// get supported surface formats
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(vksdlshit->physicalDevice, vksdlshit->surface, &formatCount, NULL);
	if (formatCount == 0)
	{
		printf("No supported surface formats.\n");
		return 0;
	}
	VkSurfaceFormatKHR* surfaceFormats = malloc(sizeof(VkSurfaceFormatKHR) * formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(vksdlshit->physicalDevice, vksdlshit->surface, &formatCount, surfaceFormats);
	// set surface format
	// default if nothing better is available
	//vksdlshit->surfaceFormat.format = surfaceFormats[0].format;
	//vksdlshit->surfaceFormat.colorSpace = surfaceFormats[0].colorSpace;
	// check for better shit
	if (formatCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		vksdlshit->surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
		vksdlshit->surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	}
	else
	{
		for (i = 0; i < formatCount; ++i)
		{
			if (surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_UNORM && surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				vksdlshit->surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
				vksdlshit->surfaceFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
				break;
			}
		}
	}
	free(surfaceFormats);
	surfaceFormats = NULL;

	// get supported present modes
	uint32_t presentModeCount;
	VkPresentModeKHR presentMode;
	vkGetPhysicalDeviceSurfacePresentModesKHR(vksdlshit->physicalDevice, vksdlshit->surface, &presentModeCount, NULL);
	if (presentModeCount == 0)
	{
		printf("No supported present modes.\n");
		return 0;
	}
	VkPresentModeKHR* presentModes = malloc(sizeof(VkPresentModeKHR) * presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(vksdlshit->physicalDevice, vksdlshit->surface, &presentModeCount, presentModes);
	// set default mode if nothing better is supported
	// presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
	// check for best mode (triple buffering)
	for (i = 0; i < presentModeCount; ++i)
	{
		if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
	}
	free(presentModes);
	presentModes = NULL;

	// set swap extent
	if (surfaceCapabilities.currentExtent.width == -1)
	{
		vksdlshit->extent.width = (uint32_t)WIDTH;
		vksdlshit->extent.height = (uint32_t)HEIGHT;

		if (vksdlshit->extent.width < surfaceCapabilities.minImageExtent.width)
			vksdlshit->extent.width = surfaceCapabilities.minImageExtent.width;
		else if (vksdlshit->extent.width > surfaceCapabilities.maxImageExtent.width)
			vksdlshit->extent.width = surfaceCapabilities.maxImageExtent.width;

		if (vksdlshit->extent.height < surfaceCapabilities.minImageExtent.height)
			vksdlshit->extent.height = surfaceCapabilities.minImageExtent.height;
		else if (vksdlshit->extent.height > surfaceCapabilities.maxImageExtent.height)
			vksdlshit->extent.height = surfaceCapabilities.maxImageExtent.height;
	}
	else
	{
		vksdlshit->extent = surfaceCapabilities.currentExtent;
	}

	// image count
	vksdlshit->imageCount = surfaceCapabilities.minImageCount + 1;
	if (surfaceCapabilities.maxImageCount > 0 && vksdlshit->imageCount > surfaceCapabilities.maxImageCount)
	{
		vksdlshit->imageCount = surfaceCapabilities.maxImageCount;
	}

	// pre transform
	VkSurfaceTransformFlagsKHR preTransform;
	if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		preTransform = surfaceCapabilities.currentTransform;
	}

	// create the fucking swap chain
	VkSwapchainCreateInfoKHR swapChainInfo;
	swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainInfo.pNext = NULL;
	swapChainInfo.surface = vksdlshit->surface;
	swapChainInfo.minImageCount = vksdlshit->imageCount;
	swapChainInfo.imageFormat = vksdlshit->surfaceFormat.format;
	swapChainInfo.imageColorSpace = vksdlshit->surfaceFormat.colorSpace;
	swapChainInfo.imageExtent = vksdlshit->extent;
	swapChainInfo.imageArrayLayers = 1;
	swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapChainInfo.queueFamilyIndexCount = 0;
	swapChainInfo.pQueueFamilyIndices = NULL;
	swapChainInfo.preTransform = preTransform;
	swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainInfo.presentMode = presentMode;
	swapChainInfo.clipped = VK_TRUE;
	swapChainInfo.oldSwapchain = oldSwapChain;

	printf("swap chain about to create\n");
	if (vkCreateSwapchainKHR(vksdlshit->logicalDevice, &swapChainInfo, NULL, &vksdlshit->swapChain) != VK_SUCCESS)
	{
		printf("Failed to create swap chain.\n");
		return 0;
	}
	printf("swap chain created\n");

	// destroy old swap chain
	if (oldSwapChain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(vksdlshit->logicalDevice, oldSwapChain, NULL);
		printf("old swap chain destroyed\n");
	}

	// get images
	vkGetSwapchainImagesKHR(vksdlshit->logicalDevice, vksdlshit->swapChain, &vksdlshit->imageCount, NULL);
	vksdlshit->images = malloc(sizeof(VkImage) * vksdlshit->imageCount);
	vkGetSwapchainImagesKHR(vksdlshit->logicalDevice, vksdlshit->swapChain, &vksdlshit->imageCount, vksdlshit->images);
	

	return 1;
}
unsigned char createImageViews(vksdl_shit* vksdlshit)
{
	vksdlshit->imageViews = malloc(sizeof(VkImageView) * vksdlshit->imageCount);
	VkImageViewCreateInfo imageViewInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = vksdlshit->surfaceFormat.format,
		.components = {
			.r = VK_COMPONENT_SWIZZLE_IDENTITY,
			.g = VK_COMPONENT_SWIZZLE_IDENTITY,
			.b = VK_COMPONENT_SWIZZLE_IDENTITY,
			.a = VK_COMPONENT_SWIZZLE_IDENTITY
			},
		.subresourceRange = {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.baseMipLevel = 0,
			.levelCount = 1,
			.baseArrayLayer = 0,
			.layerCount = 1
			}
	};
	for (unsigned char i = 0; i < vksdlshit->imageCount; ++i)
	{
		imageViewInfo.image = vksdlshit->images[i];
		if (vkCreateImageView(vksdlshit->logicalDevice, &imageViewInfo, NULL, &vksdlshit->imageViews[i]) != VK_SUCCESS)
		{
			printf("Failed to create image views.\n");
			return 0;
		}
	}

	return 1;
}
unsigned char createRenderPass(vksdl_shit* vksdlshit)
{
	VkAttachmentDescription colorAttachment;
	colorAttachment.format = vksdlshit->surfaceFormat.format;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef;
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass;
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	subpass.inputAttachmentCount = 0;
	subpass.pInputAttachments = NULL;
	subpass.pResolveAttachments = NULL;
	subpass.pDepthStencilAttachment = NULL;
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments = NULL;

	VkSubpassDependency dependency;
	dependency.dependencyFlags = 0;
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo;
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(vksdlshit->logicalDevice, &renderPassInfo, NULL, &vksdlshit->renderPass) != VK_SUCCESS)
	{
		printf("Failed to create render pass.\n");
		return 0;
	}

	return 1;
}
unsigned char createFramebuffers(vksdl_shit* vksdlshit)
{
	vksdlshit->framebuffers = malloc(sizeof(VkFramebuffer) * vksdlshit->imageCount);
	VkFramebufferCreateInfo framebufferInfo;
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = vksdlshit->renderPass;
	framebufferInfo.attachmentCount = 1;
	framebufferInfo.width = (uint32_t)WIDTH;
	framebufferInfo.height = (uint32_t)HEIGHT;
	framebufferInfo.layers = 1;
	for (unsigned char i = 0; i < vksdlshit->imageCount; ++i)
	{
		framebufferInfo.pAttachments = &vksdlshit->imageViews[i];

		if (vkCreateFramebuffer(vksdlshit->logicalDevice, &framebufferInfo, NULL, &vksdlshit->framebuffers[i]) != VK_SUCCESS)
		{
			printf("Failed to create framebuffers.\n");
			return 0;
		}
	}
	
	return 1;
}
unsigned char createCommandPool(vksdl_shit* vksdlshit)
{
	VkCommandPoolCreateInfo poolInfo;
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = vksdlshit->queueIndex;
	poolInfo.flags = 0;
	if (vkCreateCommandPool(vksdlshit->logicalDevice, &poolInfo, NULL, &vksdlshit->commandPool) != VK_SUCCESS)
	{
		printf("Failed to create command pool.\n");
		return 0;
	}

	return 1;
}
unsigned char createVertexBuffer(vksdl_shit* vksdlshit)
{
	VkDeviceSize bufferSize = sizeof(vertices);

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	// source buffer
	if (!createBuffer(vksdlshit, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&stagingBuffer, &stagingBufferMemory))
		return 0;

	void* data;
	vkMapMemory(vksdlshit->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices, (size_t)bufferSize);
	vkUnmapMemory(vksdlshit->logicalDevice, stagingBufferMemory);

	// destination buffer
	if (!createBuffer(vksdlshit, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&vksdlshit->vertexBuffer, &vksdlshit->vertexBufferMemory))
		return 0;

	copyBuffer(vksdlshit, stagingBuffer, vksdlshit->vertexBuffer, bufferSize);

	vkFreeMemory(vksdlshit->logicalDevice, stagingBufferMemory, NULL);
	vkDestroyBuffer(vksdlshit->logicalDevice, stagingBuffer, NULL);

	return 1;
}
unsigned char createIndexBuffer(vksdl_shit* vksdlshit)
{
	VkDeviceSize bufferSize = sizeof(indices);

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	// source buffer
	if (!createBuffer(vksdlshit, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&stagingBuffer, &stagingBufferMemory))
		return 0;

	void* data;
	vkMapMemory(vksdlshit->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices, (size_t)bufferSize);
	vkUnmapMemory(vksdlshit->logicalDevice, stagingBufferMemory);

	// destination buffer
	if (!createBuffer(vksdlshit, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&vksdlshit->indexBuffer, &vksdlshit->indexBufferMemory))
		return 0;

	copyBuffer(vksdlshit, stagingBuffer, vksdlshit->indexBuffer, bufferSize);

	vkFreeMemory(vksdlshit->logicalDevice, stagingBufferMemory, NULL);
	vkDestroyBuffer(vksdlshit->logicalDevice, stagingBuffer, NULL);

	return 1;
}
unsigned char createUniformBuffer(vksdl_shit* vksdlshit)
{
	VkDeviceSize bufferSize = sizeof(transform);

	createBuffer(vksdlshit, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&vksdlshit->uniformStagingBuffer, &vksdlshit->uniformStagingBufferMemory);

	createBuffer(vksdlshit, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		&vksdlshit->uniformBuffer, &vksdlshit->uniformBufferMemory);

	return 1;
}
unsigned char createDescriptorPool(vksdl_shit* vksdlshit)
{
	VkDescriptorPoolSize poolSize;
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = 1;

	VkDescriptorPoolCreateInfo poolInfo;
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = 1;
	poolInfo.flags = 0;

	if (vkCreateDescriptorPool(vksdlshit->logicalDevice, &poolInfo, NULL, &vksdlshit->descriptorPool) != VK_SUCCESS)
	{
		printf("Failed to create descriptor pool.\n");
		return 0;
	}
	printf("Descriptor pool created.\n");

	return 1;
}
unsigned char createCommandBuffers(vksdl_shit* vksdlshit)
{
	// check for existing command buffers and free
	if (sizeof(vksdlshit->commandBuffers) > sizeof(VkCommandBuffer))
	{
		vkFreeCommandBuffers(vksdlshit->logicalDevice, vksdlshit->commandPool, sizeof(vksdlshit->commandBuffers) / sizeof(VkCommandBuffer), vksdlshit->commandBuffers);
	}

	// create new command buffers
	vksdlshit->commandBuffers = malloc(sizeof(VkCommandBuffer) * vksdlshit->imageCount);
	VkCommandBufferAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;;
	allocInfo.commandPool = vksdlshit->commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = vksdlshit->imageCount;
	if (vkAllocateCommandBuffers(vksdlshit->logicalDevice, &allocInfo, vksdlshit->commandBuffers) != VK_SUCCESS)
	{
		printf("Failed to allocate command buffers.\n");
		return 0;
	}

	return 1;
}
unsigned char recordCommands(vksdl_shit* vksdlshit)
{
	// begin info
	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	beginInfo.pInheritanceInfo = NULL;

	// render pass begin
	VkRenderPassBeginInfo renderPassBeginInfo;
	VkOffset2D offset = { 0, 0 };
	VkExtent2D extent = { WIDTH, HEIGHT };
	VkClearValue clearColor = { 0.05f, 0.05f, 0.05f, 1.0f };
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = vksdlshit->renderPass;
	renderPassBeginInfo.renderArea.extent = extent;
	renderPassBeginInfo.renderArea.offset = offset;
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearColor;

	for (unsigned char i = 0; i < vksdlshit->imageCount; ++i)
	{
		if (vkBeginCommandBuffer(vksdlshit->commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			printf("Failed to begin command buffer.\n");
			return 0;
		}

		renderPassBeginInfo.framebuffer = vksdlshit->framebuffers[i];
		vkCmdBeginRenderPass(vksdlshit->commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(vksdlshit->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vksdlshit->pipeline);
		vkCmdBindDescriptorSets(vksdlshit->commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vksdlshit->pipelineLayout, 0, 1, &vksdlshit->descriptorSet, 0, NULL);
		VkBuffer vbs[] = { vksdlshit->vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(vksdlshit->commandBuffers[i], 0, 1, vbs, offsets);
		vkCmdBindIndexBuffer(vksdlshit->commandBuffers[i], vksdlshit->indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(vksdlshit->commandBuffers[i], sizeof(indices) / sizeof(uint16_t), 1, 0, 0, 0);
		vkCmdEndRenderPass(vksdlshit->commandBuffers[i]);

		if (vkEndCommandBuffer(vksdlshit->commandBuffers[i]) != VK_SUCCESS)
		{
			printf("Failed to record command buffer.\n");
			return 0;
		}
	}

	return 1;
}
unsigned char createSemaphores(vksdl_shit* vksdlshit)
{
	VkSemaphoreCreateInfo semaphoreInfo = {
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		.flags = 0,
		.pNext = NULL
	};

	if (vkCreateSemaphore(vksdlshit->logicalDevice, &semaphoreInfo, NULL, &vksdlshit->iSemaphore) != VK_SUCCESS || vkCreateSemaphore(vksdlshit->logicalDevice, &semaphoreInfo, NULL, &vksdlshit->fSemaphore) != VK_SUCCESS)
	{
		printf("Failed to create semaphores.\n");
		return 0;
	}

	return 1;
}
unsigned char createBuffer(vksdl_shit* vksdlshit, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *bufferMemory)
{
	VkBufferCreateInfo bufferInfo;
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.flags = 0;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(vksdlshit->logicalDevice, &bufferInfo, NULL, buffer) != VK_SUCCESS)
	{
		printf("Failed to create buffer.\n");
		return 0;
	}

	VkMemoryRequirements memReqs;
	vkGetBufferMemoryRequirements(vksdlshit->logicalDevice, *buffer, &memReqs);

	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(vksdlshit->physicalDevice, &memProperties);

	// find appropriate memory type
	uint32_t memIndex;
	for (unsigned char i = 0; i < memProperties.memoryTypeCount; ++i)
	{
		if (memReqs.memoryTypeBits & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties)
			== properties)
		{
			memIndex = i;
			break;
		}
	}

	VkMemoryAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = NULL;
	allocInfo.allocationSize = memReqs.size;
	allocInfo.memoryTypeIndex = memIndex;

	if (vkAllocateMemory(vksdlshit->logicalDevice, &allocInfo, NULL, bufferMemory) != VK_SUCCESS)
	{
		printf("Failed to allocate buffer memory.\n");
		return 0;
	}
	
	vkBindBufferMemory(vksdlshit->logicalDevice, *buffer, *bufferMemory, 0);

	return 1;
}
void copyBuffer(vksdl_shit* vksdlshit, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.pNext = NULL;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = vksdlshit->commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(vksdlshit->logicalDevice, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo;
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	beginInfo.pInheritanceInfo = NULL;
	beginInfo.pNext = NULL;

	VkBufferCopy copyRegion;
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	submitInfo.pNext = NULL;
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.waitSemaphoreCount = 0;

	vkQueueSubmit(vksdlshit->queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(vksdlshit->queue);

	vkFreeCommandBuffers(vksdlshit->logicalDevice, vksdlshit->commandPool, 1, &commandBuffer);
}
void updateUniformBuffer(vksdl_shit* vksdlshit, transform *t)
{
	void* data;
	VkDeviceSize bufferSize = sizeof(transform);
	vkMapMemory(vksdlshit->logicalDevice, vksdlshit->uniformStagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, t, (size_t)bufferSize);
	vkUnmapMemory(vksdlshit->logicalDevice, vksdlshit->uniformStagingBufferMemory);

	copyBuffer(vksdlshit, vksdlshit->uniformStagingBuffer, vksdlshit->uniformBuffer, bufferSize);
}
void drawFrame(vksdl_shit* vksdlshit)
{
	uint32_t imageIndex;
	// acquire image from swap chain
	vkAcquireNextImageKHR(vksdlshit->logicalDevice, vksdlshit->swapChain, ULLONG_MAX, vksdlshit->iSemaphore, VK_NULL_HANDLE, &imageIndex);
	// draw to image
	VkSemaphore waitSemaphores[] = { vksdlshit->iSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { vksdlshit->fSemaphore };
	VkSubmitInfo submitInfo = {
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = waitSemaphores,
		.pWaitDstStageMask = waitStages,
		.commandBufferCount = 1,
		.signalSemaphoreCount = 1,
		.pSignalSemaphores = signalSemaphores,
		.pCommandBuffers = &vksdlshit->commandBuffers[imageIndex]
	};

	vkQueueSubmit(vksdlshit->queue, 1, &submitInfo, VK_NULL_HANDLE);

	// return to swap chain
	VkSwapchainKHR swapChains[] = { vksdlshit->swapChain };
	VkPresentInfoKHR presentInfo = {
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = signalSemaphores,
		.swapchainCount = 1,
		.pSwapchains = swapChains,
		.pResults = NULL,
		.pImageIndices = &imageIndex,
	};

	vkQueuePresentKHR(vksdlshit->queue, &presentInfo);
}
void changeWindowSize(vksdl_shit* vksdlshit)
{
	unsigned char i;
	// wait for shit to finish
	vkDeviceWaitIdle(vksdlshit->logicalDevice);

	// get new dimensions
	SDL_GetWindowSize(vksdlshit->window, &WIDTH, &HEIGHT);
	printf("%i %i\n", WIDTH, HEIGHT);
	
	// clean up old resources
	for (i = 0; i < vksdlshit->imageCount; ++i)
	{
		vkDestroyFramebuffer(vksdlshit->logicalDevice, vksdlshit->framebuffers[i], NULL);
	}
	free(vksdlshit->framebuffers);
	vksdlshit->framebuffers = NULL;
	printf("framebuffers destroyed\n");
	vkDestroyPipelineLayout(vksdlshit->logicalDevice, vksdlshit->pipelineLayout, NULL);
	vkDestroyPipeline(vksdlshit->logicalDevice, vksdlshit->pipeline, NULL);
	printf("pipeline destroyed\n");
	vkDestroyRenderPass(vksdlshit->logicalDevice, vksdlshit->renderPass, NULL);
	printf("render pass destroyed\n");
	for (i = 0; i < vksdlshit->imageCount; ++i)
	{
		vkDestroyImageView(vksdlshit->logicalDevice, vksdlshit->imageViews[i], NULL);
	}
	free(vksdlshit->imageViews);
	vksdlshit->imageViews = NULL;
	printf("image views destroyed\n");
	vkFreeCommandBuffers(vksdlshit->logicalDevice, vksdlshit->commandPool, vksdlshit->imageCount, vksdlshit->commandBuffers);
	free(vksdlshit->commandBuffers);
	vksdlshit->commandBuffers = NULL;
	printf("command buffers detroyed\n");
	for (i = 0; i < vksdlshit->imageCount; ++i)
	{
		vkDestroyImage(vksdlshit->logicalDevice, vksdlshit->images[i], NULL);
	}
	free(vksdlshit->images);
	vksdlshit->images = NULL;
	printf("images destroyed\n");
	// create new resources
	createSwapChain(vksdlshit);
	// printf("new swap chan created\n");

	SDL_Delay(5000);
}
void cleanVkSDL(vksdl_shit* vksdlshit)
{
	unsigned char i;
	
	// wait for shit to finish
	vkDeviceWaitIdle(vksdlshit->logicalDevice);

	vkDestroySwapchainKHR(vksdlshit->logicalDevice, vksdlshit->swapChain, NULL);

	/*
	vkDestroySemaphore(vksdlshit->logicalDevice, vksdlshit->fSemaphore, NULL);
	vkDestroySemaphore(vksdlshit->logicalDevice, vksdlshit->iSemaphore, NULL);
	vkDestroyCommandPool(vksdlshit->logicalDevice, vksdlshit->commandPool, NULL);
	for (i = 0; i < vksdlshit->imageCount; ++i)
	{
		vkDestroyFramebuffer(vksdlshit->logicalDevice, vksdlshit->framebuffers[i], NULL);
	}
	free(vksdlshit->framebuffers);
	vksdlshit->framebuffers = NULL;
	vkDestroyPipeline(vksdlshit->logicalDevice, vksdlshit->pipeline, NULL);
	vkDestroyRenderPass(vksdlshit->logicalDevice, vksdlshit->renderPass, NULL);
	for (i = 0; i < vksdlshit->imageCount; ++i)
	{
		vkDestroyImageView(vksdlshit->logicalDevice, vksdlshit->imageViews[i], NULL);
	}
	free(vksdlshit->imageViews);
	vksdlshit->imageViews = NULL;
	vkDestroyDevice(vksdlshit->logicalDevice, NULL);
	vkDestroySurfaceKHR(vksdlshit->instance, vksdlshit->surface, NULL);
	SDL_DestroyWindow(vksdlshit->window);
	SDL_Quit();
	vkDestroyInstance(vksdlshit->instance, NULL);
	*/

	printf("Clean up successful.\n");
}