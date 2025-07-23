#include "SubChunk.hpp"

// ========================================================================== //
//    Constructors                                                            //
// ========================================================================== //

SubChunk::SubChunk(const Chunk *parent, const i32 subChunkY)
	:	_parent(parent), _subChunkY(subChunkY)
{
	std::cout << "-> SubChunk Y: " << this->_subChunkY << " loaded" RESET << std::endl;
}

SubChunk::~SubChunk()
{
	// Unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Delete
	glDeleteBuffers(1, &this->_vbo);
	glDeleteBuffers(1, &this->_ebo);
	glDeleteVertexArrays(1, &this->_vao);

	std::cout << "-> SubChunk Y: " << this->_subChunkY << " unloaded" RESET << std::endl;
}

// ========================================================================== //
//    Setters & Getters                                                       //
// ========================================================================== //

void	SubChunk::SetVoxel(const u8 voxelX, const u8 voxelY, const u8 voxelZ, const u16 blockID) { this->_voxels[VOXEL_INDEX(voxelX, voxelY, voxelZ)] = blockID; }
u16		SubChunk::GetVoxel(const u8 voxelX, const u8 voxelY, const u8 voxelZ) const { return (this->_voxels[VOXEL_INDEX(voxelX, voxelY, voxelZ)]); }

i8		SubChunk::GetSubChunkY() const { return (this->_subChunkY); }

u32		SubChunk::GetVAO() const { return (this->_vao); }
u32		SubChunk::GetIndexCount() const { return (this->_indexCount); }

// ========================================================================== //
//    Mesh                                                                    //
// ========================================================================== //

static u16	getFaceTexture(const u8 face, const u16 blockID);

/**
 * @brief Creates VAO, VBO, and EBO for the chunk.
 * 
 * Configures vertex attributes for positions, texture coordinates, and block ID.
 */
void	SubChunk::GenerateBuffers()
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
void	SubChunk::GenerateMesh()
{
	std::vector<u32>	vertices;
	std::vector<u32>	indices;
	u32					indexOffset = 0;

	for (u8 vx = 0; vx < CHUNK_WIDTH; vx++)
	for (u8 vy = 0; vy < CHUNK_HEIGHT; vy++)
	for (u8 vz = 0; vz < CHUNK_WIDTH; vz++)
	{
		if (GetVoxel(vx, vy, vz) == AIR_BLOCK)
			continue ;

		if (IsSurrounded(vx, vy, vz))
			continue ;

		for (u8 face = 0; face < 6; face++)
		{
			for (u8 i = 0; i < 4; i++)
				vertices.push_back(BitShiftVoxel::Pack(vx, vy, vz, face, getFaceTexture(face, GetVoxel(vx, vy, vz))));

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
//    Helper Methods                                                          //
// ========================================================================== //

bool	SubChunk::IsSurrounded(const u8 voxelX, const u8 voxelY, const u8 voxelZ) const
{
	// Inside Chunk
	if (voxelX > 0 && voxelX < CHUNK_WIDTH - 1
		&& voxelY > 0 && voxelY < CHUNK_HEIGHT - 1
		&& voxelZ > 0 && voxelZ < CHUNK_WIDTH - 1)
	{
		return (GetVoxel(voxelX + 1, voxelY, voxelZ) && GetVoxel(voxelX - 1, voxelY, voxelZ) &&
				GetVoxel(voxelX, voxelY + 1, voxelZ) && GetVoxel(voxelX, voxelY - 1, voxelZ) &&
				GetVoxel(voxelX, voxelY, voxelZ + 1) && GetVoxel(voxelX, voxelY, voxelZ - 1));
	}

	return (false);
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

// ========================================================================== //
//    Generation                                                              //
// ========================================================================== //

void	SubChunk::GenerateVoxels()
{
	// --- Noise Setup ---
	FastNoiseLite	baseNoise;
	baseNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	baseNoise.SetFrequency(0.002f);
	baseNoise.SetSeed(SEED);

	FastNoiseLite	detailNoise;
	detailNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	detailNoise.SetFrequency(0.01f);
	detailNoise.SetSeed(SEED + 1);

	FastNoiseLite	roughNoise;
	roughNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	roughNoise.SetFrequency(0.05f);
	roughNoise.SetSeed(SEED + 2);

	// --- Parameters ---
	static const i32	stoneDepth = 5;

	// --- World Gen ---
	for (u8 x = 0; x < CHUNK_WIDTH; x++)
	for (u8 z = 0; z < CHUNK_WIDTH; z++)
	{
		i32	worldX = this->_parent->GetChunkX() * CHUNK_WIDTH + x;
		i32	worldZ = this->_parent->GetChunkZ() * CHUNK_WIDTH + z;

		float	baseValue = baseNoise.GetNoise(static_cast<float>(worldX), static_cast<float>(worldZ));
		float	detailValue = detailNoise.GetNoise(static_cast<float>(worldX), static_cast<float>(worldZ));
		float	roughValue = roughNoise.GetNoise(static_cast<float>(worldX), static_cast<float>(worldZ));
		float	fusedValues = baseValue * 0.6f + detailValue * 0.3f + roughValue * 0.1f;

		i32		worldHeight = static_cast<i32>((fusedValues + 1) * 0.5f * (WORLD_HEIGHT - 1));
		i32		baseY = this->_subChunkY * CHUNK_HEIGHT;

		for (u8 y = 0; y < CHUNK_HEIGHT; y++)
		{
			i32	worldY = baseY + y;

			if (worldY == worldHeight)
				this->_voxels[VOXEL_INDEX(x, y, z)] = GRASS_BLOCK;
			else if (worldY <= worldHeight)
			{
				if (worldHeight - worldY > stoneDepth)
					this->_voxels[VOXEL_INDEX(x, y, z)] = STONE_BLOCK;
				else
					this->_voxels[VOXEL_INDEX(x, y, z)] = DIRT_BLOCK;
			}
			else
				this->_voxels[VOXEL_INDEX(x, y, z)] = AIR_BLOCK;
		}
	}
}
