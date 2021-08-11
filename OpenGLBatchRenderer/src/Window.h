#pragma once
#include <GLFW/glfw3.h>
#include "glad/glad.h"
#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
class Window
{
public:
	Window(unsigned int width, unsigned int height, const char* title)
	{
		this->width = width;
		this->height = height;
		this->title = title;

		window = NULL;
	}
	inline bool Init()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



		window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		return true;
	}
	~Window()
	{
		glfwTerminate();
	}

	GLFWwindow* GetGLFWWindow() { return window; }

	const char* GetTitle() { return title; }
private:
	unsigned int width, height;
	const char* title;
	GLFWwindow* window;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}