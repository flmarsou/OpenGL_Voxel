#pragma once

# include "types.hpp"
# include "options.hpp"

# include <GLAD/glad.h>
# include <STB/stb_image.h>
# include <iostream>

class	Texture
{
	public:
		void	Load(const i8 *imagePath, TextureID textureID);

		void	Bind(TextureID textureID);
		void	Unbind();

		void	Cleanup();

	private:
		u32	_textureID[TEXTURE_COUNT];
};
