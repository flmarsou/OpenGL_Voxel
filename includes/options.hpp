#pragma once

// --- Debug Logs ---
# define SUCCESS	"\e[32m[v] "
# define INFO		"\e[36m[i] "
# define WARNING	"\e[35m[!] "
# define ERROR		"\e[31m[x] "
# define RESET		"\e[0m"

// --- Window ---
# define WINDOW_WIDTH	1280
# define WINDOW_HEIGHT	720
# define WINDOW_TITLE	"Voxel Game"

// --- Shader Files ---
# define VOXEL_VERTEX_PATH		"../shaders/voxelVertex.glsl"
# define VOXEL_FRAGMENT_PATH	"../shaders/voxelFragment.glsl"

// --- Textures ---
# define DEBUG_BLOCK_PATH	"../assets/debug_block.png"
# define DIRT_BLOCK_PATH	"../assets/dirt_block.png"

// --- Texture Enum ---
enum	TextureID
{
	DEBUG_BLOCK,
	AIR_BLOCK,
	DIRT_BLOCK,
	TEXTURE_COUNT
};

// --- Camera Settings ---
# define CAMERA_SPEED			0.5f
# define CAMERA_SENSITIVITY		0.1f
# define CAMERA_FOV				45.0f

// --- World ---
# define RENDER_DISTANCE	5
# define CHUNK_WIDTH		32
# define CHUNK_HEIGHT		32
