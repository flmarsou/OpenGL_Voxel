#include "Chunk.hpp"

// ========================================================================== //
//    Constructors                                                            //
// ========================================================================== //

Chunk::Chunk(const i32 x, const i32 y, const i32 z)
	:	_chunkX(x), _chunkY(y), _chunkZ(z)
{
	std::cout << INFO "Chunk " <<
		"X: " << _chunkX <<
		" | Y: " << _chunkY <<
		" | Z: " << _chunkZ <<
		" loaded" << std::endl;

	Init();
}

Chunk::~Chunk()
{
	std::cout << INFO "Chunk " <<
		"X: " << _chunkX <<
		" | Y: " << _chunkY <<
		" | Z: " << _chunkZ <<
		" unloaded" RESET << std::endl;
}

// ========================================================================== //
//    Init                                                                    //
// ========================================================================== //

void	Chunk::Init()
{
	for (u8 x = 0; x < CHUNK_WIDTH; x++)
		for (u8 y = 0; y < CHUNK_HEIGHT; y++)
			for (u8 z = 0; z < CHUNK_WIDTH; z++)
				this->_voxels[VOXEL_INDEX(x, y, z)] = Voxel::Pack(x, y, z);
}

// ========================================================================== //
//    Getters                                                                 //
// ========================================================================== //

u32	Chunk::GetVoxel(u8 x, u8 y, u8 z) const
{
	return (this->_voxels[VOXEL_INDEX(x, y, z)]);
}

i32	Chunk::GetChunkX() const
{
	return (this->_chunkX);
}

i32	Chunk::GetChunkY() const
{
	return (this->_chunkY);
}

i32	Chunk::GetChunkZ() const
{
	return (this->_chunkZ);
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

bool Chunk::IsSurrounded(u8 x, u8 y, u8 z) const
{
	// Check boundaries
	if (x == 0 || x == CHUNK_WIDTH - 1
		|| y == 0 || y == CHUNK_HEIGHT - 1
		|| z == 0 || z == CHUNK_WIDTH - 1)
		return (false);

	// Check all six neighbors
	return (GetVoxel(x - 1, y, z)	// Left
		&& GetVoxel(x + 1, y, z)	// Right
		&& GetVoxel(x, y - 1, z)	// Bottom
		&& GetVoxel(x, y + 1, z)	// Top
		&& GetVoxel(x, y, z - 1)	// Front
		&& GetVoxel(x, y, z + 1)	// Back
	);
}

bool	Chunk::IsFaceVisible(u8 x, u8 y, u8 z, u8 dir) const
{
	i8	nx = x;
	i8	ny = y;
	i8	nz = z;

	// Get neighbor coordinates
	switch (dir)
	{
		case (0): nx++; break ;
		case (1): nx--; break ;
		case (2): ny++; break ;
		case (3): ny--; break ;
		case (4): nz++; break ;
		case (5): nz--; break ;
	}

	// Check boundaries
	if (nx < 0 || nx >= CHUNK_WIDTH
		|| ny < 0 || ny >= CHUNK_HEIGHT
		|| nz < 0 || nz >= CHUNK_WIDTH)
		return (true);

	// Check neighbor
	return (GetVoxel(nx, ny, nz) == 0);
}
