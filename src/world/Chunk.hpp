#pragma once

# include "types.hpp"
# include "options.hpp"

# include "VoxelMesh.hpp"

# define VOXEL_INDEX(x, y, z)	(x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z))

struct	BitShiftChunk
{
	// [z z z z z z z z z z z z z z z z z z z z z z z z z z z z z z z z x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x]
	static u64	Pack(i32 x, i32 z)
	{
		return ((u64)((u32)(z)) << 32) | (u32)(x);
	}

	static void	Unpack(u64 data, i32 &x, i32 &z)
	{
		x = (i32)(data & 0xFFFFFFFF);
		z = (i32)(data >> 32);
	}
};

class	Chunk
{
	public:
		// --- Constructors ---
		Chunk(const i32 x, const i32 z);
		~Chunk();

		// --- Loader ---
		void	Init();

		// --- Setters & Getters ---
		u32		GetVoxel(u8 x, u8 y, u8 z) const;
		i32		GetChunkX() const;
		i32		GetChunkZ() const;

		void	SetNorthNeighbour(Chunk *chunk);
		void	SetSouthNeighbour(Chunk *chunk);
		void	SetEastNeighbour(Chunk *chunk);
		void	SetWestNeighbour(Chunk *chunk);

		Chunk	*GetNorthNeighbour() const;
		Chunk	*GetSouthNeighbour() const;
		Chunk	*GetEastNeighbour() const;
		Chunk	*GetWestNeighbour() const;

		// --- Methods ---
		u32		GetNeighborVoxel(i8 x, i8 y, i8 z) const;
		bool	IsSurrounded(u8 x, u8 y, u8 z) const;
		bool	IsFaceVisible(i8 x, i8 y, i8 z, u8 dir) const;

	private:
		const i32	_chunkX;
		const i32	_chunkZ;

		u32			_voxels[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH];

		Chunk	*_northNeighbour = nullptr;
		Chunk	*_southNeighbour = nullptr;
		Chunk	*_eastNeighbour = nullptr;
		Chunk	*_westNeighbour = nullptr;
};
