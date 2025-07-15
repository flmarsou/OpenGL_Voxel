#pragma once

# include "types.hpp"
# include "options.hpp"

# include <iostream>

# include <GLAD/glad.h>

struct	Voxel
{
	// [- - - - - - - b b b b b b b b b b z z z z z y y y y y x x x x x]
	static u32	Pack(u8 x, u8 y, u8 z, u32 blockID)
	{
		return ((blockID & 0x3FF) << 15 | (z & 0x1F) << 10 | (y & 0x1F) << 5 | (x & 0x1F));
	}

	static void	Unpack(u32 data, u8 &x, u8 &y, u8 &z, u32 &blockID)
	{
		x = (data) & 0x1F;
		y = (data >> 5) & 0x1F;
		z = (data >> 10) & 0x1F;
		blockID = (data >> 15) & 0x3FF;
	}
};

class	VoxelMesh
{
	public:
		void	Init();

		void	Bind() const;

		void	Cleanup();

	private:
		u32	_vao;
		u32	_vbo;
		u32	_ebo;
};
