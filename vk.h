#pragma once
// WSI selection (better to create separate vk.o for each kernel and window library and build on each for new releases)
#define VK_USE_PLATFORM_WIN32_KHR
// #define VK_USE_PLATFORM_XLIB_KHR
// #define VK_USE_PLATFORM_WAYLAND_KHR
// #define VK_USE_PLATFORM_MIR_KHR

// standard
#include <stdio.h>
#include <stdbool.h>
// external
#include <vulkan\vulkan.h>
// local
#include "sdl.h"

// stack handles
typedef struct {
	VkInstance instance;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice;
	uint32_t queueFamilyIndex;
	VkDevice device;
	VkQueue queue;
	uint32_t imageCount;
	VkFormat imageFormat;
	VkSwapchainKHR swapChain;
	VkImage* images;
	VkImageView* imageViews;
} vk_shit;

// prototypes
bool initVk(vk_shit* vkshit, sdl_shit* sdlshit);
bool createInstance(vk_shit* vkshit);
bool createSurface(vk_shit* vkshit, sdl_shit* sdlshit);
bool createPhysicalDevice(vk_shit* vkshit);
bool createDevice(vk_shit* vkshit);
bool createSwapChain(vk_shit* vkshit, sdl_shit* sdlshit);
bool createImageViews(vk_shit* vkshit);
bool assessError(VkResult result);
bool resizeWindow(vk_shit* vkshit, sdl_shit* sdlshit);
void cleanVk(vk_shit* vkshit);