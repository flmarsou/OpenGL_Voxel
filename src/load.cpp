#include "types.hpp"
#include "mesh.hpp"
#include "debug.hpp"

#include <GLAD/glad.h>

mesh_t	loadCubeMesh()
{
	const float	vertices[] =
	{
		0.0f, 0.0f, 0.0f,	// 0: Bottom Front Left
		1.0f, 0.0f, 0.0f,	// 1: Bottom Front Right
		1.0f, 1.0f, 0.0f,	// 2: Top Front Right
		0.0f, 1.0f, 0.0f,	// 3: Top Front Left
		0.0f, 0.0f, 1.0f,	// 4: Bottom Back Left
		1.0f, 0.0f, 1.0f,	// 5: Bottom Back Right
		1.0f, 1.0f, 1.0f,	// 6: Top Back Right
		0.0f, 1.0f, 1.0f	// 7: Top Back Left
	};

	const u32	indices[] =
	{
		// Front Face
		0, 1, 2,
		2, 3, 0,

		// Right Face
		1, 5, 6,
		6, 2, 1,

		// Back Face
		7, 6, 5,
		5, 4, 7,

		// Left Face
		4, 0, 3,
		3, 7, 4,

		// Bottom Face
		4, 5, 1,
		1, 0, 4,

		// Top Face
		3, 2, 6,
		6, 7, 3
	};

	mesh_t	mesh;

	// Vertex Array Object
	glGenVertexArrays(1, &mesh.VAO);
	glBindVertexArray(mesh.VAO);

	// Vertex Buffer Object
	glGenBuffers(1, &mesh.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Element Buffer Object
	glGenBuffers(1, &mesh.EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertex Attribute: Position
	glVertexAttribPointer(
		0,					// Index
		3,					// Size (X, Y, Z)
		GL_FLOAT,			// Type
		GL_FALSE,			// Normalized?
		3 * sizeof(float),	// Stride
		(void *)0			// Offset
	);
	glEnableVertexAttribArray(0);

	mesh.indexCount = sizeof(indices) / sizeof(u32);

	debug("Created cube mesh (VAO, VBO, and EBO)!", INFO);

	return (mesh);
}
