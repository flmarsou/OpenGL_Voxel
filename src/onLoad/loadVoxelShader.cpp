#include "onLoad.hpp"
#include <fstream>	// std::ifstream

static u32	makeModule(const char *path, const u32 type)
{
	// Open shader file
	std::ifstream		file;

	file.open(path);
	if (!file.is_open())
	{
		debug(ERROR);
		std::cerr << "Failed to open shader file: " << path << std::endl;
		return (0);
	}

	// Read shader file
	std::stringstream	buffer;

	buffer << file.rdbuf();

	const std::string	shaderSourceString = buffer.str();

	file.close();

	// Create and compile the shader
	u32	shader;
	shader = glCreateShader(type);

	const char	*shaderSourceChar = shaderSourceString.c_str();
	glShaderSource(shader, 1, &shaderSourceChar, nullptr);

	glCompileShader(shader);

	// Check for compilation errors
	i32	status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		char	log[1024];
		glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
		debug(ERROR);
		std::cerr << "Shader compilation failed in \"" << path << "\": " << log << std::endl;
		return (0);
	}

	debug(INFO);
	std::cout << "Compiled shader \"" << path << "\"" << std::endl;

	return (shader);
}

static u32	makeShader(const u32 modules[2])
{
	u32	shader = glCreateProgram();
	glAttachShader(shader, modules[0]);
	glAttachShader(shader, modules[1]);
	glLinkProgram(shader);

	// Check for linking errors
	i32	status;
	glGetProgramiv(shader, GL_LINK_STATUS, &status);
	if (!status)
	{
		char	log[1024];
		glGetProgramInfoLog(shader, sizeof(log), nullptr, log);
		debug(ERROR);
		std::cerr << "Voxel shaders linking failed: " << log << std::endl;
		return (0);
	}

	debug(INFO);
	std::cout << "Voxel shader linked" << std::endl;

	// Cleanup
	glDeleteShader(modules[0]);
	glDeleteShader(modules[1]);

	return (shader);
}

bool	loadVoxelShader(u32 &shader)
{
	u32	modules[2];

	modules[0] = makeModule(VOXEL_VERTEX_PATH, GL_VERTEX_SHADER);
	if (!modules[0])
		return (false);

	modules[1] = makeModule(VOXEL_FRAGMENT_PATH, GL_FRAGMENT_SHADER);
	if (!modules[1])
		return (false);

	shader = makeShader(modules);
	if (!shader)
		return (false);

	return (true);
}
