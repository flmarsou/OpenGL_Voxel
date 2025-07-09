#include "onLoad.hpp"

i32	main()
{
	// ====== On Load ====== //
	GLFWwindow	*window;
	u32			shaders[1];
	mesh_t		meshes[1];

	// --- Init Libraries ---
	if (!initGLFW(window) || !initGLAD())
		return (-1);

	// --- Load Shaders ---
	if (!loadVoxelShader(shaders[0]))
	{
		glfwTerminate();
		return (-1);
	}

	debug(SUCCESS);
	std::cout << "Shaders loaded successfully" << std::endl;

	// --- Load Meshes ---
	meshes[CUBE] = loadCubeMesh();

	debug(SUCCESS);
	std::cout << "Meshes loaded successfully" << std::endl;

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
