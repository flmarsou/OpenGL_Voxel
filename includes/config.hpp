#pragma once

# include "types.hpp"

// --- Debug Logs ---
# include <iostream>

# define SUCCESS	"\e[32m[v] "
# define INFO		"\e[36m[i] "
# define WARNING	"\e[35m[!] "
# define ERROR		"\e[31m[x] "
# define RESET		"\e[0m"

// --- Window ---
# define WINDOW_WIDTH	1480
# define WINDOW_HEIGHT	920
# define WINDOW_TITLE	"Voxel Game"

// --- Shader Files ---
# define VOXEL_VERTEX_PATH		"../shaders/voxelVertex.glsl"
# define VOXEL_FRAGMENT_PATH	"../shaders/voxelFragment.glsl"

// --- Textures ---
# define DEBUG_BLOCK_PATH		"../assets/debug_block.png"
# define DIRT_BLOCK_PATH		"../assets/dirt_block.png"
# define GRASS_BLOCK_TOP_PATH	"../assets/grass_block_top.png"
# define GRASS_BLOCK_SIDE_PATH	"../assets/grass_block_side.png"

// --- Block Enums ---
enum	BlockIDs
{
	AIR_BLOCK,
	DIRT_BLOCK,
	GRASS_BLOCK,
	DEBUG_BLOCK,
};

enum	TextureIDs
{
	DIRT_BLOCK_TEXTURE,
	GRASS_BLOCK_TOP_TEXTURE,
	GRASS_BLOCK_SIDE_TEXTURE,
	DEBUG_BLOCK_TEXTURE,
};

// --- Camera Settings ---
# define CAMERA_SPEED			1.0f
# define CAMERA_SENSITIVITY		0.1f
# define CAMERA_FOV				90.0f

// --- World ---
# define RENDER_DISTANCE	10
# define CHUNK_WIDTH		32
# define CHUNK_HEIGHT		32
# define SEED				42
