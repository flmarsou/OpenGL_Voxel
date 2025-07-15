#pragma once

# include "types.hpp"
# include "options.hpp"

# include "VoxelShader.hpp"

# define GLM_ENABLE_EXPERIMENTAL
# include <GLFW/glfw3.h>
# include <GLM/glm.hpp>
# include <GLM/gtc/matrix_transform.hpp>
# include <GLM/gtc/type_ptr.hpp>
# include <GLM/gtx/rotate_vector.hpp>
# include <GLM/gtx/vector_angle.hpp>

class	Camera
{
	public:
		void	Init(const u32 width, const u32 height, const glm::vec3 pos);

		void	Matrix(float nearPlane, float farPlane, u32 shader, const char *uniform);
		void	Input(GLFWwindow *window);

		glm::vec3	Position;
		glm::vec3	Direction = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3	Angle = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4	View;
		glm::mat4	Proj;

		float		width;
		float		height;
};
