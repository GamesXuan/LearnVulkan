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
		//��ֹGLFW�Զ�����OpenGL������
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
		Ӧ�ó�����Ϣ
		*/
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		/*
		����vulkan����������Ҫʹ�õ�ȫ����չ��У��㡣
		ȫ��ָ������Ӧ�ó�����Ч����������һ���豸��Ч��
		*/
		VkInstanceCreateInfo createrInfo = {};
		createrInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createrInfo.pApplicationInfo = &appInfo;
		/*
		ָ����Ҫ��ȫ����չ���細��ϵͳ��������չ��
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
	


		//����Vulkanʵ��
		if (vkCreateInstance(&createrInfo, nullptr, &instance) !=VK_SUCCESS)
		{
			throw std::runtime_error("failed to creater instance!");
		}


		/*
		* available��չ֧�֡�
		*/
		uint32_t instanceExtensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr);

		//�����������洢��չ��Ϣ
		std::vector<VkExtensionProperties> instanceExtensions(instanceExtensionCount);

		//��ȡ������չ��Ϣ
		vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, instanceExtensions.data());
		
		std::cout << "available extensions:" << std::endl;
		for (const auto& extension:instanceExtensions)
		{
			std::cout << "\t" << extension.extensionName << std::endl;
		}
	
	}

	/*
	* �������п��õ�У���
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