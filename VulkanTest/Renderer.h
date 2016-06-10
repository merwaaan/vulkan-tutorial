#pragma once

#include <vector>
#include <vulkan/vulkan.h>

class Renderer
{
public:
	Renderer();
	~Renderer();

//private:
	void InitInstance();
	void DeInitInstance();

	void InitDevice();
	void DeInitDevice();

	void SetupDebug();
	void InitDebug();
	void DeInitDebug();

	VkInstance _instance = VK_NULL_HANDLE;
	VkPhysicalDevice _gpu = VK_NULL_HANDLE;
	VkDevice _device = VK_NULL_HANDLE;
	VkQueue _queue = VK_NULL_HANDLE;

	uint32_t _graphics_family_index = -1;

	std::vector<const char*> _instance_layers;
	std::vector<const char*> _instance_extensions;
	std::vector<const char*> _device_extensions;

	VkDebugReportCallbackCreateInfoEXT debug_callback_create_info = {};
	VkDebugReportCallbackEXT _debug_callback = VK_NULL_HANDLE;
};

