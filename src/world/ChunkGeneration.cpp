#include "Chunk.hpp"

#include <Noises/FastNoiseLite.hpp>

void	Chunk::GenerateVoxels()
{
	FastNoiseLite	noise;

	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.01f);
	noise.SetSeed(42);

	for (u8 x = 0; x < CHUNK_WIDTH; x++)
	for (u8 z = 0; z < CHUNK_WIDTH; z++)
	{
		i32	worldX = this->_chunkX * CHUNK_WIDTH + x;
		i32	worldZ = this->_chunkZ * CHUNK_WIDTH + z;

		float	noiseValue = noise.GetNoise(static_cast<float>(worldX), static_cast<float>(worldZ));
		i32		height = static_cast<i32>((noiseValue + 1) * 0.5f * (CHUNK_HEIGHT - 1));

		for (u8 y = 0; y < CHUNK_HEIGHT; y++)
		{
			if (y == height)
				this->_voxels[VOXEL_INDEX(x, y, z)] = BitShiftVoxel::Pack(x, y, z, GRASS_BLOCK);
			else if (y <= height)
				this->_voxels[VOXEL_INDEX(x, y, z)] = BitShiftVoxel::Pack(x, y, z, DIRT_BLOCK);
			else
				this->_voxels[VOXEL_INDEX(x, y, z)] = BitShiftVoxel::Pack(x, y, z, AIR_BLOCK);
		}
	}
}
