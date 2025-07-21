#include "Chunk.hpp"

#include <Noises/FastNoiseLite.hpp>

void	Chunk::GenerateVoxels()
{
	FastNoiseLite	baseNoise;

	baseNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	baseNoise.SetFrequency(0.01f);
	baseNoise.SetSeed(SEED);

	FastNoiseLite	detailNoise;

	detailNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	detailNoise.SetFrequency(0.05f);
	detailNoise.SetSeed(SEED);

	for (u8 x = 0; x < CHUNK_WIDTH; x++)
	for (u8 z = 0; z < CHUNK_WIDTH; z++)
	{
		i32	worldX = this->_chunkX * CHUNK_WIDTH + x;
		i32	worldZ = this->_chunkZ * CHUNK_WIDTH + z;

		float	baseValue = baseNoise.GetNoise(static_cast<float>(worldX), static_cast<float>(worldZ));
		float	detailValue = detailNoise.GetNoise(static_cast<float>(worldX), static_cast<float>(worldZ));

		float	fusedValues = baseValue * 0.7f + detailValue * 0.3f;

		i32		height = static_cast<i32>((fusedValues + 1) * 0.5f * (CHUNK_HEIGHT - 1));

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
