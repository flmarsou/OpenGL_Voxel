#include "World.hpp"

static bool	isLoad(const i32 playerX, const i32 playerZ, const i32 chunkX, const i32 chunkZ, const i32 radius);

// ========================================================================== //
//    Load                                                                    //
// ========================================================================== //

void	World::Reload(const i32 playerX, const i32 playerZ)
{
	const i32	radius = RENDER_DISTANCE / 2;

	std::vector<u64>			newLoadedChunkKey;
	std::unordered_set<Chunk *>	chunksToUpdate;

	// 1. Load new chunks and unload old ones
	for (i32 offsetZ = -radius - 1; offsetZ <= radius + 1; offsetZ++)
	for (i32 offsetX = -radius - 1; offsetX <= radius + 1; offsetX++)
	{
		i32	chunkX = playerX + offsetX;
		i32	chunkZ = playerZ + offsetZ;
		u64	currentChunkKey = BitShiftChunk::Pack(chunkX, chunkZ);

		auto	it = this->chunks.find(currentChunkKey);

		// --- Unload old chunks ---
		if (it != this->chunks.end())
		{
			if (!isLoad(playerX, playerZ, chunkX, chunkZ, radius))
			{
				delete this->chunks[currentChunkKey];
				this->chunks.erase(currentChunkKey);
			}
		}
		// --- Load new chunks ---
		else if (it == this->chunks.end())
		{
			if (isLoad(playerX, playerZ, chunkX, chunkZ, radius))
			{
				// Create the chunk
				this->chunks[currentChunkKey] = new Chunk(chunkX, chunkZ);
				newLoadedChunkKey.push_back(currentChunkKey);

				// Create the subChunks
				for (u8 chunkY = 0; chunkY < SUBCHUNK_AMOUNT; chunkY++)
					this->chunks[currentChunkKey]->subChunks[chunkY]->GenerateVoxels();
			}
		}
	}

	// 2. Set neighbors for newly loaded chunks
	for (u64 key : newLoadedChunkKey)
	{
		i32	chunkX;
		i32	chunkZ;

		BitShiftChunk::Unpack(key, chunkX, chunkZ);
		SetNeighbors(chunkX, chunkZ, key);

		// Chunks to update
		Chunk	*currentChunk = this->chunks[key];
		if (currentChunk->GetNorthNeighbour())
			chunksToUpdate.insert(currentChunk->GetNorthNeighbour());
		if (currentChunk->GetSouthNeighbour())
			chunksToUpdate.insert(currentChunk->GetSouthNeighbour());
		if (currentChunk->GetEastNeighbour())
			chunksToUpdate.insert(currentChunk->GetEastNeighbour());
		if (currentChunk->GetWestNeighbour())
			chunksToUpdate.insert(currentChunk->GetWestNeighbour());
	}

	std::vector<SubChunk *>	toUpload;

	// 3. Generate buffers + meshes for newly loaded chunks (threads)
	for (u64 key : newLoadedChunkKey)
	for (u8 chunkY = 0; chunkY < SUBCHUNK_AMOUNT; chunkY++)
	{
		SubChunk	*subChunk = this->chunks[key]->subChunks[chunkY];
		this->_pool.Enqueue([subChunk]{ subChunk->GenerateMeshData(); });
		toUpload.push_back(subChunk);
	}

	// 4. Reload buffers + meshes of neighboring chunks of newly loaded chunks (threads)
	for (Chunk *chunk : chunksToUpdate)
	for (u8 chunkY = 0; chunkY < SUBCHUNK_AMOUNT; chunkY++)
	{
		SubChunk	*subChunk = chunk->subChunks[chunkY];
		this->_pool.Enqueue([subChunk]{ subChunk->GenerateMeshData(); });
		toUpload.push_back(subChunk);
	}

	this->_pool.Wait();

	for (SubChunk *subChunk : toUpload)
		subChunk->UploadMeshToGPU();
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

void	World::SetNeighbors(const i32 chunkX, const i32 chunkZ, const u64 chunkKey)
{
	Chunk	*currentChunk = this->chunks[chunkKey];

	const u64	northChunkKey = BitShiftChunk::Pack(chunkX, chunkZ - 1);
	const u64	southChunkKey = BitShiftChunk::Pack(chunkX, chunkZ + 1);
	const u64	eastChunkKey = BitShiftChunk::Pack(chunkX + 1, chunkZ);
	const u64	westChunkKey = BitShiftChunk::Pack(chunkX - 1, chunkZ);

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

static bool	isLoad(const i32 playerX, const i32 playerZ, const i32 chunkX, const i32 chunkZ, const i32 radius)
{
	i32	dx = chunkX - playerX;
	i32	dy = chunkZ - playerZ;
	return (dx * dx + dy * dy <= radius * radius);
}
