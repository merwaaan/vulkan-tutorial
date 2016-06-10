#include "Renderer.h"
#include "Shared.h"

#include <assert.h>
#include <cstdlib>
#include <sstream>
#include <iostream>

#ifdef WIN32
#include <Windows.h>
#endif

Renderer::Renderer()
{
	SetupDebug();
	InitInstance();
	InitDebug();
	InitDevice();
}

Renderer::~Renderer()
{
	DeInitDevice();
	DeInitDebug();
	DeInitInstance();
}

void Renderer::InitInstance()
{
	VkApplicationInfo application_info{};
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.apiVersion = VK_MAKE_VERSION(1, 0, 13);
	application_info.pApplicationName = "Vulkan test";

	VkInstanceCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &application_info;
	create_info.enabledLayerCount = _instance_layers.size();
	create_info.ppEnabledLayerNames = _instance_layers.data();
	create_info.enabledExtensionCount = _instance_extensions.size();
	create_info.ppEnabledExtensionNames = _instance_extensions.data();
	create_info.pNext = &debug_callback_create_info;

	ErrorCheck(vkCreateInstance(&create_info, VK_NULL_HANDLE, &_instance));
}

void Renderer::DeInitInstance()
{
	vkDestroyInstance(_instance, VK_NULL_HANDLE);
	_instance = VK_NULL_HANDLE;
}

void Renderer::InitDevice()
{
	// Enumerate the GPUs and use the first available one
	uint32_t gpu_count = 0;
	vkEnumeratePhysicalDevices(_instance, &gpu_count, VK_NULL_HANDLE);
	std::vector<VkPhysicalDevice> gpu_list(gpu_count);
	vkEnumeratePhysicalDevices(_instance, &gpu_count, gpu_list.data());
	_gpu = gpu_list[0];

	// Enumerate the queue families and get one for which graphics are supported
	uint32_t family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &family_count, VK_NULL_HANDLE);
	std::vector<VkQueueFamilyProperties> family_property_list(family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &family_count, family_property_list.data());

	for (auto i = 0; i < family_count; ++i)
		if (family_property_list[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			_graphics_family_index = i;

	if (_graphics_family_index < 0) {
		assert(0 && "Cannot find queue family supporting graphics");
	std:exit(-1);
	}

	{
		// Enumerate available instance layers
		uint32_t layer_count;
		vkEnumerateInstanceLayerProperties(&layer_count, VK_NULL_HANDLE);
		std::vector<VkLayerProperties> layer_property_list(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, layer_property_list.data());
		std::cout << "Instance layers" << std::endl;
		for (auto &layer : layer_property_list)
			std::cout << "\t" << layer.layerName << ": " << layer.description << std::endl;
	}
	{
		// Enumerate available instance extensions
		uint32_t extension_count;
		vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &extension_count, VK_NULL_HANDLE);
		std::vector<VkExtensionProperties> extension_property_list(extension_count);
		vkEnumerateInstanceExtensionProperties(VK_NULL_HANDLE, &extension_count, extension_property_list.data());
		std::cout << "Instance extensions" << std::endl;
		for (auto &extension : extension_property_list)
			std::cout << "\t" << extension.extensionName << std::endl;
	}
	{
		// Enumerate available device layers
		uint32_t layer_count;
		vkEnumerateDeviceLayerProperties(_gpu, &layer_count, VK_NULL_HANDLE);
		std::vector<VkLayerProperties> layer_property_list(layer_count);
		vkEnumerateDeviceLayerProperties(_gpu, &layer_count, layer_property_list.data());
		std::cout << "Device layers" << std::endl;
		for (auto &layer : layer_property_list)
			std::cout << "\t" << layer.layerName << ": " << layer.description << std::endl;
	}

	float queue_priorities[]{ 1.0f };
	VkDeviceQueueCreateInfo queue_create_info{};
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex = _graphics_family_index;
	queue_create_info.queueCount = 1;
	queue_create_info.pQueuePriorities = queue_priorities;

	VkDeviceCreateInfo device_create_info{};
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.queueCreateInfoCount = 1;
	device_create_info.pQueueCreateInfos = &queue_create_info;
	device_create_info.enabledExtensionCount = _device_extensions.size();
	device_create_info.ppEnabledExtensionNames = _device_extensions.data();

	ErrorCheck(vkCreateDevice(_gpu, &device_create_info, VK_NULL_HANDLE, &_device));

	vkGetDeviceQueue(_device, _graphics_family_index, 0, &_queue);
}

void Renderer::DeInitDevice()
{
	vkDestroyDevice(_device, VK_NULL_HANDLE);
	_device = VK_NULL_HANDLE;
}

VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanDebugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT obj_type,
	uint64_t src_obj,
	size_t location,
	int32_t msg_code,
	const char* layer_prefix,
	const char* msg,
	void* user_data)
{
	std::ostringstream stream;
	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
		stream << "INFO";
	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
		stream << "WARNING";
	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
		stream << "PERFORMANCE";
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
		stream << "ERROR";
	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
		stream << "DEBUG";

	stream << " @[" << layer_prefix << "]: " << msg << std::endl;
	std::cout << stream.str();

#ifdef WIN32
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
		MessageBox(NULL, stream.str().c_str(), "Vulkan error!", 0);
#endif

	return false;
}

void Renderer::SetupDebug()
{
	debug_callback_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	debug_callback_create_info.pfnCallback = VulkanDebugCallback;
	debug_callback_create_info.flags =
		VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
		VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT |
		VK_DEBUG_REPORT_DEBUG_BIT_EXT;

	_instance_layers.push_back("VK_LAYER_LUNARG_core_validation");

	_instance_extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
}

PFN_vkCreateDebugReportCallbackEXT fvkCreateDebugReportCallbackEXT = VK_NULL_HANDLE;
PFN_vkDestroyDebugReportCallbackEXT fvkDestroyDebugReportCallbackEXT = VK_NULL_HANDLE;

void Renderer::InitDebug()
{
	fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(_instance, "vkCreateDebugReportCallbackEXT");
	fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(_instance, "vkDestroyDebugReportCallbackEXT");
	
	if (fvkCreateDebugReportCallbackEXT == VK_NULL_HANDLE || fvkDestroyDebugReportCallbackEXT == VK_NULL_HANDLE) {
		assert(0 && "Cannot fetch debug function pointers");
		std::exit(-1);
	}
	
	ErrorCheck(fvkCreateDebugReportCallbackEXT(_instance, &debug_callback_create_info, VK_NULL_HANDLE, &_debug_callback));
}

void Renderer::DeInitDebug()
{
	fvkDestroyDebugReportCallbackEXT(_instance, _debug_callback, VK_NULL_HANDLE);
}
