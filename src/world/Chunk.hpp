#pragma once

# include "config.hpp"

# include "SubChunk.hpp"

struct	BitShiftChunk
{
	static u64	Pack(i32 chunkX, i32 chunkZ)
	{
		return (static_cast<u64>(chunkX) << 32) | (static_cast<u32>(chunkZ));
	}

	static void	Unpack(u64 data, i32 &chunkX, i32 &chunkZ)
	{
		chunkX = static_cast<i32>(data >> 32);
		chunkZ = static_cast<i32>(data & 0xFFFFFFFF);
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

		// --- SubChunks ---

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
