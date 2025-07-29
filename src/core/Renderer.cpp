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
	this->_camera.Init(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(16, 100, 16));

	this->_world.Reload(0, 0);
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
	static i32	prevChunkX = 0;
	static i32	prevChunkZ = 0;

	i32			currentChunkX;
	i32			currentChunkZ;

	if (this->_camera.Position.x < 0)
		currentChunkX = static_cast<i32>(this->_camera.Position.x) / CHUNK_WIDTH - 1;
	else
		currentChunkX = static_cast<i32>(this->_camera.Position.x) / CHUNK_WIDTH;

	if (this->_camera.Position.z < 0)
		currentChunkZ = static_cast<i32>(this->_camera.Position.z) / CHUNK_WIDTH - 1;
	else
		currentChunkZ = static_cast<i32>(this->_camera.Position.z) / CHUNK_WIDTH;

	if (currentChunkX != prevChunkX || currentChunkZ != prevChunkZ)
	{
		prevChunkX = currentChunkX;
		prevChunkZ = currentChunkZ;
		this->_world.Reload(currentChunkX, currentChunkZ);
	}

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
