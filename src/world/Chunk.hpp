#pragma once

# include "types.hpp"
# include "options.hpp"

# include "VoxelMesh.hpp"

# define VOXEL_INDEX(x, y, z)	(x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z))

class	Chunk
{
	public:
		Chunk(const i32 x, const i32 y, const i32 z);
		~Chunk();

		void	Init();

		u32		GetVoxel(u8 x, u8 y, u8 z) const;
		i32		GetChunkX() const;
		i32		GetChunkY() const;
		i32		GetChunkZ() const;

		bool	IsSurrounded(u8 x, u8 y, u8 z) const;
		bool	IsFaceVisible(i8 x, i8 y, i8 z, u8 dir) const;

	private:
		const i32	_chunkX;
		const i32	_chunkY;
		const i32	_chunkZ;

		u32	_voxels[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH];
};
