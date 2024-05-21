#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <iostream>

#include <stdexcept>
#include <functional>

#include <cstdlib>
#include <vector>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KNRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication
{
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
protected:
private:
	VkInstance instance;

	GLFWwindow* window;


	void initWindow() {
		glfwInit();
		//阻止GLFW自动创建OpenGL上下文
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(WIDTH, HEIGHT, "HelloVulkan", nullptr, nullptr);
	}

	void initVulkan() {
	
		createInstance();
	
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}

	void cleanup() {

		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);
		glfwTerminate();
	}



	void createInstance() {
		if (enableValidationLayers && !checkValidationLayerSupper())
		{
			throw std::runtime_error("Validation layers requested ,but not available!");
		}


		/*
		应用程序信息
		*/
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		/*
		告诉vulkan驱动程序需要使用的全局扩展和校验层。
		全局指对整个应用程序都有效，不仅仅对一个设备有效。
		*/
		VkInstanceCreateInfo createrInfo = {};
		createrInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createrInfo.pApplicationInfo = &appInfo;
		/*
		指定需要的全局扩展。如窗口系统交互的扩展。
		*/
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::cout << "glfwExtensionCount = "<< glfwExtensionCount << std::endl;
		
		if (enableValidationLayers)
		{
			createrInfo.enabledExtensionCount = glfwExtensionCount;
			createrInfo.ppEnabledExtensionNames = glfwExtensions;

		}
		else
		{
			createrInfo.enabledLayerCount = 0;

		}
	


		//创造Vulkan实例
		if (vkCreateInstance(&createrInfo, nullptr, &instance) !=VK_SUCCESS)
		{
			throw std::runtime_error("failed to creater instance!");
		}


		/*
		* available扩展支持。
		*/
		uint32_t instanceExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr);

		//分配数组来存储扩展信息
		std::vector<VkExtensionProperties> instanceExtensions(instanceExtensionCount);

		//获取所有扩展信息
		vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, instanceExtensions.data());
		
		std::cout << "available extensions:" << std::endl;
		for (const auto& extension:instanceExtensions)
		{
			std::cout << "\t" << extension.extensionName << std::endl;
		}
	
	}

	/*
	* 请求所有可用的校验层
	*/
	bool checkValidationLayerSupper() {
		uint32_t layerCount;
	
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::cout << "layerCount = " << layerCount << std::endl;

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	
		for (const char* layername:validationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties:availableLayers)
			{
				std::cout << "Layername \t" << layerProperties.layerName << std::endl;
				if (strcmp(layername,layerProperties.layerName)==0)
				{
					layerFound = true;
					break;
				}
			}
			if (!layerFound)
			{
				return false;
			}
		}
		return false;
	}


};



int main() {
	HelloTriangleApplication app;

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}