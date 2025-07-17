#pragma once

# include "config.hpp"

# include "Chunk.hpp"

# include <unordered_map>

class World
{
	public:
	void	Load(const i32 playerX, const i32 playerZ);
	void	Reload(const i32 playerX, const i32 playerZ);

	void	SetNeighbors(i32 currentX, i32 currentZ, u64 currentChunkKey);

	std::unordered_map<u64, Chunk *>	chunks;
};

bool	isLoad(const i32 playerX, const i32 playerZ, const i32 currentX, const i32 currentZ, const i32 radius);
