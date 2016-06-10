#include "Shared.h"

#include <assert.h>
#include <iostream>

void ErrorCheck(VkResult result)
{
	if (result < 0) {
		switch (result)
		{
		case VK_NOT_READY:
			std::cout << "VK_NOT_READY" << std::endl;
			break;
		case VK_TIMEOUT:
			std::cout << "VK_TIMEOUT" << std::endl;
			break;
		case VK_EVENT_SET:
			std::cout << "VK_EVENT_SET" << std::endl;
			break;
		case VK_EVENT_RESET:
			std::cout << "VK_EVENT_RESET" << std::endl;
			break;
		case VK_INCOMPLETE:
			std::cout << "VK_INCOMPLETE" << std::endl;
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			std::cout << "VK_ERROR_OUT_OF_HOST_MEMORY" << std::endl;
			break;
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			std::cout << "VK_ERROR_OUT_OF_DEVICE_MEMORY" << std::endl;
			break;
		case VK_ERROR_INITIALIZATION_FAILED:
			std::cout << "VK_ERROR_INITIALIZATION_FAILED" << std::endl;
			break;
		case VK_ERROR_DEVICE_LOST:
			std::cout << "VK_ERROR_DEVICE_LOST" << std::endl;
			break;
		case VK_ERROR_MEMORY_MAP_FAILED:
			std::cout << "VK_ERROR_MEMORY_MAP_FAILED" << std::endl;
			break;
		case VK_ERROR_LAYER_NOT_PRESENT:
			std::cout << "VK_ERROR_LAYER_NOT_PRESENT" << std::endl;
			break;
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			std::cout << "VK_ERROR_EXTENSION_NOT_PRESENT" << std::endl;
			break;
		case VK_ERROR_FEATURE_NOT_PRESENT:
			std::cout << "VK_ERROR_FEATURE_NOT_PRESENT" << std::endl;
			break;
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			std::cout << "VK_ERROR_INCOMPATIBLE_DRIVER" << std::endl;
			break;
		case VK_ERROR_TOO_MANY_OBJECTS:
			std::cout << "VK_ERROR_TOO_MANY_OBJECTS" << std::endl;
			break;
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			std::cout << "VK_ERROR_FORMAT_NOT_SUPPORTE" << std::endl;
			break;
		case VK_ERROR_SURFACE_LOST_KHR:
			std::cout << "VK_ERROR_SURFACE_LOST_KHR" << std::endl;
			break;
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			std::cout << "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" << std::endl;
			break;
		case VK_SUBOPTIMAL_KHR:
			std::cout << "VK_SUBOPTIMAL_KHR" << std::endl;
			break;
		case VK_ERROR_OUT_OF_DATE_KHR:
			std::cout << "VK_ERROR_OUT_OF_DATE_KH" << std::endl;
			break;
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			std::cout << "VK_ERROR_INCOMPATIBLE_DISPLAY_KH" << std::endl;
			break;
		case VK_ERROR_VALIDATION_FAILED_EXT:
			std::cout << "VK_ERROR_VALIDATION_FAILED_EXT" << std::endl;
			break;
		case VK_ERROR_INVALID_SHADER_NV:
			std::cout << "VK_ERROR_INVALID_SHADER_N" << std::endl;
			break;
		default:
			break;
		}

		assert(0 && "Vulkan runtime error");
	}
}
