#include "Chunk.hpp"

// ========================================================================== //
//    Constructors                                                            //
// ========================================================================== //

Chunk::Chunk(const i32 x, const i32 z)
	:	_chunkX(x), _chunkZ(z)
{
	std::cout << INFO "Chunk " << "X: " << _chunkX << " | Z: " << _chunkZ << " loaded" << std::endl;

	GenerateBuffers();
	GenerateVoxels();
	GenerateMesh();
}

Chunk::~Chunk()
{
	std::cout << INFO "Chunk " <<
		"X: " << _chunkX <<
		" | Z: " << _chunkZ <<
		" unloaded" RESET << std::endl;
	if (this->GetNorthNeighbour())
		this->GetNorthNeighbour()->SetSouthNeighbour(nullptr);
	if (this->GetSouthNeighbour())
		this->GetSouthNeighbour()->SetNorthNeighbour(nullptr);
	if (this->GetEastNeighbour())
		this->GetEastNeighbour()->SetWestNeighbour(nullptr);
	if (this->GetWestNeighbour())
		this->GetWestNeighbour()->SetEastNeighbour(nullptr);
}

// ========================================================================== //
//    Init                                                                    //
// ========================================================================== //

void Chunk::GenerateVoxels()
{
	for (u8 x = 0; x < CHUNK_WIDTH; x++)
	for (u8 y = 0; y < CHUNK_HEIGHT; y++)
	for (u8 z = 0; z < CHUNK_WIDTH; z++)
	{
		if (y == CHUNK_HEIGHT - 1 && x == CHUNK_WIDTH / 2 && z == CHUNK_WIDTH / 2)
			this->_voxels[VOXEL_INDEX(x, y, z)] = BitShiftVoxel::Pack(x, y, z, DEBUG_BLOCK);
		else
			this->_voxels[VOXEL_INDEX(x, y, z)] = BitShiftVoxel::Pack(x, y, z, DIRT_BLOCK);
	}
}

// ========================================================================== //
//    Setters & Getters                                                       //
// ========================================================================== //

u32		Chunk::GetVoxel(u8 x, u8 y, u8 z) const { return (this->_voxels[VOXEL_INDEX(x, y, z)]); }

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

u32		Chunk::GetVAO() const { return (this->_vao); }
u32		Chunk::GetIndexCount() const { return (this->_indexCount); }

// ========================================================================== //
//    Render                                                                  //
// ========================================================================== //

void	Chunk::GenerateBuffers()
{
	// VAO (Vertex Array Object)
	glGenVertexArrays(1, &this->_vao);
	glBindVertexArray(this->_vao);

	// VBO (Vertex Buffer Object)
	glGenBuffers(1, &this->_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);

	// EBO (Element Buffer Object)
	glGenBuffers(1, &this->_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);

	// Position
	glVertexAttribPointer(
		0,							// Layout
		3,							// Size
		GL_FLOAT,					// Type
		GL_FALSE,					// Normalized?
		6 * sizeof(float),			// Stride
		(void *)0					// Offset
	);
	glEnableVertexAttribArray(0);

	// Texture Position
	glVertexAttribPointer(
		1,							// Layout
		2,							// Size
		GL_FLOAT,					// Type
		GL_FALSE,					// Normalized?
		6 * sizeof(float),			// Stride
		(void *)(3 * sizeof(float))	// Offset
	);
	glEnableVertexAttribArray(1);

	// Block ID
	glVertexAttribPointer(
		2,							// Layout
		1,							// Size
		GL_FLOAT,					// Type
		GL_FALSE,					// Normalized?
		6 * sizeof(float),			// Stride
		(void *)(5 * sizeof(float))	// Offset
	);
	glEnableVertexAttribArray(2);

	// Unbind
	glBindVertexArray(0);
}

void	Chunk::GenerateMesh()
{
	std::vector<float>	vertices;
	std::vector<u32>	indices;
	u32					indexOffset = 0;

	for (u8 vx = 0; vx < CHUNK_WIDTH; vx++)
	for (u8 vy = 0; vy < CHUNK_HEIGHT; vy++)
	for (u8 vz = 0; vz < CHUNK_WIDTH; vz++)
	{
		if (IsSurrounded(vx, vy, vz))
			continue ;

		u32	voxel = GetVoxel(vx, vy, vz);
		u32	blockID;
		BitShiftVoxel::UnpackBlockID(voxel, blockID);

		for (u8 face = 0; face < 6; face++)
		{
			if (!IsFaceVisible(vx, vy, vz, face))
				continue ;

			auto	faceVert = getFaceVertices(vx, vy, vz, face, blockID);
			vertices.insert(vertices.end(), faceVert.begin(), faceVert.end());

			indices.push_back(indexOffset + 0);
			indices.push_back(indexOffset + 1);
			indices.push_back(indexOffset + 2);
			indices.push_back(indexOffset + 2);
			indices.push_back(indexOffset + 3);
			indices.push_back(indexOffset + 0);

			indexOffset += 4;
		}
	}

	glBindVertexArray(this->_vao);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);

	this->_indexCount = indices.size();

	glBindVertexArray(0);
}

std::array<float, 24>	getFaceVertices(const u8 vx, const u8 vy, const u8 vz, const u8 face, const u32 blockID)
{
	std::array<float, 24>	verts;

	const float	x = static_cast<float>(vx);
	const float	y = static_cast<float>(vy);
	const float	z = static_cast<float>(vz);

	switch (face)
	{
		case (RIGHT):
			verts = {
				x + 1, y + 0, z + 0, 0, 0, (float)blockID,
				x + 1, y + 1, z + 0, 0, 1, (float)blockID,
				x + 1, y + 1, z + 1, 1, 1, (float)blockID,
				x + 1, y + 0, z + 1, 1, 0, (float)blockID
			};
			break ;

		case (LEFT):
			verts = {
				x + 0, y + 0, z + 1, 0, 0, (float)blockID,
				x + 0, y + 1, z + 1, 0, 1, (float)blockID,
				x + 0, y + 1, z + 0, 1, 1, (float)blockID,
				x + 0, y + 0, z + 0, 1, 0, (float)blockID
			};
			break ;

		case (TOP):
			verts = {
				x + 0, y + 1, z + 0, 0, 0, (float)blockID,
				x + 0, y + 1, z + 1, 0, 1, (float)blockID,
				x + 1, y + 1, z + 1, 1, 1, (float)blockID,
				x + 1, y + 1, z + 0, 1, 0, (float)blockID
			};
			break ;

		case (BOTTOM):
			verts = {
				x + 0, y + 0, z + 1, 0, 0, (float)blockID,
				x + 0, y + 0, z + 0, 0, 1, (float)blockID,
				x + 1, y + 0, z + 0, 1, 1, (float)blockID,
				x + 1, y + 0, z + 1, 1, 0, (float)blockID
			};
			break ;

		case (BACK):
			verts = {
				x + 0, y + 0, z + 1, 0, 0, (float)blockID,
				x + 1, y + 0, z + 1, 1, 0, (float)blockID,
				x + 1, y + 1, z + 1, 1, 1, (float)blockID,
				x + 0, y + 1, z + 1, 0, 1, (float)blockID
			};
			break ;

		case (FRONT):
			verts = {
				x + 1, y + 0, z + 0, 0, 0, (float)blockID,
				x + 0, y + 0, z + 0, 1, 0, (float)blockID,
				x + 0, y + 1, z + 0, 1, 1, (float)blockID,
				x + 1, y + 1, z + 0, 0, 1, (float)blockID
			};
			break ;
	}

	return (verts);
}

// ========================================================================== //
//    Methods                                                                 //
// ========================================================================== //

u32		Chunk::GetNeighborVoxel(i8 x, i8 y, i8 z) const
{
	// Inside current chunk
	if (x >= 0 && x < CHUNK_WIDTH
		&& y >= 0 && y < CHUNK_HEIGHT
		&& z >= 0 && z < CHUNK_WIDTH)
		return (GetVoxel(x, y, z));

	// Outside current chunk
	if (x < 0 && this->_westNeighbour)
		return (this->_westNeighbour->GetNeighborVoxel(x + CHUNK_WIDTH, y, z));

	if (x >= CHUNK_WIDTH && this->_eastNeighbour)
		return (this->_eastNeighbour->GetNeighborVoxel(x - CHUNK_WIDTH, y, z));

	if (z < 0 && this->_northNeighbour)
		return (this->_northNeighbour->GetNeighborVoxel(x, y, z + CHUNK_WIDTH));

	if (z >= CHUNK_WIDTH && this->_southNeighbour)
		return (this->_southNeighbour->GetNeighborVoxel(x, y, z - CHUNK_WIDTH));

	// Missing neighbor or vertical
	return (0);
}

bool Chunk::IsSurrounded(u8 x, u8 y, u8 z) const
{

	// Check top/bottom boundaries
	if (y == 0 || y == CHUNK_HEIGHT - 1)
		return (false);

	// Check all six neighbors
	return ((!this->GetWestNeighbour() || GetNeighborVoxel(x - 1, y, z))	// Left = West
		&& (!this->GetEastNeighbour() || GetNeighborVoxel(x + 1, y, z))	// Right = East
		&& GetNeighborVoxel(x, y - 1, z)	// Bottom
		&& GetNeighborVoxel(x, y + 1, z)	// Top
		&& (!this->GetNorthNeighbour() || GetNeighborVoxel(x, y, z - 1))	// Front = North
		&& (!this->GetSouthNeighbour() || GetNeighborVoxel(x, y, z + 1))	// Back = South
	);
}

bool	Chunk::IsFaceVisible(i8 x, i8 y, i8 z, u8 dir) const
{
	// Get neighbor coordinates
	switch (dir)
	{
		case (0): x++; break ;
		case (1): x--; break ;
		case (2): y++; break ;
		case (3): y--; break ;
		case (4): z++; break ;
		case (5): z--; break ;
	}

	// Check neighbor
	return (GetNeighborVoxel(x, y, z) == 0);
}
