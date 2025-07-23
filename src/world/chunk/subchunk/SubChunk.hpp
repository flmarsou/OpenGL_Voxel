#pragma once

# include "Chunk.hpp"

class	Subchunk
{
	public:
		Subchunk(const i32 cx, const i32 scy, const i32 cz);
		~Subchunk();

	private:
		const i32	_subChunkX;
		const i32	_subChunkY;
		const i32	_subChunkZ;

		u16			_voxels[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH];
};
