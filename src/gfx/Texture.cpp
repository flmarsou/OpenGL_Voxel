#include "Texture.hpp"

// ========================================================================== //
//    Load                                                                    //
// ========================================================================== //

void	Texture::LoadArray(const std::vector<const char *> &imagePaths)
{
	// Flip texture
	stbi_set_flip_vertically_on_load(true);

	i32	imageWidth;
	i32	imageHeight;
	i32	imageChannel;
	const u32	imageCount = imagePaths.size();

	// Fetch data
	u8	*bytes = stbi_load(imagePaths[0], &imageWidth, &imageHeight, &imageChannel, 4);
	if (!bytes)
		throw std::runtime_error(std::string("Failed to load texture: ") + imagePaths[0]);
	stbi_image_free(bytes);

	// Generate texture array
	glGenTextures(1, &this->_textureArrayID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->_textureArrayID);

	// Allocate storage for all layers
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, imageWidth, imageHeight, imageCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	// Upload textures data to GPU
	for (u32 i = 0; i < imageCount; i++)
	{
		i32	currentWidth;
		i32	currentHeight;

		u8	*data = stbi_load(imagePaths[i], &currentWidth, &currentHeight, &imageChannel, 4);

		if (!data)
			throw std::runtime_error(std::string("Failed to load texture: ") + imagePaths[i]);
		if (currentWidth != imageWidth || currentHeight != imageHeight)
			throw std::runtime_error(std::string("All textures must have the same dimensions: ") + imagePaths[i]);
		std::cout << SUCCESS "Loaded texture " << imagePaths[i] << std::endl;

		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, imageWidth, imageHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	// Apply texture filter
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

void	Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->_textureArrayID);
}

void	Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void	Texture::Cleanup()
{
	glDeleteTextures(1, &this->_textureArrayID);
	std::cout << INFO "Textures deleted" << std::endl;
}
