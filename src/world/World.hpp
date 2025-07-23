#pragma once

# include "config.hpp"

# include "Chunk.hpp"

# include <unordered_map>

class World
{
	public:
		void	Load(const i32 playerX, const i32 playerZ);
		// void	Reload(const i32 playerX, const i32 playerZ);

		void	SetNeighbors(const i32 chunkX, const i32 chunkZ, const u64 chunkKey);

		std::unordered_map<u64, Chunk *>	chunks;
};
