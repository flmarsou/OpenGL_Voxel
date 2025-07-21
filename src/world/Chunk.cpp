#include "Chunk.hpp"

// ========================================================================== //
//    Constructors                                                            //
// ========================================================================== //

Chunk::Chunk(const i32 cx, const i32 cz)
	:	_chunkX(cx), _chunkZ(cz)
{
	std::cout << INFO "Chunk " << "X: " << this->_chunkX << " | Z: " << this->_chunkZ << " loaded" RESET << std::endl;

	GenerateBuffers();
	GenerateVoxels();
}

Chunk::~Chunk()
{
	std::cout << INFO "Chunk " << "X: " << this->_chunkX << " | Z: " << this->_chunkZ << " unloaded" RESET << std::endl;

	// Unlink Neighbours
	if (GetNorthNeighbour())
		GetNorthNeighbour()->SetSouthNeighbour(nullptr);
	if (GetSouthNeighbour())
		GetSouthNeighbour()->SetNorthNeighbour(nullptr);
	if (GetEastNeighbour())
		GetEastNeighbour()->SetWestNeighbour(nullptr);
	if (GetWestNeighbour())
		GetWestNeighbour()->SetEastNeighbour(nullptr);

	UnbindMesh();
}

// ========================================================================== //
//    Setters & Getters                                                       //
// ========================================================================== //

u32		Chunk::GetVoxelID(u8 vx, u8 vy, u8 vz) const
{
	u32	blockID;
	BitShiftVoxel::UnpackBlockID(this->_voxels[VOXEL_INDEX(vx, vy, vz)], blockID);
	return (blockID);
}

i32		Chunk::GetChunkX() const { return (this->_chunkX); }
i32		Chunk::GetChunkZ() const { return (this->_chunkZ); }

void	Chunk::SetNorthNeighbour(Chunk *chunk) { this->_northNeighbour = chunk; }
void	Chunk::SetSouthNeighbour(Chunk *chunk) { this->_southNeighbour = chunk; }
void	Chunk::SetEastNeighbour(Chunk *chunk) { this->_eastNeighbour = chunk; }
void	Chunk::SetWestNeighbour(Chunk *chunk) { this->_westNeighbour = chunk; }

Chunk	*Chunk::GetNorthNeighbour() const { return (this->_northNeighbour); }
Chunk	*Chunk::GetSouthNeighbour() const { return (this->_southNeighbour); }
Chunk	*Chunk::GetEastNeighbour() const { return (this->_eastNeighbour); }
Chunk	*Chunk::GetWestNeighbour() const { return (this->_westNeighbour); }

u32		Chunk::GetVAO() const { return (this->_vao); }
u32		Chunk::GetIndexCount() const { return (this->_indexCount); }

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

/**
 * @brief Checks if neighboring voxel is solid
 * @param vx X Voxel
 * @param vy Y Voxel
 * @param vz Z Voxel
 * @return `True` if the neighbor is solid or out of bounds; `False` if air.
 */
bool	Chunk::IsNeighborVoxel(i8 vx, i8 vy, i8 vz) const
{
	// Inside current chunk
	if (vx >= 0 && vx < CHUNK_WIDTH && vy >= 0 && vy < CHUNK_HEIGHT && vz >= 0 && vz < CHUNK_WIDTH)
	{
		if (GetVoxelID(vx, vy, vz) == AIR_BLOCK)
			return (false);
		return (true);
	}

	// Outside current chunk
	if (vx < 0 && !this->_westNeighbour)
		return (true);
	if (vx < 0 && this->_westNeighbour)
		return (this->_westNeighbour->IsNeighborVoxel(vx + CHUNK_WIDTH, vy, vz));

	if (vx >= CHUNK_WIDTH && !this->_eastNeighbour)
		return(true);
	if (vx >= CHUNK_WIDTH && this->_eastNeighbour)
		return (this->_eastNeighbour->IsNeighborVoxel(vx - CHUNK_WIDTH, vy, vz));

	if (vz < 0 && !this->_northNeighbour)
		return (true);
	if (vz < 0 && this->_northNeighbour)
		return (this->_northNeighbour->IsNeighborVoxel(vx, vy, vz + CHUNK_WIDTH));

	if (vz >= CHUNK_WIDTH && !this->_southNeighbour)
		return (true);
	if (vz >= CHUNK_WIDTH && this->_southNeighbour)
		return (this->_southNeighbour->IsNeighborVoxel(vx, vy, vz - CHUNK_WIDTH));

	return (true);
}

/**
 * @brief Checks if a block is completely surrounded by other blocks.
 * @param vx X Voxel
 * @param vy Y Voxel
 * @param vz Z Voxel
 * @return `True` if surrounded on all 6 sides; `False` otherwise.
 */
bool Chunk::IsSurrounded(u8 vx, u8 vy, u8 vz) const
{
	// Check all six neighbors
	return ((!GetWestNeighbour() || IsNeighborVoxel(vx - 1, vy, vz))	// Left = West
		&& (!GetEastNeighbour() || IsNeighborVoxel(vx + 1, vy, vz))	// Right = East
		&& IsNeighborVoxel(vx, vy - 1, vz)								// Bottom
		&& IsNeighborVoxel(vx, vy + 1, vz)								// Top
		&& (!GetNorthNeighbour() || IsNeighborVoxel(vx, vy, vz - 1))	// Front = North
		&& (!GetSouthNeighbour() || IsNeighborVoxel(vx, vy, vz + 1))	// Back = South
	);
}

/**
 * @brief Check if a specific face of a block should be rendered.
 * @param vx X Voxel
 * @param vy Y Voxel
 * @param vz Z Voxel
 * @return `True` if face is visible; `False` otherwise.
 */
bool	Chunk::IsFaceVisible(i8 vx, i8 vy, i8 vz, u8 face) const
{
	// Get neighbor coordinates
	switch (face)
	{
		case (0): vx++; break ;
		case (1): vx--; break ;
		case (2): vy++; break ;
		case (3): vy--; break ;
		case (4): vz++; break ;
		case (5): vz--; break ;
	}

	// Check neighbor
	return (!IsNeighborVoxel(vx, vy, vz));
}
