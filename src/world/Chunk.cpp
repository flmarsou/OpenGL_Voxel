#include "Chunk.hpp"

// ========================================================================== //
//    Constructors                                                            //
// ========================================================================== //

Chunk::Chunk(const i32 x, const i32 z)
	:	_chunkX(x), _chunkZ(z)
{
	std::cout << INFO "Chunk " <<
		"X: " << _chunkX <<
		" | Z: " << _chunkZ <<
		" loaded" << std::endl;

	Init();
}

Chunk::~Chunk()
{
	std::cout << INFO "Chunk " <<
		"X: " << _chunkX <<
		" | Z: " << _chunkZ <<
		" unloaded" RESET << std::endl;
}

// ========================================================================== //
//    Init                                                                    //
// ========================================================================== //

void Chunk::Init()
{
	for (u8 x = 0; x < CHUNK_WIDTH; x++)
		for (u8 y = 0; y < CHUNK_HEIGHT; y++)
			for (u8 z = 0; z < CHUNK_WIDTH; z++)
				if (y == CHUNK_HEIGHT - 1 && x == CHUNK_WIDTH / 2 && z == CHUNK_WIDTH / 2)
					this->_voxels[VOXEL_INDEX(x, y, z)] = BitShiftVoxel::Pack(x, y, z, DEBUG_BLOCK);
				else
					this->_voxels[VOXEL_INDEX(x, y, z)] = BitShiftVoxel::Pack(x, y, z, DIRT_BLOCK);
}


// ========================================================================== //
//    Setters & Getters                                                       //
// ========================================================================== //

u32	Chunk::GetVoxel(u8 x, u8 y, u8 z) const { return (this->_voxels[VOXEL_INDEX(x, y, z)]); }

i32	Chunk::GetChunkX() const { return (this->_chunkX); }
i32	Chunk::GetChunkZ() const { return (this->_chunkZ); }

void	Chunk::SetNorthNeighbour(Chunk *chunk) { this->_northNeighbour = chunk; }
void	Chunk::SetSouthNeighbour(Chunk *chunk) { this->_southNeighbour = chunk; }
void	Chunk::SetEastNeighbour(Chunk *chunk) { this->_eastNeighbour = chunk; }
void	Chunk::SetWestNeighbour(Chunk *chunk) { this->_westNeighbour = chunk; }

Chunk	*Chunk::GetNorthNeighbour() const { return (this->_northNeighbour); }
Chunk	*Chunk::GetSouthNeighbour() const { return (this->_southNeighbour); }
Chunk	*Chunk::GetEastNeighbour() const { return (this->_eastNeighbour); }
Chunk	*Chunk::GetWestNeighbour() const { return (this->_westNeighbour); }

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

u32		Chunk::GetNeighborVoxel(i8 x, i8 y, i8 z) const
{
	// Inside current chunk
	if (x >= 0 && x < CHUNK_WIDTH
		&& y >= 0 && y < CHUNK_HEIGHT
		&& z >= 0 && z < CHUNK_WIDTH)
		return (GetVoxel(x, y, z));

	// Outside current chunk
	if (x < 0 && this->_westNeighbour)
		return (this->_westNeighbour->GetNeighborVoxel(x + CHUNK_WIDTH, y, z));

	if (x >= CHUNK_WIDTH && this->_eastNeighbour)
		return (this->_eastNeighbour->GetNeighborVoxel(x - CHUNK_WIDTH, y, z));

	if (z < 0 && this->_northNeighbour)
		return (this->_northNeighbour->GetNeighborVoxel(x, y, z + CHUNK_WIDTH));

	if (z >= CHUNK_WIDTH && this->_southNeighbour)
		return (this->_southNeighbour->GetNeighborVoxel(x, y, z - CHUNK_WIDTH));

	// Missing neighbor or vertical
	return (0);
}

bool Chunk::IsSurrounded(u8 x, u8 y, u8 z) const
{

	// Check top/bottom boundaries
	if (y == 0 || y == CHUNK_HEIGHT - 1)
		return (false);

	// Check all six neighbors
	return ((!this->GetWestNeighbour() || GetNeighborVoxel(x - 1, y, z))	// Left = West
		&& (!this->GetEastNeighbour() || GetNeighborVoxel(x + 1, y, z))	// Right = East
		&& GetNeighborVoxel(x, y - 1, z)	// Bottom
		&& GetNeighborVoxel(x, y + 1, z)	// Top
		&& (!this->GetNorthNeighbour() || GetNeighborVoxel(x, y, z - 1))	// Front = North
		&& (!this->GetSouthNeighbour() || GetNeighborVoxel(x, y, z + 1))	// Back = South
	);
}

bool	Chunk::IsFaceVisible(i8 x, i8 y, i8 z, u8 dir) const
{
	// Get neighbor coordinates
	switch (dir)
	{
		case (0): x++; break ;
		case (1): x--; break ;
		case (2): y++; break ;
		case (3): y--; break ;
		case (4): z++; break ;
		case (5): z--; break ;
	}

	// Check neighbor
	return (GetNeighborVoxel(x, y, z) == 0);
}
