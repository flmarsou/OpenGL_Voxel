#pragma once

# include "config.hpp"

# include <GLAD/glad.h>

# include <vector>
# include <array>

# define VOXEL_INDEX(x, y, z)	(x + CHUNK_WIDTH * (y + CHUNK_HEIGHT * z))

enum	VoxelFaces
{
	RIGHT,
	LEFT,
	TOP,
	BOTTOM,
	BACK,
	FRONT
};

struct	BitShiftVoxel
{
	// [- - - - b b b b b b b b b b f f f z z z z z y y y y y x x x x x]
	static u32	Pack(u8 x, u8 y, u8 z, u8 face, u16 blockID)
	{
		return (
			(blockID & 0x3FF) << 18 |
			(face & 0x7) << 15 |
			(z & 0x1F) << 10 |
			(y & 0x1F) << 5 |
			(x & 0x1F)
		);
	}
};


struct	BitShiftChunk
{
	// [z z z z z z z z z z z z z z z z z z z z z z z z z z z z z z z z
	// x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x x]
	static u64	Pack(i32 x, i32 z)
	{
		return ((u64)((u32)(z)) << 32) | (u32)(x);
	}

	static void	Unpack(u64 data, i32 &x, i32 &z)
	{
		x = (i32)(data & 0xFFFFFFFF);
		z = (i32)(data >> 32);
	}
};

class	Chunk
{
	public:
		// --- Constructors ---

		Chunk(const i32 cx, const i32 cz);
		~Chunk();

		// --- Setters & Getters ---

		u32		GetVoxel(u8 vx, u8 vy, u8 vz) const;
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

		u32		GetVAO() const;
		u32		GetIndexCount() const;

		// --- Methods ---

		bool	IsNeighborVoxel(i8 vx, i8 vy, i8 vz) const;
		bool	IsSurrounded(u8 vx, u8 vy, u8 vz) const;
		bool	IsFaceVisible(i8 vx, i8 vy, i8 vz, u8 face) const;

		// --- Meshing --- (ChunkMesh.cpp)

		void	GenerateBuffers();
		void	GenerateMesh();

		// --- Generation --- (ChunkGeneration.cpp)

		void	GenerateVoxels();

	private:
		// --- Chunk Data ---

		const i32	_chunkX;
		const i32	_chunkZ;

		u16			_voxels[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH];

		Chunk		*_northNeighbour = nullptr;
		Chunk		*_southNeighbour = nullptr;
		Chunk		*_eastNeighbour = nullptr;
		Chunk		*_westNeighbour = nullptr;

		// --- Chunk Mesh ---

		u32			_vao;
		u32			_vbo;
		u32			_ebo;
		u32			_indexCount;
};
