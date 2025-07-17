#pragma once

# include "config.hpp"

# include <GLAD/glad.h>
# include <STB/stb_image.h>

# include <vector>

class	Texture
{
	public:
		void	LoadArray(const std::vector<const char *> &imagePaths);

		void	Bind();
		void	Unbind();

		void	Cleanup();

	private:
		u32	_textureArrayID;
};
