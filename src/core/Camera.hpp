#pragma once

# include "config.hpp"

# include "VoxelShader.hpp"

# include <GLFW/glfw3.h>
# define GLM_ENABLE_EXPERIMENTAL
# include <GLM/glm.hpp>
# include <GLM/gtc/matrix_transform.hpp>
# include <GLM/gtc/type_ptr.hpp>
# include <GLM/gtx/rotate_vector.hpp>
# include <GLM/gtx/vector_angle.hpp>

enum	FacingDirection
{
	NORTH,
	SOUTH,
	EAST,
	WEST
};

class	Camera
{
	public:
		// --- Loader ---
		void	Init(const u32 width, const u32 height, const glm::vec3 pos);

		// --- Methods ---
		void	Matrix(float nearPlane, float farPlane, u32 shader, const char *uniform);
		void	Input(GLFWwindow *window);

		// --- Getters ---
		u8		GetFacingDirection() const;

		// --- Variables ---
		glm::vec3	Position;
		glm::vec3	Direction = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3	Angle = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4	View;
		glm::mat4	Proj;

		float		width;
		float		height;

	private:
		u8		_facing;
		float	_yaw = 0.0f;
		float	_pitch = -90.0f;
};
