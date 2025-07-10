#include "onLoad.hpp"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

glm::vec3	cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3	cameraDir = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3	cameraAngle = glm::vec3(0.0f, 1.0f, 0.0f);

static bool	onLoad(GLFWwindow *&window, u32 (&shaders)[1], mesh_t (&meshes)[1])
{
	// --- Init Libraries ---
	if (!initGLFW(window) || !initGLAD())
		return (false);

	// --- Load Shaders ---
	if (!loadVoxelShader(shaders[0]))
	{
		glfwTerminate();
		return (false);
	}

	debug(SUCCESS);
	std::cout << "Shaders loaded successfully" << std::endl;

	// --- Load Meshes ---
	meshes[CUBE] = loadCubeMesh();

	debug(SUCCESS);
	std::cout << "Meshes loaded successfully" << std::endl;

	return (true);
}

static void	onUpdate(GLFWwindow *&window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += CAMERA_SPEED * cameraDir;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= CAMERA_SPEED * cameraDir;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraDir, cameraAngle)) * CAMERA_SPEED;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraDir, cameraAngle)) * CAMERA_SPEED;
}

static void	onRender(u32 (&shaders)[1], mesh_t (&meshes)[1])
{
	// Bind
	glUseProgram(shaders[0]);
	glBindVertexArray(meshes[CUBE].VAO);

	// Matrices
	glm::mat4	model = glm::mat4(1.0f);
	glm::mat4	view = glm::lookAt(cameraPos, cameraPos + cameraDir, cameraAngle);
	glm::mat4	proj = glm::perspective(glm::radians(45.0f), (float)1024 / 768, 0.1f, 100.0f);

	glUniformMatrix4fv(glGetUniformLocation(shaders[0], "uModel"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shaders[0], "uView"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaders[0], "uProj"), 1, GL_FALSE, glm::value_ptr(proj));
	
	// Draw
	glDrawElements(GL_TRIANGLES, meshes[CUBE].indexCount, GL_UNSIGNED_INT, 0);
}

i32	main()
{
	GLFWwindow	*window;
	u32			shaders[1];
	mesh_t		meshes[1];

	onLoad(window, shaders, meshes);

	// --- Main Loop ---
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		onUpdate(window);
		onRender(shaders, meshes);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return (0);
}
