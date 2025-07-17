#pragma once

# include "types.hpp"
# include "options.hpp"

# include "Chunk.hpp"

# include <unordered_map>
# include <iostream>

class World
{
	public:
		void	SetNeighbors(i32 currentX, i32 currentZ, u64 currentChunkKey);
		void	Load(const i32 playerX, const i32 playerZ);
		void	Reload(const i32 playerX, const i32 playerZ);

		std::unordered_map<u64, Chunk *>	chunks;
};

bool	isLoad(const i32 playerX, const i32 playerZ, const i32 currentX, const i32 currentZ, const i32 radius);
