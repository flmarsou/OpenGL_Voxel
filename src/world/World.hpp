#pragma once

# include "config.hpp"

# include "Chunk.hpp"

# include <unordered_map>
# include <unordered_set>

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

class World
{
	public:
		World() : _pool(std::thread::hardware_concurrency()) {}

		void	Reload(const i32 playerX, const i32 playerZ);

		void	SetNeighbors(const i32 chunkX, const i32 chunkZ, const u64 chunkKey);

		std::unordered_map<u64, Chunk *>	chunks;

	private:
		ThreadPool	_pool;
};
