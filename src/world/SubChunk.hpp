#pragma once

# include "config.hpp"
# include "Chunk.hpp"

# include <GLAD/glad.h>
# include <Noises/FastNoiseLite.hpp>

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

class	Chunk;

class	SubChunk
{
	public:
		// --- Constructors ---

		SubChunk(Chunk *parent, const i32 subChunkY);
		~SubChunk();

		// --- Setters & Getters ---
		void	SetVoxel(const u8 voxelX, const u8 voxelY, const u8 voxelZ, const u16 blockID);
		u16		GetVoxel(const u8 voxelX, const u8 voxelY, const u8 voxelZ) const;

		i8		GetSubChunkY() const;

		u32		GetVAO() const;
		u32		GetIndexCount() const;

		// --- Mesh ---
		void	GenerateBuffers();
		void	GenerateMesh();

		// --- Helper Methods ---
		bool	IsSurrounded(const u8 voxelX, const u8 voxelY, const u8 voxelZ) const;
		bool	IsNeighborVoxelAir(const i8 neighborX, const i8 neighborY, const i8 neighborZ) const;

		// --- Generation ---
		void	GenerateVoxels();

	private:
		Chunk		*_parent;
		const i32	_subChunkY;

		u16			_voxels[CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_WIDTH];

		u32			_vao;
		u32			_vbo;
		u32			_ebo;
		u32			_indexCount;
};
