#include "types.hpp"
#include "debug.hpp"
#include "mesh.hpp"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

static bool	initGLFW(GLFWwindow	*&window)
{
	// GLFW: Initializes and configures
	if (!glfwInit())
	{
		debug(ERROR, "Failed to initialize GLFW!");
		return (false);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	debug(SUCCESS, "GLFW initialized successfully");

	// GLFW: Window creation
	window = glfwCreateWindow(1024, 768, "Voxel Game", NULL, NULL);
	if (!window)
	{
		debug(ERROR, "Failed to create the window!");
		glfwTerminate();
		return (false);
	}
	glfwMakeContextCurrent(window);

	debug(SUCCESS, "Window created successfully!");

	return (true);
}

static bool	initGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		debug(ERROR, "Failed to initialize GLAD!");
		glfwTerminate();
		return (false);
	}

	debug(SUCCESS, "GLAD initialized successfully");

	return (true);
}

i32	main()
{
	GLFWwindow	*window;

	// --- Init Libraries ---
	if (!initGLFW(window) || !initGLAD())
		return (-1);

	// --- Load Meshes ---
	mesh_t	meshes[1];

	meshes[0] = loadCubeMesh();

	debug(SUCCESS, "Meshes loaded successfully!");

	// --- Main Loop ---
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
