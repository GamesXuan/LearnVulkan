#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <iostream>

#include <stdexcept>
#include <functional>

#include <cstdlib>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;


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
		glfwDestroyWindow(window);
		glfwTerminate();
	}



	void createInstance() {
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
		std::cout << "glfwExtensionCount = " + glfwExtensionCount << std::endl;
		
		createrInfo.enabledExtensionCount = glfwExtensionCount;
		createrInfo.ppEnabledExtensionNames = glfwExtensions;

		/*
		* ָ��ȫ��У���
		*/
		createrInfo.enabledLayerCount = 0;

		//����Vulkanʵ��
		VkResult result = vkCreateInstance(&createrInfo, nullptr, &instance);

		if (result!=VK_SUCCESS)
		{
			throw std::runtime_error("failed to creater instance!");
		}
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