#pragma once

# include "types.hpp"

enum	mesh_e
{
	CUBE
};

struct	mesh_t
{
	u32	VAO = 0;
	u32	VBO = 0;
	u32	EBO = 0;
	u32	indexCount = 0;
};
