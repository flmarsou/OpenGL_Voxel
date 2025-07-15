#pragma once

# include "types.hpp"
# include "options.hpp"

# include "Chunk.hpp"

# include <vector>
# include <iostream>

class World
{
	public:
		// void	init();
		void	load(const int playerX, const int playerY);
		// void	reload();
		// void	actuLoad();
		std::vector<Chunk *>	chunks;

	protected:

	private:
		// Chunk	_Chunks[];
};
