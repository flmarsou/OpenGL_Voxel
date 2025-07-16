#include "World.hpp"

// ========================================================================== //
//    Load                                                                    //
// ========================================================================== //

void	World::Load(const i32 playerX, const i32 playerZ)
{
	const i32	radius = RENDER_DISTANCE / 2;

	for (i32 offsetZ = -radius; offsetZ <= radius; offsetZ++)
	{
		for (i32 offsetX = -radius; offsetX <= radius; offsetX++)
		{
			i32	currentX = playerX + offsetX;
			i32	currentZ = playerZ + offsetZ;

			if (!isLoad(playerX, playerZ, currentX, currentZ, radius))
				continue ;

			// Create chunk
			u64	currentChunkKey = BitShiftChunk::Pack(currentX, currentZ);
			this->chunks[currentChunkKey] = new Chunk(currentX, currentZ);

			// Check and set neighboring chunks
			Chunk	*currentChunk = this->chunks[currentChunkKey];

			u64	northChunkKey = BitShiftChunk::Pack(currentX, currentZ - 1);
			u64	southChunkKey = BitShiftChunk::Pack(currentX, currentZ + 1);
			u64	eastChunkKey = BitShiftChunk::Pack(currentX + 1, currentZ);
			u64	westChunkKey = BitShiftChunk::Pack(currentX - 1, currentZ);

			if (this->chunks.find(northChunkKey) != this->chunks.end())
			{
				Chunk	*northChunk = this->chunks[northChunkKey];

				currentChunk->SetNorthNeighbour(northChunk);
				northChunk->SetSouthNeighbour(currentChunk);
			}

			if (this->chunks.find(southChunkKey) != this->chunks.end())
			{
				Chunk	*southChunk = this->chunks[southChunkKey];

				currentChunk->SetSouthNeighbour(southChunk);
				southChunk->SetNorthNeighbour(currentChunk);
			}

			if (this->chunks.find(eastChunkKey) != this->chunks.end())
			{
				Chunk	*eastChunk = this->chunks[eastChunkKey];

				currentChunk->SetEastNeighbour(eastChunk);
				eastChunk->SetWestNeighbour(currentChunk);
			}

			if (this->chunks.find(westChunkKey) != this->chunks.end())
			{
				Chunk	*westChunk = this->chunks[westChunkKey];

				currentChunk->SetWestNeighbour(westChunk);
				westChunk->SetEastNeighbour(currentChunk);
			}
		}
	}
}

// ========================================================================== //
//    Functions                                                               //
// ========================================================================== //

bool	isLoad(const i32 playerX, const i32 playerZ, const i32 currentX, const i32 currentZ, const i32 radius)
{
	i32	dx = currentX - playerX;
	i32	dy = currentZ - playerZ;
	return (dx * dx + dy * dy <= radius * radius);
}
