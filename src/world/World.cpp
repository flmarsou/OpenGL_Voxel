#include "World.hpp"

static bool	isLoad(const i32 playerX, const i32 playerZ, const i32 currentX, const i32 currentZ, const i32 radius);

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

			SetNeighbors(currentX, currentZ, currentChunkKey);
		}
	}
	for (auto &chunk : this->chunks)
	{
		chunk.second->GenerateMesh();
	}
}

void	World::Reload(const i32 playerX, const i32 playerZ)
{
	const i32	radius = RENDER_DISTANCE / 2;

	for (i32 offsetZ = -radius - 1; offsetZ <= radius + 1; offsetZ++)
	for (i32 offsetX = -radius - 1; offsetX <= radius + 1; offsetX++)
	{
		i32	currentX = playerX + offsetX;
		i32	currentZ = playerZ + offsetZ;
		u64	currentChunkKey = BitShiftChunk::Pack(currentX, currentZ);

		if (this->chunks.find(currentChunkKey) == this->chunks.end())
		{
			if (isLoad(playerX, playerZ, currentX, currentZ, radius))
			{
				this->chunks[currentChunkKey] = new Chunk(currentX, currentZ);
				this->SetNeighbors(currentX, currentZ, currentChunkKey);

				this->chunks[currentChunkKey]->GenerateMesh();

				if (this->chunks[currentChunkKey]->GetNorthNeighbour())
					this->chunks[currentChunkKey]->GetNorthNeighbour()->GenerateMesh();
				if (this->chunks[currentChunkKey]->GetSouthNeighbour())
					this->chunks[currentChunkKey]->GetSouthNeighbour()->GenerateMesh();
				if (this->chunks[currentChunkKey]->GetEastNeighbour())
					this->chunks[currentChunkKey]->GetEastNeighbour()->GenerateMesh();
				if (this->chunks[currentChunkKey]->GetWestNeighbour())
					this->chunks[currentChunkKey]->GetWestNeighbour()->GenerateMesh();
			}
		}
		else
		{
			if (!isLoad(playerX, playerZ, currentX, currentZ, radius))
			{
				delete this->chunks[currentChunkKey];
				this->chunks.erase(currentChunkKey);
			}
		}
	}
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

void	World::SetNeighbors(const i32 currentX, const i32 currentZ, const u64 currentChunkKey)
{
	Chunk	*currentChunk = this->chunks[currentChunkKey];

	const u64	northChunkKey = BitShiftChunk::Pack(currentX, currentZ - 1);
	const u64	southChunkKey = BitShiftChunk::Pack(currentX, currentZ + 1);
	const u64	eastChunkKey = BitShiftChunk::Pack(currentX + 1, currentZ);
	const u64	westChunkKey = BitShiftChunk::Pack(currentX - 1, currentZ);

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

// ========================================================================== //
//    Helper Functions                                                        //
// ========================================================================== //

static bool	isLoad(const i32 playerX, const i32 playerZ, const i32 currentX, const i32 currentZ, const i32 radius)
{
	i32	dx = currentX - playerX;
	i32	dy = currentZ - playerZ;
	return (dx * dx + dy * dy <= radius * radius);
}
