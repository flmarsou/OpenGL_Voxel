#include "Renderer.hpp"

// ========================================================================== //
//    Init                                                                    //
// ========================================================================== //

void	Renderer::Init()
{
	glEnable(GL_CULL_FACE);
	// glEnable(GL_DEPTH_TEST);

	// --- Shaders ---
	this->_voxelShader.Init(VOXEL_VERTEX_PATH, VOXEL_FRAGMENT_PATH);
	this->_voxelShader.Use();

	// --- Textures ---
	const std::vector<const char *>	textures = {
		DEBUG_BLOCK_PATH,
		DIRT_BLOCK_PATH
	};
	this->_texture.LoadArray(textures);
	this->_texture.Bind();

	// --- Camera ---
	// this->_camera.Init(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(16.5f, 45, 16.5f));
	this->_camera.Init(WINDOW_WIDTH, WINDOW_HEIGHT, glm::vec3(static_cast<float>(CHUNK_WIDTH / 2), 45, static_cast<float>(CHUNK_WIDTH / 2)));

	// --- World ---
	this->_world.Load(0, 0);
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

	this->_frustum.ExtractPlanes(this->_camera.Proj * this->_camera.View);

	// ReloadChunk if necessary

	static float	ancientX = this->_camera.Position.x / CHUNK_WIDTH;
	static float	ancientZ = this->_camera.Position.z / CHUNK_WIDTH;

	// West or east
	if (ancientX != this->_camera.Position.x / CHUNK_WIDTH)
	{
		this->_world.Reload(this->_camera.Position.x / CHUNK_WIDTH, this->_camera.Position.z / CHUNK_WIDTH);
		ancientX = this->_camera.Position.x / CHUNK_WIDTH;
	}
	// North or South
	else if (ancientZ != this->_camera.Position.z / CHUNK_WIDTH)
	{
		this->_world.Reload(this->_camera.Position.x / CHUNK_WIDTH, this->_camera.Position.z / CHUNK_WIDTH);
		ancientZ = this->_camera.Position.z / CHUNK_WIDTH;
	}

	// --- Chunks Loop ---
	for (auto &chunk : this->_world.chunks)
	{
		i32	chunkX = chunk.second->GetChunkX() * CHUNK_WIDTH;
		i32	chunkZ = chunk.second->GetChunkZ() * CHUNK_WIDTH;

		glm::vec3	min = glm::vec3(chunkX, 0, chunkZ);
		glm::vec3	max = min + glm::vec3(CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH);

		// Skip rendering if chunk is out of view
		if (!this->_frustum.IsChunkVisible(min, max))
			continue ;

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunkX, 0, chunkZ));
		glUniformMatrix4fv(glGetUniformLocation(_voxelShader.program, "uModel"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(chunk.second->GetVAO());
		glDrawElements(GL_TRIANGLES, chunk.second->GetIndexCount(), GL_UNSIGNED_INT, 0);
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
	this->_texture.Cleanup();

	for (auto &chunk : this->_world.chunks)
		delete chunk.second;
}
