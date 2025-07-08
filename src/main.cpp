#include "types.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ctime>
#include <iomanip>

void	debug(const std::string message)
{
	std::time_t	time = std::time(nullptr);
	std::tm		*localTime = std::localtime(&time);

	std::cout << "[" << std::put_time(localTime, "%H:%M:%S") << "] " << message << std::endl;
}

bool	initGLFW(GLFWwindow	*window)
{
	// GLFW: Initializes and configures
	if (!glfwInit())
	{
		debug("Failed to initialize GLFW");
		return (false);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// GLFW: Window creation
	window = glfwCreateWindow(1024, 768, "Voxel Game", NULL, NULL);
	if (!window)
	{
		debug("Failed to create the window");
		glfwTerminate();
		return (false);
	}
	glfwMakeContextCurrent(window);

	debug("Window created successfully");

	return (true);
}

i32	main()
{
	GLFWwindow	*window;

	if (!initGLFW(window))
		return (-1);

	return (0);
}
