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

VkDebugUtilsMessengerEXT callback;

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
		setupDebugCallback();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
		}
	}

	void cleanup() {
		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(instance, callback, nullptr);
		}

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
		/*	uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);*/
		auto extensions = getRequiredExtensions();
		createrInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createrInfo.ppEnabledExtensionNames = extensions.data();

	


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


	/*
	* ��Ϣ�ص���
	* �����Ƿ����ü��㣬�����������չ�б�
	* ����ֵ��VK_KHR_surface��VK_KHR_win32_surface��VK_EXT_debug_utils
	*/
	std::vector<const char*> getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		return extensions;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="messageSeverity">
	/// VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT�������Ϣ
	/// VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT����Դ����֮�����Ϣ
	///	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT��������Ϣ
	///	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT�����Ϸ��Ϳ�����ɱ����Ĳ�����Ϣ
	/// </ param>
	/// <param name="messageType">
	/// VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT��������һЩ��淶�������޹ص��¼�
	/// VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT��������Υ���淶�����������һ�����ܵĴ���
	///	VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT�������˿���Ӱ��Vulkan���ܵ���Ϊ
	/// < / param>
	/// <param name="pCallbackData">
	/// 
	/// </param>
	/// <param name="pUserData"></param>
	/// <returns></returns>
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) 
	{
		std::cerr << "Validation lauer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	void setupDebugCallback() {
		if (!enableValidationLayers)
		{
			return;
		}

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;

		if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS)
		{
			throw std::runtime_error("faild to set up debug callback!");

		}
	}

	VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func!=nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func!=nullptr)
		{
			func(instance, callback, pAllocator);
		}
	}

};



int main() {

	unsigned int mask2 = (1 << 5) | (1 << 3) | (1 << 1);
	unsigned int data = 0b10101101;
	unsigned int extractedData = data & mask2;

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