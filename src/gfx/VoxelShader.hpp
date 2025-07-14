#pragma once

# include "types.hpp"
# include "options.hpp"

# include <GLAD/glad.h>

# include <iostream>
# include <string>
# include <fstream>
# include <sstream>

class	VoxelShader
{
	public:
		bool	Init(const i8 *vertexFile, const i8 *fragmentFile);

		void	Use() const;

		void	Cleanup();

		u32		program;
};

std::string	getFileContent(const i8 *fileName);
u32			createShader(const i8 *shaderSource, u32 moduleType, const i8 *fileName);
