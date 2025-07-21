#include "Chunk.hpp"

static u32						getFaceTexture(const u8 face, const u32 blockID);
static std::array<float, 24>	getFaceVertices(const u8 vx, const u8 vy, const u8 vz, const u8 face, const u32 blockID);

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

	// Position
	glVertexAttribPointer(
		0,							// Layout
		3,							// Size
		GL_FLOAT,					// Type
		GL_FALSE,					// Normalized?
		6 * sizeof(float),			// Stride
		(void *)0					// Offset
	);
	glEnableVertexAttribArray(0);

	// Texture Position
	glVertexAttribPointer(
		1,							// Layout
		2,							// Size
		GL_FLOAT,					// Type
		GL_FALSE,					// Normalized?
		6 * sizeof(float),			// Stride
		(void *)(3 * sizeof(float))	// Offset
	);
	glEnableVertexAttribArray(1);

	// Block ID
	glVertexAttribPointer(
		2,							// Layout
		1,							// Size
		GL_FLOAT,					// Type
		GL_FALSE,					// Normalized?
		6 * sizeof(float),			// Stride
		(void *)(5 * sizeof(float))	// Offset
	);
	glEnableVertexAttribArray(2);

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
	std::vector<float>	vertices;
	std::vector<u32>	indices;
	u32					indexOffset = 0;

	for (u8 vx = 0; vx < CHUNK_WIDTH; vx++)
	for (u8 vy = 0; vy < CHUNK_HEIGHT; vy++)
	for (u8 vz = 0; vz < CHUNK_WIDTH; vz++)
	{
		u32	blockID = GetVoxelID(vx, vy, vz);

		if (blockID == AIR_BLOCK || IsSurrounded(vx, vy, vz))
			continue ;

		for (u8 face = 0; face < 6; face++)
		{
			if (!IsFaceVisible(vx, vy, vz, face))
				continue ;

			u32		textureIndex = getFaceTexture(face, blockID);
			auto	faceVert = getFaceVertices(vx, vy, vz, face, textureIndex);
			vertices.insert(vertices.end(), faceVert.begin(), faceVert.end());

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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);

	this->_indexCount = indices.size();

	glBindVertexArray(0);
}

/**
 * @brief Returns the texture index for a given block face.
 * @param face Face direction (0-5)
 * @param blockID Block ID
 * @return Texture index according to the block ID and face
 */
static u32	getFaceTexture(const u8 face, const u32 blockID)
{
	switch (blockID)
	{
		case (DIRT_BLOCK): return (DIRT_BLOCK_TEXTURE);
		case (GRASS_BLOCK):
			if (face == TOP) return (GRASS_BLOCK_TOP_TEXTURE);
			if (face == BOTTOM) return (DIRT_BLOCK_TEXTURE);
			return (GRASS_BLOCK_SIDE_TEXTURE);
		default: return (DEBUG_BLOCK_TEXTURE);
	}
}

// ========================================================================== //
//    Helper Functions                                                        //
// ========================================================================== //

/**
 * @brief Returns the 4 vertices (with UV and blockID) for a specific face of a block.
 * @param vx Voxel X position
 * @param vy Voxel Y position
 * @param vz Voxel Z position
 * @param face Face direction (0-5)
 * @param blockID Block ID
 * @return Array of 24 floats (3 pos, 2 uv, blockID)
 */
static std::array<float, 24>	getFaceVertices(const u8 vx, const u8 vy, const u8 vz, const u8 face, const u32 blockID)
{
	std::array<float, 24>	verts;

	const float	x = static_cast<float>(vx);
	const float	y = static_cast<float>(vy);
	const float	z = static_cast<float>(vz);

	switch (face)
	{
		case (RIGHT):
			verts = {
				x + 1, y + 0, z + 0, 0, 0, static_cast<float>(blockID),
				x + 1, y + 1, z + 0, 0, 1, static_cast<float>(blockID),
				x + 1, y + 1, z + 1, 1, 1, static_cast<float>(blockID),
				x + 1, y + 0, z + 1, 1, 0, static_cast<float>(blockID)
			};
			break ;

		case (LEFT):
			verts = {
				x + 0, y + 0, z + 1, 0, 0, static_cast<float>(blockID),
				x + 0, y + 1, z + 1, 0, 1, static_cast<float>(blockID),
				x + 0, y + 1, z + 0, 1, 1, static_cast<float>(blockID),
				x + 0, y + 0, z + 0, 1, 0, static_cast<float>(blockID)
			};
			break ;

		case (TOP):
			verts = {
				x + 0, y + 1, z + 0, 0, 0, static_cast<float>(blockID),
				x + 0, y + 1, z + 1, 0, 1, static_cast<float>(blockID),
				x + 1, y + 1, z + 1, 1, 1, static_cast<float>(blockID),
				x + 1, y + 1, z + 0, 1, 0, static_cast<float>(blockID)
			};
			break ;

		case (BOTTOM):
			verts = {
				x + 0, y + 0, z + 1, 0, 0, static_cast<float>(blockID),
				x + 0, y + 0, z + 0, 0, 1, static_cast<float>(blockID),
				x + 1, y + 0, z + 0, 1, 1, static_cast<float>(blockID),
				x + 1, y + 0, z + 1, 1, 0, static_cast<float>(blockID)
			};
			break ;

		case (BACK):
			verts = {
				x + 0, y + 0, z + 1, 0, 0, static_cast<float>(blockID),
				x + 1, y + 0, z + 1, 1, 0, static_cast<float>(blockID),
				x + 1, y + 1, z + 1, 1, 1, static_cast<float>(blockID),
				x + 0, y + 1, z + 1, 0, 1, static_cast<float>(blockID)
			};
			break ;

		case (FRONT):
			verts = {
				x + 1, y + 0, z + 0, 0, 0, static_cast<float>(blockID),
				x + 0, y + 0, z + 0, 1, 0, static_cast<float>(blockID),
				x + 0, y + 1, z + 0, 1, 1, static_cast<float>(blockID),
				x + 1, y + 1, z + 0, 0, 1, static_cast<float>(blockID)
			};
			break ;
	}

	return (verts);
}

/**
 * @brief Unbinds VAO, VBO, and EBO to prevent accidental modifications
 */
void	Chunk::UnbindMesh()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
