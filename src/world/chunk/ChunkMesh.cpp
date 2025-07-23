#include "Chunk.hpp"

static u16	getFaceTexture(const u8 face, const u16 blockID);

/**
 * @brief Creates VAO, VBO, and EBO for the chunk.
 * 
 * Configures vertex attributes for positions, texture coordinates, and block ID.
 */
void	Chunk::GenerateBuffers()
{
	// VAO (Vertex Array Object)
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);

	// VBO (Vertex Buffer Object)
	glGenBuffers(1, &this->_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);

	// EBO (Element Buffer Object)
	glGenBuffers(1, &this->_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);

	// Send voxel's x, y, z, face, and texture ID
	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(u32), (void *)0);
	glEnableVertexAttribArray(0);

	// Unbind
	glBindVertexArray(0);
}

/**
 * @brief Builds the mesh for the chunk based on visible faces.
 * 
 * - Skip if block is empty (AIR_BLOCK)
 * 
 * - Skip if all faces are surrounded
 * 
 * - Skip if faces are covered
 */
void	Chunk::GenerateMesh()
{
	std::vector<u32>	vertices;
	std::vector<u32>	indices;
	u32					indexOffset = 0;

	for (u8 vx = 0; vx < CHUNK_WIDTH; vx++)
	for (u8 vy = 0; vy < CHUNK_HEIGHT; vy++)
	for (u8 vz = 0; vz < CHUNK_WIDTH; vz++)
	{
		u32	blockID = GetVoxel(vx, vy, vz);

		if (blockID == AIR_BLOCK || IsSurrounded(vx, vy, vz))
			continue ;

		for (u8 face = 0; face < 6; face++)
		{
			if (!IsFaceVisible(vx, vy, vz, face))
				continue ;

			for (u8 i = 0; i < 4; i++)
				vertices.push_back(BitShiftVoxel::Pack(vx, vy, vz, face, getFaceTexture(face, blockID)));

			indices.push_back(indexOffset + 0);
			indices.push_back(indexOffset + 1);
			indices.push_back(indexOffset + 2);
			indices.push_back(indexOffset + 2);
			indices.push_back(indexOffset + 3);
			indices.push_back(indexOffset + 0);

			indexOffset += 4;
		}
	}

	glBindVertexArray(this->_vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(u32), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);

	this->_indexCount = indices.size();

	glBindVertexArray(0);
}

// ========================================================================== //
//    Helper Functions                                                        //
// ========================================================================== //

/**
 * @brief Returns the texture index for a given block face.
 * @param face Face direction (0-5)
 * @param blockID Block ID
 * @return Texture index according to the block ID and face
 */
static u16	getFaceTexture(const u8 face, const u16 blockID)
{
	switch (blockID)
	{
		case (DIRT_BLOCK): return (DIRT_BLOCK_TEXTURE);
		case (GRASS_BLOCK):
			if (face == TOP) return (GRASS_BLOCK_TOP_TEXTURE);
			if (face == BOTTOM) return (DIRT_BLOCK_TEXTURE);
			return (GRASS_BLOCK_SIDE_TEXTURE);
		case (STONE_BLOCK): return (STONE_BLOCK_TEXTURE);
		case (SAND_BLOCK): return (SAND_BLOCK_TEXTURE);
		default: return (DEBUG_BLOCK_TEXTURE);
	}
}
