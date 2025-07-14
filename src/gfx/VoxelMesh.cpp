#include "VoxelMesh.hpp"

// ========================================================================== //
//    Init                                                                    //
// ========================================================================== //

void	VoxelMesh::Init()
{
	const float	vertices[] = {
		//  x  y  z
			0, 0, 0,	// 0
			1, 0, 0,	// 1
			1, 1, 0,	// 2
			0, 1, 0,	// 3
			0, 0, 1,	// 4
			1, 0, 1,	// 5
			1, 1, 1,	// 6
			0, 1, 1		// 7
		};

	const u32	indices[] = {
		1, 5, 6, 6, 2, 1,	// Right  x+
		0, 4, 7, 7, 3, 0,	// Left   x-
		3, 2, 6, 6, 7, 3,	// Top    y+
		0, 1, 5, 5, 4, 0,	// Bottom y-
		0, 1, 2, 2, 3, 0,	// Back   z+
		4, 5, 6, 6, 7, 4	// Front  z-
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

	// Vertex Attribute: Position
	glVertexAttribPointer(
		0,					// Index
		3,					// Size
		GL_FLOAT,			// Type
		GL_FALSE,			// Normalized?
		3 * sizeof(float),	// Stride
		(void *)0			// Pointer
	);
	glEnableVertexAttribArray(0);

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
