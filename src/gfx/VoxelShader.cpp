#include "VoxelShader.hpp"

// ========================================================================== //
//    Init                                                                    //
// ========================================================================== //

bool	VoxelShader::Init(const i8 *vertexFile, const i8 *fragmentFile)
{
	// Read from shader files
	std::string	vertexCode = getFileContent(vertexFile);
	std::string	fragmentCode = getFileContent(fragmentFile);

	// Convert string to char
	const i8	*vertexSource = vertexCode.c_str();
	const i8	*fragmentSource = fragmentCode.c_str();

	// Compile shaders
	u32	vertexShader = createShader(vertexSource, GL_VERTEX_SHADER, vertexFile);
	u32	fragmentShader = createShader(fragmentSource, GL_FRAGMENT_SHADER, fragmentFile);

	// Link program
	this->program = glCreateProgram();
	glAttachShader(this->program, vertexShader);
	glAttachShader(this->program, fragmentShader);
	glLinkProgram(this->program);

	// Cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return (true);
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

void	VoxelShader::Use() const
{
	glUseProgram(this->program);
}

void	VoxelShader::Cleanup()
{
	glDeleteProgram(this->program);

	std::cout << INFO "VoxelShader's program shader deleted" RESET << std::endl;
}

// ========================================================================== //
//    Helper Functions                                                        //
// ========================================================================== //

std::string	getFileContent(const i8 *fileName)
{
	std::ifstream	in(fileName, std::ios::binary);

	if (in)
	{
		std::cout << INFO "Found shader file: " RESET << fileName << std::endl;

		std::string	buffer;

		in.seekg(0, std::ios::end);

		buffer.resize(in.tellg());

		in.seekg(0, std::ios::beg);
		in.read(&buffer[0], buffer.size());
		in.close();

		if (buffer.empty())
			throw std::runtime_error(std::string("Shader file empty: ") + fileName);

		return (buffer);
	}

	throw std::runtime_error(std::string("Couldn't open shader file: ") + fileName);
}

u32	createShader(const i8 *shaderSource, u32 moduleType, const i8 *fileName)
{
	// Create and compile the shader
	u32	shader = glCreateShader(moduleType);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	// Check for compilation errors
	i32	status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		i8	log[512];
		glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
		throw std::runtime_error(std::string("Shader compilation failed: ") + log);
	}

	std::cout << SUCCESS "Compiled shader file: " RESET << fileName << std::endl;

	return (shader);
}
