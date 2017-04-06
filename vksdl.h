#pragma once
// Enable WSI extensions
#if defined(__linux__)
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif

// standard
#include <stdio.h>
#include <limits.h>
// external 
#include <SDL.h>
#include <SDL_syswm.h>
#include <vulkan\vulkan.h>

// global allocation
typedef struct {
	SDL_Window* window;
	SDL_Event event;

	VkSurfaceKHR surface;
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkPhysicalDeviceProperties gpuProperties;
	VkDevice logicalDevice;
	VkQueue queue;
	uint32_t queueIndex;
	VkSurfaceFormatKHR surfaceFormat;
	VkExtent2D extent;
	VkSwapchainKHR swapChain;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline pipeline;
	uint32_t imageCount;
	VkImage* images;
	VkImageView* imageViews;
	VkFramebuffer* framebuffers;
	VkCommandPool commandPool;
	VkBuffer vertexBuffer; // vkDestroyBuffer
	VkDeviceMemory vertexBufferMemory; // vkFreeMemory
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	VkBuffer uniformStagingBuffer;
	VkDeviceMemory uniformStagingBufferMemory;
	VkBuffer uniformBuffer;
	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet;
	VkDeviceMemory uniformBufferMemory;
	VkCommandBuffer* commandBuffers;
	VkSemaphore iSemaphore;
	VkSemaphore fSemaphore;
	VkSubmitInfo submitInfo;
	VkPresentInfoKHR presentInfo;
} vksdl_shit;

typedef struct {
	float proj[4];
	float model[3];
	float view[3];
} transform;

int WIDTH;
int HEIGHT;

// prototype allocation
unsigned char initVkSDL(vksdl_shit* vksdlshit);
unsigned char createInstance(vksdl_shit* vksdlshit);
unsigned char initSDL(vksdl_shit* vksdlshit);
unsigned char createVulkanSurface(vksdl_shit* vksdlshit);
unsigned char pickPhysicalDevice(vksdl_shit* vksdlshit);
unsigned char createLogicalDevice(vksdl_shit* vksdlshit);
unsigned char createSwapChain(vksdl_shit* vksdlshit);
unsigned char createImageViews(vksdl_shit* vksdlshit);
unsigned char createRenderPass(vksdl_shit* vksdlshit);
unsigned char createFramebuffers(vksdl_shit* vksdlshit);
unsigned char createCommandPool(vksdl_shit* vksdlshit);
unsigned char createVertexBuffer(vksdl_shit* vksdlshit);
unsigned char createIndexBuffer(vksdl_shit* vksdlshit);
unsigned char createUniformBuffer(vksdl_shit* vksdlshit);
unsigned char createDescriptorPool(vksdl_shit* vksdlshit);
unsigned char createCommandBuffers(vksdl_shit* vksdlshit);
unsigned char recordCommands(vksdl_shit* vksdlshit);
unsigned char createSemaphores(vksdl_shit* vksdlshit);
unsigned char createBuffer(vksdl_shit* vksdlshit, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *bufferMemory);
void copyBuffer(vksdl_shit* vksdlshit, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
void updateUniformBuffer(vksdl_shit* vksdlshit, transform* t);
void drawFrame(vksdl_shit* vksdlshit);
void changeWindowSize(vksdl_shit* vksdlshit);
void cleanVkSDL(vksdl_shit* vksdlshit);