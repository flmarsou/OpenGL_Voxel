#include "Renderer.hpp"

// ========================================================================== //
//    Init                                                                    //
// ========================================================================== //

void	Renderer::Init()
{
	this->_voxelMesh.Init();
	this->_voxelShader.Init(VOXEL_VERTEX_PATH, VOXEL_FRAGMENT_PATH);
	this->_camera.Init(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(16, 16, 100));

	this->_chunks.push_back(new Chunk(0, 0, 0));
}

void	Renderer::Render(GLFWwindow *win)
{
	this->_triangleCount = 0;
	this->_vertexCount = 0;

	this->_voxelShader.Use();

	this->_camera.Input(win);
	this->_camera.Matrix(0.1f, 500.0f, this->_voxelShader.program, "uCamera");

	this->_voxelMesh.Bind();

	for (i32 chunkCount = 0; chunkCount < this->_chunks.size(); chunkCount++)
	{
		i32	baseX = this->_chunks[chunkCount]->GetChunkX() * CHUNK_WIDTH;
		i32	baseY = this->_chunks[chunkCount]->GetChunkY() * CHUNK_HEIGHT;
		i32	baseZ = this->_chunks[chunkCount]->GetChunkZ() * CHUNK_WIDTH;

		for (i32 x = 0; x < CHUNK_WIDTH; x++)
		{
			for (i32 y = 0; y < CHUNK_HEIGHT; y++)
			{
				for (i32 z = 0; z < CHUNK_WIDTH; z++)
				{
					// Skip rendering if block is hidden
					// Reduce ~85% of total triangles
					// 393216 to 58416
					if (this->_chunks[chunkCount]->IsSurrounded(x, y, z))
						continue ;

					u32	voxel = this->_chunks[chunkCount]->GetVoxel(x, y, z);

					u8	vx;
					u8	vy;
					u8	vz;
					Voxel::Unpack(voxel, vx, vy, vz);

					glm::mat4	model = glm::translate(glm::mat4(1.0f), glm::vec3(baseX + vx, baseY + vy, baseZ + vz));

					glUniformMatrix4fv(glGetUniformLocation(this->_voxelShader.program, "uModel"), 1, GL_FALSE, glm::value_ptr(model));

					// Skip render if face is hidden
					// Reduce ~97% of total triangles
					// 58416 to 10494
					for (i8 face = 0; face < 6; face++)
					{
						if (!this->_chunks[chunkCount]->IsFaceVisible(x, y, z, face))
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

	for (i32 chunkCount = 0; chunkCount < this->_chunks.size(); chunkCount++)
		delete this->_chunks[chunkCount];
}
