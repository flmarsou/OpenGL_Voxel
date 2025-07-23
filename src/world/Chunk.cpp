#include "Chunk.hpp"

// ========================================================================== //
//    Constructors                                                            //
// ========================================================================== //

Chunk::Chunk(const i32 cx, const i32 cz)
	:	_chunkX(cx), _chunkZ(cz)
{
	std::cout << INFO "Chunk " << "X: " << this->_chunkX << " | Z: " << this->_chunkZ << " loaded" RESET << std::endl;

	// Create SubChunks
	for (u8 i = 0; i < SUBCHUNK_AMOUNT; i++)
	{
		this->subChunks[i] = new SubChunk(this, i);
		this->subChunks[i]->GenerateVoxels();
		this->subChunks[i]->GenerateBuffers();
		this->subChunks[i]->GenerateMesh();
	}
}

Chunk::~Chunk()
{
	std::cout << INFO "Chunk " << "X: " << this->_chunkX << " | Z: " << this->_chunkZ << " unloaded" RESET << std::endl;

	// Unlink Neighbours
	if (GetNorthNeighbour())
		GetNorthNeighbour()->SetSouthNeighbour(nullptr);
	if (GetSouthNeighbour())
		GetSouthNeighbour()->SetNorthNeighbour(nullptr);
	if (GetEastNeighbour())
		GetEastNeighbour()->SetWestNeighbour(nullptr);
	if (GetWestNeighbour())
		GetWestNeighbour()->SetEastNeighbour(nullptr);

	// Delete SubChunks
	for (i32 i = 0; i < SUBCHUNK_AMOUNT; i++)
		delete this->subChunks[i];
}

// ========================================================================== //
//    Setters & Getters                                                       //
// ========================================================================== //

i32		Chunk::GetChunkX() const { return (this->_chunkX); }
i32		Chunk::GetChunkZ() const { return (this->_chunkZ); }

void	Chunk::SetNorthNeighbour(Chunk *chunk) { this->_northNeighbour = chunk; }
void	Chunk::SetSouthNeighbour(Chunk *chunk) { this->_southNeighbour = chunk; }
void	Chunk::SetEastNeighbour(Chunk *chunk) { this->_eastNeighbour = chunk; }
void	Chunk::SetWestNeighbour(Chunk *chunk) { this->_westNeighbour = chunk; }

Chunk	*Chunk::GetNorthNeighbour() const { return (this->_northNeighbour); }
Chunk	*Chunk::GetSouthNeighbour() const { return (this->_southNeighbour); }
Chunk	*Chunk::GetEastNeighbour() const { return (this->_eastNeighbour); }
Chunk	*Chunk::GetWestNeighbour() const { return (this->_westNeighbour); }
