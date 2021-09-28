#include "stdafx.h"
#include "vulkan/vulkan.h"
#include <iostream>

#define CONVERT_BOOL(val)	printNumToBool(val)

#define ASSERT_VULKAN(val)\
	if(val != VK_SUCCESS)\
	{\
		__debugbreak();\
	}\

VkInstance instance;



void printNumToBool(uint32_t num)
{
	if (num == 0)
	{
		std::cout << "false";
		return;
	}
	std::cout << "true";
}

void printStats(VkPhysicalDevice& device)
{
	VkPhysicalDeviceProperties props;
	vkGetPhysicalDeviceProperties(device, &props);

	std::cout << "Device type:        " << props.deviceType << ", Device name: " << props.deviceName << ", ID: " << props.deviceID << std::endl;
	std::cout << "vendor ID:          " << props.vendorID << std::endl;

	uint32_t version = props.apiVersion;
	uint32_t driverVer = props.driverVersion;

	std::cout << "API version:        " << VK_VERSION_MAJOR(version) << "." << VK_VERSION_MINOR(version) << "." << VK_VERSION_PATCH(version) << std::endl;
	std::cout << "Driver version:     " << VK_VERSION_MAJOR(driverVer) << "." << VK_VERSION_MINOR(driverVer) << "." << VK_VERSION_PATCH(driverVer) << std::endl;
	std::cout << std::endl;

	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceFeatures(device, &features);

	std::cout << "Tessalation Shader: "; CONVERT_BOOL(features.tessellationShader); std::cout << std::endl;
	std::cout << "Geometry Shader:    "; CONVERT_BOOL(features.geometryShader); std::cout << std::endl;
	std::cout << "Multi viewport:     "; CONVERT_BOOL(features.multiViewport); std::cout << std::endl;

	VkPhysicalDeviceMemoryProperties memProp;
	vkGetPhysicalDeviceMemoryProperties(device, &memProp);
	std::cout << "Memory heap Count: " << memProp.memoryHeapCount << std::endl << std::endl;

	uint32_t amountQueueFam = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &amountQueueFam, NULL);
	VkQueueFamilyProperties *pFamProps = new VkQueueFamilyProperties[amountQueueFam];
	vkGetPhysicalDeviceQueueFamilyProperties(device, &amountQueueFam, pFamProps);

	std::cout << "Amount of queue families: " << std::endl << std::endl;

	for (int i = 0; i < amountQueueFam; i++)
	{
		std::cout << "Queue Family #" << i << std::endl;
		std::cout << "Queue family with >" << pFamProps[i].queueCount << "< Queues" << std::endl;

		std::cout << "VK_QUEUE_GRAPHICS_BIT        " << ((pFamProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) !=  0) << std::endl;
		std::cout << "VK_QUEUE_COMPUTE_BIT         " << ((pFamProps[i].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) << std::endl;
		std::cout << "VK_QUEUE_TRANSFER_BIT        " << ((pFamProps[i].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) << std::endl;
		std::cout << "VK_QUEUE_SPARSE_BINDING_BIT  " << ((pFamProps[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) != 0) << std::endl;

		
		std::cout << "Timestamp valid bits:            " << pFamProps[i].timestampValidBits << std::endl;
		std::cout << "Min image timestamp Granularity: " << std::endl;
		std::cout << "        Width:  " << pFamProps[i].minImageTransferGranularity.width << std::endl;
		std::cout << "        Height: " << pFamProps[i].minImageTransferGranularity.height << std::endl;
		std::cout << "        Depth:  " << pFamProps[i].minImageTransferGranularity.depth << std::endl;

		std::cout << std::endl;
	}

	std::cout << std::endl;
	delete[] pFamProps;
}

int main()
{
	//----------------------------- Create Vulkan instance ----------------------------------------------
	
	VkApplicationInfo appInfo;
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = NULL;
	appInfo.pApplicationName = "Gomu Gomu no mi (Vulkan x86)";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.pEngineName = "Gear5th Vulkan";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo instanceInfo;
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = NULL;
	instanceInfo.flags = 0;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledLayerCount = 0;
	instanceInfo.ppEnabledLayerNames = NULL;
	instanceInfo.enabledExtensionCount = 0;
	instanceInfo.ppEnabledExtensionNames = NULL;

	VkInstance instance;

	ASSERT_VULKAN(vkCreateInstance(&instanceInfo, NULL, &instance));

	//---------------------------------------- Access to GPU --------------------------------------------------
	//PhysicalDevice reference

	uint32_t amountDevices = 0;
	ASSERT_VULKAN(vkEnumeratePhysicalDevices(instance, &amountDevices, NULL)); //how many Cards are there

	VkPhysicalDevice* physicalDevices = new VkPhysicalDevice[amountDevices];
	ASSERT_VULKAN(vkEnumeratePhysicalDevices(instance, &amountDevices, physicalDevices)); //get physical reference to Graphic cards

	//GPU properties				-- auto graphic settings --					// GPU features
	for (uint32_t i = 0; i < amountDevices; i++)
	{
		printStats(physicalDevices[i]);
	}

	return 69;
}