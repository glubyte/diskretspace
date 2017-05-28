#include "vk.h"

bool initVk(vk_shit* vkshit, sdl_shit* sdlshit)
{
	printf("Creating instance...\n");
	if (createInstance(vkshit))
		return 1;
	printf("Instance created.\n");
	printf("Creating surface...\n");
	if (createSurface(vkshit, sdlshit))
		return 1;
	printf("Surface created.\n");
	printf("Scanning GPU's...\n");
	if (createPhysicalDevice(vkshit))
		return 1;
	printf("GPU selected.\n");
	printf("Creating logical device...\n");
	if (createDevice(vkshit))
		return 1;
	printf("Logical device created.\n");
	printf("Creating swap chain...\n");
	if (createSwapChain(vkshit, sdlshit))
		return 1;
	printf("Swap chain created.\n");
	printf("Creating image views...\n");
	if (createImageViews(vkshit))
		return 1;
	printf("Image views created.\n");

	return 0;
}
bool createInstance(vk_shit* vkshit)
{
#if defined(VK_USE_PLATFORM_WIN32_KHR)
	const char* extensionNames[] = { VK_KHR_SURFACE_EXTENSION_NAME , VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	const char* extensionNames[] = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_XLIB_SURFACE_EXTENSION_NAME };
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
	const char* extensionNames[] = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME };
#elif defined(VK_USE_PLATFORM_MIR_KHR)
	const char* extensionNames[] = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_MIR_SURFACE_EXTENSION_NAME };
#endif

	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = NULL;
	appInfo.pApplicationName = "Diskretspace";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.pEngineName = "Konstrukteur";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceInfo;
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = NULL;
	instanceInfo.flags = 0;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledLayerCount = 0;
	instanceInfo.ppEnabledLayerNames = NULL;
	instanceInfo.enabledExtensionCount = 2;
	instanceInfo.ppEnabledExtensionNames = extensionNames;

	if(assessError(vkCreateInstance(&instanceInfo, NULL, &vkshit->instance)))
	{
		printf("Failed to create Vulkan instance.\n");
		return 1;
	}

	return 0;
}
bool createSurface(vk_shit* vkshit, sdl_shit* sdlshit)
{	
#if defined(VK_USE_PLATFORM_WIN32_KHR)
	VkWin32SurfaceCreateInfoKHR surfaceInfo;
	surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.pNext = NULL;
	surfaceInfo.flags = 0;
	surfaceInfo.hinstance = GetModuleHandle(NULL);
	surfaceInfo.hwnd = sdlshit->windowInfo.info.win.window;

	if (assessError(vkCreateWin32SurfaceKHR(vkshit->instance, &surfaceInfo, NULL, &vkshit->surface)))
	{
		printf("Failed to create Win32 surface.\n");
		return 1;
	}
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	VkXlibSurfaceCreateInfoKHR surfaceInfo;
	surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.pNext = NULL;
	surfaceInfo.flags = 0;
	surfaceInfo.dpy = sdlshit->windowInfo.info.X11.display;

	if (assessError(vkCreateXlibSurfaceKHR(vkshit->instance, &surfaceInfo, NULL, &vkshit->surface)))
	{
		printf("Failed to created Xlib surface.\n");
		return 1;
	}
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
	VkWaylandSurfaceCreateInfoKHR surfaceInfo;
	surfaceInfo.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.pNext = NULL;
	surfaceInfo.flags = 0;
	surfaceInfo.display = sdlshit->windowInfo.info.wl.display;
	surfaceInfo.surface = sdlshit->windowInfo.info.wl.surface;

	if (assessError(vkCreateWaylandSurfaceKHR(vkshit->instance, &surfaceInfo, NULL, &vkshit->surface)))
	{
		printf("Failed to created Wayland surface.\n");
		return 1;
	}
#elif defined(VK_USE_PLATFORM_MIR_KHR)
	VkMirSurfaceCreateInfoKHR surfaceInfo;
	surfaceInfo.sType = VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.pNext = NULL;
	surfaceInfo.flags = 0;
	surfaceInfo.connection = sdlshit->windowInfo.info.mir.connection;
	surfaceInfo.mirSurface = sdlshit->windowInfo.info.mir.surface;

	if (assessError(vkCreateMirSurfaceKHR(vkshit->instance, &surfaceInfo, NULL, &vkshit->surface)))
	{
		printf("Failed to created Mir surface.\n");
		return 1;
	}
#endif

	return 0;
}
bool createPhysicalDevice(vk_shit* vkshit)
{
	uint32_t deviceCount = 0;
	VkPhysicalDevice* physicalDevices;
	VkPhysicalDeviceProperties deviceProperties;
	uint32_t queueFamilyPropertyCount = 0;
	VkQueueFamilyProperties* queueFamilyProperties;
	VkBool32 supported;
	unsigned char i, j;

	// enumerate physical devices
	vkEnumeratePhysicalDevices(vkshit->instance, &deviceCount, NULL);
	if (deviceCount == 0)
	{
		printf("Failed to find GPU with Vulkan support.\n");
		return 1;
	}
	physicalDevices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
	vkEnumeratePhysicalDevices(vkshit->instance, &deviceCount, physicalDevices);

	// isolate compatible device and queue family
	for (i = 0; i < deviceCount; i++)
	{
		vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
		printf("Assessing %s...\n", deviceProperties.deviceName);

		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyPropertyCount, NULL);
			printf("Scanning queue families...\n");

			if (queueFamilyPropertyCount > 0)
			{
				queueFamilyProperties = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyPropertyCount);
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &queueFamilyPropertyCount, queueFamilyProperties);

				for (j = 0; j < queueFamilyPropertyCount; j++)
				{
					if (queueFamilyProperties[j].queueCount > 0 && queueFamilyProperties[j].queueFlags & VK_QUEUE_GRAPHICS_BIT &&
						queueFamilyProperties[j].queueFlags & VK_QUEUE_COMPUTE_BIT)
					{
						// check for surface support
						if (assessError(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevices[i], j, vkshit->surface, &supported)))
						{
							printf("Could not retrieve surface information for family %i..\n", j);
							continue;
						}
						if (supported == VK_TRUE)
						{
							vkshit->physicalDevice = physicalDevices[i];
							vkshit->queueFamilyIndex = j;

							printf("Family %i is supported.\n", j);
							printf("Compatible device isolated:\n%s\nDriver Version: %i\n", deviceProperties.deviceName, deviceProperties.driverVersion);

							free(queueFamilyProperties);
							queueFamilyProperties = NULL;
							free(physicalDevices);
							physicalDevices = NULL;
							return 0;
						}
						printf("Family %i supports compute shaders, but not the current window manager...\n", j);
						continue;
					}
					printf("Family %i does not support compute shaders...\n", j);
				}
				free(queueFamilyProperties);
				queueFamilyProperties = NULL;
			}
			printf("Ignoring %s: No supported queue families...\n", deviceProperties.deviceName);
			continue;
		}
		printf("Ignoring %s: Not a discrete GPU...\n", deviceProperties.deviceName);
	}
	// terminate program, no compatible devices found
	printf("Although there exists a Vulkan compatible GPU, it does not support the additional required features.\n");
	free(physicalDevices);
	physicalDevices = NULL;
	return 1;
}
bool createDevice(vk_shit* vkshit)
{
	const char *deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	float queuePriority = 1.0f;

	VkDeviceQueueCreateInfo queueInfo;
	queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueInfo.pNext = NULL;
	queueInfo.flags = 0;
	queueInfo.queueCount = 1;
	queueInfo.queueFamilyIndex = vkshit->queueFamilyIndex;
	queueInfo.pQueuePriorities = &queuePriority;

	VkDeviceCreateInfo deviceInfo;
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext = NULL;
	deviceInfo.flags = 0;
	deviceInfo.pEnabledFeatures = NULL;
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pQueueCreateInfos = &queueInfo;
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = NULL;
	deviceInfo.enabledExtensionCount = 1;
	deviceInfo.ppEnabledExtensionNames = deviceExtensions;
	
	if (assessError(vkCreateDevice(vkshit->physicalDevice, &deviceInfo, NULL, &vkshit->device)))
	{
		printf("Failed to create logical device.\n");
		return 1;
	}

	vkGetDeviceQueue(vkshit->device, vkshit->queueFamilyIndex, 0, &vkshit->queue);

	return 0;
}
bool createSwapChain(vk_shit* vkshit, sdl_shit* sdlshit)
{
	VkSwapchainCreateInfoKHR swapchainInfo;
	VkSurfaceCapabilitiesKHR capabilities;
	unsigned char i;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkshit->physicalDevice, vkshit->surface, &capabilities);

	// isolate image format and color space
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(vkshit->physicalDevice, vkshit->surface, &formatCount, NULL);
	if (formatCount > 0)
	{
		VkSurfaceFormatKHR* formats = malloc(sizeof(VkSurfaceFormatKHR) * formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(vkshit->physicalDevice, vkshit->surface, &formatCount, formats);

		// check for best case scenario
		if (formatCount == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
		{
			printf("Ideal surface format and color space selected.\n");
			vkshit->imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
			swapchainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		}
		else
		{
			for (i = 0; i < formatCount; i++)
			{
				if (formats[i].format == VK_FORMAT_B8G8R8A8_UNORM && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				{
					printf("Ideal surface format and color space selected.\n");
					vkshit->imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
					swapchainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
					i = 0;
					break;
				}
			}
			if (i)
			{
				printf("Could not locate ideal surface format and color space. Settling with sub-optimal specifications.\n");
				vkshit->imageFormat = formats[0].format;
				swapchainInfo.imageColorSpace = formats[0].colorSpace;
			}
		}
		free(formats);
		formats = NULL;
	}
	else 
	{
		printf("No supported surface formats.\n");
		return 0;
	}

	// isolate present mode
	uint32_t presentCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(vkshit->physicalDevice, vkshit->surface, &presentCount, NULL);
	if (presentCount > 0)
	{
		VkPresentModeKHR* presentModes = malloc(sizeof(VkPresentModeKHR) * presentCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(vkshit->physicalDevice, vkshit->surface, &presentCount, presentModes);

		for (i = 0; i < presentCount; i++)
		{
			if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				printf("Triple buffering support found.\n");
				swapchainInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				i = 0;
				break;
			}
		}
		if (i)
		{
			printf("Device does not support triple buffering. Settling with sub-optimal...\n");
			swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
		}

		free(presentModes);
		presentModes = NULL;
	}
	else
	{
		printf("No supported present modes.\n");
		return 0;
	}

	// handle swap chain length
	vkshit->imageCount = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && vkshit->imageCount > capabilities.maxImageCount)
	{
		vkshit->imageCount = capabilities.maxImageCount;
	}

	// assign image extent
	VkExtent2D extent = { sdlshit->width, sdlshit->height };

	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.pNext = NULL;
	swapchainInfo.flags = 0;
	swapchainInfo.surface = vkshit->surface;
	swapchainInfo.minImageCount = vkshit->imageCount;
	swapchainInfo.imageFormat = vkshit->imageFormat;
	swapchainInfo.imageExtent = extent;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainInfo.queueFamilyIndexCount = 0;
	swapchainInfo.pQueueFamilyIndices = NULL;
	swapchainInfo.preTransform = capabilities.currentTransform;
	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.clipped = VK_TRUE;
	swapchainInfo.oldSwapchain = NULL;

	if (assessError(vkCreateSwapchainKHR(vkshit->device, &swapchainInfo, NULL, &vkshit->swapChain)))
	{
		printf("Failed to create swap chain.\n");
		return 1;
	}

	// get swap chain image handles
	vkGetSwapchainImagesKHR(vkshit->device, vkshit->swapChain, &vkshit->imageCount, NULL);
	vkshit->images = malloc(sizeof(VkImage) * vkshit->imageCount);
	vkGetSwapchainImagesKHR(vkshit->device, vkshit->swapChain, &vkshit->imageCount, vkshit->images);

	return 0;
}
bool createImageViews(vk_shit* vkshit)
{
	vkshit->imageViews = malloc(sizeof(VkImageView) * vkshit->imageCount);

	VkImageViewCreateInfo imageViewInfo;
	imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewInfo.pNext = NULL;
	imageViewInfo.flags = 0;
	imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	imageViewInfo.format = vkshit->imageFormat;
	imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageViewInfo.subresourceRange.baseMipLevel = 0;
	imageViewInfo.subresourceRange.levelCount = 1;
	imageViewInfo.subresourceRange.baseArrayLayer = 0;
	imageViewInfo.subresourceRange.layerCount = 1;

	for (unsigned char i = 0; i < vkshit->imageCount; i++)
	{
		imageViewInfo.image = vkshit->images[i];

		if (assessError(vkCreateImageView(vkshit->device, &imageViewInfo, NULL, &vkshit->imageViews[i])))
		{
			printf("Failed to create image views.\n");
			return 1;
		}
	}

	return 0;
}
bool assessError(VkResult result)
{
	if (result == 0)
		return 0;
	else if (result == -9)
	{
		printf("Unable to locate compatible driver, organism.\n");
		printf("It is recommended that GPU drivers are updated.\n");
		return 1;
	}
}
bool resizeWindow(vk_shit* vkshit, sdl_shit* sdlshit)
{
	unsigned char i;

	printf("Resizing window...\n");

	// wait for idle
	vkDeviceWaitIdle(vkshit->device);

	// acquire new dimensions
	SDL_GetWindowSize(sdlshit->window, &sdlshit->width, &sdlshit->height);
	// clean dependent variables
	printf("Cleaning up resources...\n");
	printf("Destroying image views...\n");
	for (i = 0; i < vkshit->imageCount; i++)
	{
		vkDestroyImageView(vkshit->device, vkshit->imageViews[i], NULL);
	}
	free(vkshit->imageViews);
	vkshit->imageViews = NULL;
	printf("Image views destroyed.\n");
	printf("Destroying swap chain...\n");
	vkDestroySwapchainKHR(vkshit->device, vkshit->swapChain, NULL); // implicitly destroys VkImage
	free(vkshit->images); // memory is still allocated, needs to be freed
	vkshit->images = NULL;
	printf("Swap chain destroyed.\n");
	printf("Clean up complete.\n");
	// assign dependent variables
	printf("Generating new resources...\n");
	printf("Creating swap chain...\n");
	if (createSwapChain(vkshit, sdlshit))
		return 1;
	printf("Swap chain created.\n");
	printf("Creating image views...\n");
	if (createImageViews(vkshit))
		return 1;
	printf("Image views created.\n");
	printf("Window resize complete.\n");

	return 0;
}
void cleanVk(vk_shit* vkshit)
{
	unsigned char i;

	printf("Cleaning up Vulkan...\n");
	printf("Destroying image views...\n");
	for (i = 0; i < vkshit->imageCount; i++)
	{
		vkDestroyImageView(vkshit->device, vkshit->imageViews[i], NULL);
	}
	free(vkshit->imageViews);
	vkshit->imageViews = NULL;
	printf("Image views destroyed.\n");
	printf("Destroying swap chain...\n");
	vkDestroySwapchainKHR(vkshit->device, vkshit->swapChain, NULL); // implicitly destroys VkImage
	printf("Swap chain destroyed.\n");
	free(vkshit->images); // memory is still allocated, needs to be freed
	vkshit->images = NULL;
	printf("Destroying device...\n");
	vkDestroyDevice(vkshit->device, NULL);  // implicitly destroys VkQueue
	printf("Device destroyed.\n");
	printf("Destroying surface...\n");
	vkDestroySurfaceKHR(vkshit->instance, vkshit->surface, NULL);
	printf("Surface destroyed.\n");
	printf("Destroying instance...\n");
	vkDestroyInstance(vkshit->instance, NULL); // implicitly destroys VkPhysicalDevice
	printf("Instance destroyed.\n");

	printf("Vulkan clean up successful.\n");
}
