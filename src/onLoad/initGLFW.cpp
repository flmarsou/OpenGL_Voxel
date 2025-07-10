#include "onLoad.hpp"

bool	initGLFW(GLFWwindow	*&window)
{
	// GLFW: Initializes and configures
	if (!glfwInit())
	{
		debug(ERROR);
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return (false);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	debug(SUCCESS);
	std::cout << "GLFW initialized successfully" << std::endl;

	// GLFW: Window creation
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		debug(ERROR);
		std::cerr << "Failed to create the window" << std::endl;
		glfwTerminate();
		return (false);
	}
	glfwMakeContextCurrent(window);

	debug(SUCCESS);
	std::cout << "Window created successfully" << std::endl;

	return (true);
}
