#include "Renderer.hpp"

// ========================================================================== //
//    Init                                                                    //
// ========================================================================== //

void	Renderer::Init()
{
	glEnable(GL_CULL_FACE);

	// --- Voxel ---
	this->_voxelMesh.Init();
	this->_voxelMesh.Bind();

	// --- Shaders ---
	this->_voxelShader.Init(VOXEL_VERTEX_PATH, VOXEL_FRAGMENT_PATH);
	this->_voxelShader.Use();

	// --- Textures ---
	stbi_set_flip_vertically_on_load(true);
	this->_texture.Load(DEBUG_BLOCK_PATH, DEBUG_BLOCK);
	this->_texture.Load(DIRT_BLOCK_PATH, DIRT_BLOCK);

	// --- Camera ---
	this->_camera.Init(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(16, 16, 100));

	// --- World ---
	this->_world.load(0, 0);
}

// ========================================================================== //
//    Render Loop                                                             //
// ========================================================================== //

void	Renderer::Render(GLFWwindow *win)
{
	this->_triangleCount = 0;
	this->_vertexCount = 0;

	// --- Camera ---
	this->_camera.Input(win);
	this->_camera.Matrix(0.1f, 500.0f, this->_voxelShader.program, "uCamera");

	for (i32 chunkCount = 0; chunkCount < this->_world.chunks.size(); chunkCount++)
	{
		i32	baseX = this->_world.chunks[chunkCount]->GetChunkX() * CHUNK_WIDTH;
		i32	baseY = this->_world.chunks[chunkCount]->GetChunkY() * CHUNK_HEIGHT;
		i32	baseZ = this->_world.chunks[chunkCount]->GetChunkZ() * CHUNK_WIDTH;

		this->_frustum.ExtractPlanes(this->_camera.Proj * this->_camera.View);
	}

	// --- Chunks Loop ---
	for (i32 chunkCount = 0; chunkCount < this->_world.chunks.size(); chunkCount++)
	{
		i32	baseX = this->_world.chunks[chunkCount]->GetChunkX() * CHUNK_WIDTH;
		i32	baseY = this->_world.chunks[chunkCount]->GetChunkY() * CHUNK_HEIGHT;
		i32	baseZ = this->_world.chunks[chunkCount]->GetChunkZ() * CHUNK_WIDTH;

		glm::vec3	min = glm::vec3(baseX, baseY, baseZ);
		glm::vec3	max = min + glm::vec3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH);

		// Skip rendering if chunk is out of view
		if (!this->_frustum.IsChunkVisible(min, max))
			continue ;

		// --- Voxels Loop ---
		for (i32 x = 0; x < CHUNK_WIDTH; x++)
		{
			for (i32 y = 0; y < CHUNK_HEIGHT; y++)
			{
				for (i32 z = 0; z < CHUNK_WIDTH; z++)
				{
					// Skip rendering if block is hidden
					if (this->_world.chunks[chunkCount]->IsSurrounded(x, y, z))
						continue ;

					// Get voxel info
					u32	voxel = this->_world.chunks[chunkCount]->GetVoxel(x, y, z);

					u8	vx;
					u8	vy;
					u8	vz;
					u32	blockID;
					Voxel::Unpack(voxel, vx, vy, vz, blockID);

					switch (blockID)
					{
						case (DIRT_BLOCK): this->_texture.Bind(DIRT_BLOCK); break ;
						default: this->_texture.Bind(DEBUG_BLOCK); break ;
					}

					// Get voxel position
					glm::mat4	model = glm::translate(glm::mat4(1.0f), glm::vec3(baseX + vx, baseY + vy, baseZ + vz));

					glUniformMatrix4fv(glGetUniformLocation(this->_voxelShader.program, "uModel"), 1, GL_FALSE, glm::value_ptr(model));

					// Skip render if face is hidden
					for (i8 face = 0; face < 6; face++)
					{
						if (!this->_world.chunks[chunkCount]->IsFaceVisible(x, y, z, face))
							continue ;

						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(face * 6 * sizeof(u32)));
						this->_triangleCount += 2;
						this->_vertexCount += 6;
					}
				}
			}
		}
	}
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

void	Renderer::PrintStatistics()
{
	std::cout << INFO "Triangles: " << this->_triangleCount << std::endl;
	std::cout << INFO "Vertices: " << this->_vertexCount << std::endl;

	this->_triangleCount = 0;
	this->_vertexCount = 0;
}

void	Renderer::Cleanup()
{
	this->_voxelShader.Cleanup();
	this->_voxelMesh.Cleanup();
	this->_texture.Cleanup();

	for (i32 i = 0; i < this->_world.chunks.size(); i++)
		delete this->_world.chunks[i];
}
