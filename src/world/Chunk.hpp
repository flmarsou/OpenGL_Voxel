#pragma once

# include "config.hpp"

# include "SubChunk.hpp"

struct	BitShiftChunk
{
	static u64	Pack(i32 chunkX, i32 chunkZ)
	{
		return ((u64)((u32)(chunkX)) << 32) | (u32)(chunkZ);
	}

	static void	Unpack(u64 data, i32 &chunkX, i32 &chunkZ)
	{
		chunkX = (i32)(data & 0xFFFFFFFF);
		chunkZ = (i32)(data >> 32);
	}
};

class	SubChunk;

class	Chunk
{
	public:
		// --- Constructors ---

		Chunk(const i32 chunkX, const i32 chunkZ);
		~Chunk();

		// --- Setters & Getters ---

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

		SubChunk	*subChunks[SUBCHUNK_AMOUNT];

	private:
		// --- Chunk Data ---

		const i32	_chunkX;
		const i32	_chunkZ;


		Chunk		*_northNeighbour = nullptr;
		Chunk		*_southNeighbour = nullptr;
		Chunk		*_eastNeighbour = nullptr;
		Chunk		*_westNeighbour = nullptr;
};
