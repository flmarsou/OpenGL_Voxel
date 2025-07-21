#pragma once

# include "config.hpp"

# include "VoxelShader.hpp"
# include "Camera.hpp"
# include "Texture.hpp"
# include "World.hpp"

# include <vector>

struct	Frustum
{
	glm::vec4	planes[6];

	void	ExtractPlanes(const glm::mat4 &matrix)
	{
		// Left
		planes[0] = glm::vec4(
			matrix[0][3] + matrix[0][0],
			matrix[1][3] + matrix[1][0],
			matrix[2][3] + matrix[2][0],
			matrix[3][3] + matrix[3][0]
		);

		// Right
		planes[1] = glm::vec4(
			matrix[0][3] - matrix[0][0],
			matrix[1][3] - matrix[1][0],
			matrix[2][3] - matrix[2][0],
			matrix[3][3] - matrix[3][0]
		);

		// Bottom
		planes[2] = glm::vec4(
			matrix[0][3] + matrix[0][1],
			matrix[1][3] + matrix[1][1],
			matrix[2][3] + matrix[2][1],
			matrix[3][3] + matrix[3][1]
		);

		// Top
		planes[3] = glm::vec4(
			matrix[0][3] - matrix[0][1],
			matrix[1][3] - matrix[1][1],
			matrix[2][3] - matrix[2][1],
			matrix[3][3] - matrix[3][1]
		);

		// Near
		planes[4] = glm::vec4(
			matrix[0][3] + matrix[0][2],
			matrix[1][3] + matrix[1][2],
			matrix[2][3] + matrix[2][2],
			matrix[3][3] + matrix[3][2]
		);

		// Far
		planes[5] = glm::vec4(
			matrix[0][3] - matrix[0][2],
			matrix[1][3] - matrix[1][2],
			matrix[2][3] - matrix[2][2],
			matrix[3][3] - matrix[3][2]
		);

		for (u8 i = 0; i < 6; i++)
			planes[i] /= glm::length(glm::vec3(planes[i]));
	}

	bool	IsChunkVisible(const glm::vec3 &min, const glm::vec3 &max)
	{
		for (u8 i = 0; i < 6; i++)
		{
			glm::vec3	p = min;

			if (planes[i].x >= 0) p.x = max.x;
			if (planes[i].y >= 0) p.y = max.y;
			if (planes[i].z >= 0) p.z = max.z;

			if (glm::dot(glm::vec3(planes[i]), p) + planes[i].w < 0)
				return (false);
		}
		return (true);
	}
};

class	Renderer
{
	public:
		void	Init();

		void	Render(GLFWwindow *win);
		void	PrintStatistics();

		void	Cleanup();

	private:
		VoxelShader	_voxelShader;
		Texture		_texture;
		Camera		_camera;
		World		_world;

		Frustum		_frustum;
};
