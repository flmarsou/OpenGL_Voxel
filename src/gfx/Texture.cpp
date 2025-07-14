#include "Texture.hpp"

void	Texture::Load(const i8 *imagePath, TextureID textureID)
{
	i32	imageWidth;
	i32	imageHeight;
	i32	imageChannel;

	u8	*bytes = stbi_load(imagePath, &imageWidth, &imageHeight, &imageChannel, 4);
	if (!bytes)
		throw std::runtime_error(std::string("Failed to load texture: ") + imagePath);

	glGenTextures(1, &_textureID[textureID]);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Texture Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);

	std::cout << SUCCESS "Loaded texture: " << imagePath << std::endl;
}

void	Texture::Bind(TextureID textureID)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void	Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void	Texture::Cleanup()
{
	for (i32 i = 0; i < TEXTURE_COUNT; i++)
		glDeleteTextures(1, &_textureID[i]);
	std::cout << INFO "Textures deleted" << std::endl;
}
