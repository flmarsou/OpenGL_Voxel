#include "types.hpp"
#include "debug.hpp"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "load.cpp"

bool	initGLFW(GLFWwindow	*&window)
{
	// GLFW: Initializes and configures
	if (!glfwInit())
	{
		debug("Failed to initialize GLFW!", ERROR);
		return (false);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	debug("GLFW initialized successfully", SUCCESS);

	// GLFW: Window creation
	window = glfwCreateWindow(1024, 768, "Voxel Game", NULL, NULL);
	if (!window)
	{
		debug("Failed to create the window!", ERROR);
		glfwTerminate();
		return (false);
	}
	glfwMakeContextCurrent(window);

	debug("Window created successfully!", SUCCESS);

	return (true);
}

bool	initGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		debug("Failed to initialize GLAD!", ERROR);
		glfwTerminate();
		return (false);
	}

	debug("GLAD initialized successfully", SUCCESS);

	return (true);
}

i32	main()
{
	GLFWwindow	*window;

	if (!initGLFW(window) || !initGLAD())
		return (-1);

	const mesh_t	cubeMesh = loadCubeMesh();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.2f, 0.4f, 1.0f);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return (0);
}
