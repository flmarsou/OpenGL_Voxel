#include "World.hpp"

static bool	isLoad(const int playerX, const int playerY, const int currentX, const int currentY, int radius)
{
	int	dx = currentX - playerX;
	int	dy = currentY - playerY;
	return (dx * dx + dy * dy <= radius * radius);
}

void	World::load(const int playerX, const int playerY)
{
	const int	radius = RENDER_DISTANCE / 2;

	for (int offsetY = -radius; offsetY <= radius; offsetY++)
	{
		for (int offsetX = -radius; offsetX <= radius; offsetX++)
		{
			int	currentX = playerX + offsetX;
			int	currentY = playerY + offsetY;

			if (!isLoad(playerX, playerY, currentX, currentY, radius))
				continue ;
			this->chunks.push_back(new Chunk(currentX, 0, currentY));
		}
	}
}
