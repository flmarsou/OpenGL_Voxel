#include "VoxelMesh.hpp"

// ========================================================================== //
//    Init                                                                    //
// ========================================================================== //

void	VoxelMesh::Init()
{
	const float	vertices[] = {
	//  x, y, z, u, v
	// Right Face (x+)
		1, 0, 0, 0, 0,
		1, 1, 0, 0, 1,
		1, 1, 1, 1, 1,
		1, 0, 1, 1, 0,

	// Left Face (x-)
		0, 0, 1, 0, 0,
		0, 1, 1, 0, 1,
		0, 1, 0, 1, 1,
		0, 0, 0, 1, 0,

	// Top Face (y+)
		0, 1, 0, 0, 0,
		0, 1, 1, 0, 1,
		1, 1, 1, 1, 1,
		1, 1, 0, 1, 0,

	// Bottom Face (y-)
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 1,
		1, 0, 0, 1, 1,
		1, 0, 1, 1, 0,

	// Back Face (z+)
		0, 0, 1, 0, 0,
		1, 0, 1, 1, 0,
		1, 1, 1, 1, 1,
		0, 1, 1, 0, 1,

	// Front Face (z-)
		1, 0, 0, 0, 0,
		0, 0, 0, 1, 0,
		0, 1, 0, 1, 1,
		1, 1, 0, 0, 1
	};

	const u32	indices[] = {
		0, 1, 2, 2, 3, 0,		// Right  (x+)
		4, 5, 6, 6, 7, 4,		// Left   (x-)
		8, 9, 10, 10, 11, 8,	// Top    (y+)
		12, 13, 14, 14, 15, 12,	// Bottom (y-)
		16, 17, 18, 18, 19, 16,	// Back   (z+)
		20, 21, 22, 22, 23, 20	// Front  (z-)
	};

	// VAO (Vertex Array Object)
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);

	// VBO (Vertex Buffer Object)
	glGenBuffers(1, &this->_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBO (Element Buffer Object)
	glGenBuffers(1, &this->_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertex Attribute: Position (location = 0)
	glVertexAttribPointer(
		0,					// Attribute Index
		3,					// Size (x, y, z)
		GL_FLOAT,
		GL_FALSE,
		5 * sizeof(float),
		(void *)0			// Offset
	);
	glEnableVertexAttribArray(0);

	// Vertex Attribute: Texture UV (location = 1)
	glVertexAttribPointer(
		1,							// Attribute Index
		2,							// Size (u, v)
		GL_FLOAT,
		GL_FALSE,
		5 * sizeof(float),
		(void *)(3 * sizeof(float))	// Offset
	);
	glEnableVertexAttribArray(1);

	std::cout << INFO "VoxelMesh's VAO, VBO, EBO created" RESET << std::endl;
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

void	VoxelMesh::Bind() const
{
	glBindVertexArray(this->_vao);
}

void	VoxelMesh::Cleanup()
{
	glDeleteVertexArrays(1, &this->_vao);
	glDeleteBuffers(1, &this->_vbo);
	glDeleteBuffers(1, &this->_ebo);

	std::cout << INFO "VoxelMesh's VAO, VBO, EBO deleted" RESET << std::endl;
}
