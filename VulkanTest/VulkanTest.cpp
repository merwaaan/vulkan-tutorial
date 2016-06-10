#include "Renderer.h"

int main()
{
	Renderer r;

	auto device = r._device;
	auto queue = r._queue;

	VkFence fence;
	VkFenceCreateInfo fence_create_info {};
	fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	vkCreateFence(device, &fence_create_info, VK_NULL_HANDLE, &fence);

	VkSemaphore semaphore;
	VkSemaphoreCreateInfo semaphore_create_info{};
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	vkCreateSemaphore(device, &semaphore_create_info, VK_NULL_HANDLE, &semaphore);

	VkCommandPoolCreateInfo pool_create_info{};
	pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	pool_create_info.queueFamilyIndex = r._graphics_family_index;
	pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VkCommandPool command_pool;
	vkCreateCommandPool(device, &pool_create_info, VK_NULL_HANDLE, &command_pool);

	VkCommandBuffer command_buffer[2];
	VkCommandBufferAllocateInfo command_buffer_allocate_info{};
	command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.commandPool = command_pool;
	command_buffer_allocate_info.commandBufferCount = 2;
	command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	vkAllocateCommandBuffers(device, &command_buffer_allocate_info, command_buffer);

	{
		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(command_buffer[1], &begin_info);

		VkViewport viewport{};
		vkCmdSetViewport(command_buffer[1], 0, 1, &viewport);

		vkEndCommandBuffer(command_buffer[1]);
	}
	{
		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(command_buffer[1], &begin_info);

		VkViewport viewport{};
		vkCmdSetViewport(command_buffer[1], 0, 1, &viewport);

		vkEndCommandBuffer(command_buffer[1]);
	}

	{
		VkSubmitInfo submit_info{};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer[0];
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = &semaphore;

		vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE);
	}
	{
		VkPipelineStageFlags flags[]{
				VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
		};

		VkSubmitInfo submit_info{};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &command_buffer[1];
		submit_info.waitSemaphoreCount = 1;
		submit_info.pWaitSemaphores = &semaphore;
		submit_info.pWaitDstStageMask = flags;

		vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE);
	}

	//vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
	vkQueueWaitIdle(queue);

	vkDestroyCommandPool(device, command_pool, VK_NULL_HANDLE);
	vkDestroyFence(device, fence, VK_NULL_HANDLE);
	vkDestroySemaphore(device, semaphore, VK_NULL_HANDLE);

	return 0;
}

