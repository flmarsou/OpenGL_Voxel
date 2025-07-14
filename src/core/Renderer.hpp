#pragma once

# include "types.hpp"
# include "options.hpp"

# include "VoxelMesh.hpp"
# include "VoxelShader.hpp"
# include "Camera.hpp"
# include "Chunk.hpp"

# include <iostream>
# include <vector>

class	Renderer
{
	public:
		void	Init();

		void	Render(GLFWwindow *win);

		void	PrintStatistics();
		void	Cleanup();

	private:
		VoxelMesh				_voxelMesh;
		VoxelShader				_voxelShader;
		Camera					_camera;
		std::vector<Chunk *>	_chunks;

		u32	_triangleCount;
		u32	_vertexCount;
};
