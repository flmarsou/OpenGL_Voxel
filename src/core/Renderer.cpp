#include "Renderer.hpp"

// ========================================================================== //
//    Init                                                                    //
// ========================================================================== //

void	Renderer::Init()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// --- Shaders ---
	this->_voxelShader.Init(VOXEL_VERTEX_PATH, VOXEL_FRAGMENT_PATH);
	this->_voxelShader.Use();

	// --- Textures ---
	const std::vector<const char *>	textures = {
		DIRT_BLOCK_PATH,
		GRASS_BLOCK_TOP_PATH,
		GRASS_BLOCK_SIDE_PATH,
		STONE_BLOCK_PATH,
		SAND_BLOCK_PATH,
		DEBUG_BLOCK_PATH
	};
	this->_texture.LoadArray(textures);
	this->_texture.Bind();

	// --- Camera ---
	this->_camera.Init(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(16, 100, 100));

	this->_world.Load(0, 0);
}

// ========================================================================== //
//    Render Loop                                                             //
// ========================================================================== //

void	Renderer::Render(GLFWwindow *win)
{
	// --- Camera ---
	this->_camera.Input(win);
	this->_camera.Matrix(this->_voxelShader.program, "uCamera");
	this->_frustum.ExtractPlanes(this->_camera.Proj * this->_camera.View);

	// --- Chunk Reloading ---
	// static float	prevX = roundf(this->_camera.Position.x);
	// static float	prevZ = roundf(this->_camera.Position.z);
	// i32				save[2];

	// if (this->_camera.Position.x < 0)
	// 	save[0] = static_cast<i32>(((this->_camera.Position.x) / CHUNK_WIDTH) - 1);
	// else
	// 	save[0] = static_cast<i32>(this->_camera.Position.x) / CHUNK_WIDTH;
	// if (this->_camera.Position.z < 0)
	// 	save[1] = static_cast<i32>(((this->_camera.Position.z) / CHUNK_WIDTH) - 1);
	// else
	// 	save[1] = static_cast<i32>(this->_camera.Position.z) / CHUNK_WIDTH;

	// // West or East
	// if (prevX != save[0])
	// {
	// 	prevX = save[0];
	// 	this->_world.Reload(save[0], save[1]);
	// }
	// // North or South
	// else if (prevZ != save[1])
	// {
	// 	prevZ = save[1];
	// 	this->_world.Reload(save[0], save[1]);
	// }

	// --- Chunks Rendering ---
	for (auto &chunkPair : this->_world.chunks)
	{
		Chunk	*chunk = chunkPair.second;
		i32	chunkX = chunk->GetChunkX() * CHUNK_WIDTH;
		i32	chunkZ = chunk->GetChunkZ() * CHUNK_WIDTH;

		for (u8 chunkY = 0; chunkY < SUBCHUNK_AMOUNT; chunkY++)
		{
			SubChunk	*subChunk = chunk->subChunks[chunkY];
			if (!subChunk)
				continue ;

			glm::vec3	min = glm::vec3(chunkX, chunkY * CHUNK_HEIGHT, chunkZ);
			glm::vec3	max = min + glm::vec3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH);

			if (!this->_frustum.IsChunkVisible(min, max))
				continue ;

			glm::mat4 model = glm::translate(glm::mat4(1.0f), min);
			glUniformMatrix4fv(glGetUniformLocation(_voxelShader.program, "uModel"), 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(subChunk->GetVAO());
			glDrawElements(GL_TRIANGLES, subChunk->GetIndexCount(), GL_UNSIGNED_INT, 0);
		}
	}
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

void	Renderer::Cleanup()
{
	this->_voxelShader.Cleanup();
	this->_texture.Cleanup();

	for (auto &chunk : this->_world.chunks)
		delete chunk.second;
}
