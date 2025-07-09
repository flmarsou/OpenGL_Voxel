#pragma once

# include "types.hpp"
# include "mesh.hpp"
# include "debug.hpp"
# include <GLAD/glad.h>
# include <GLFW/glfw3.h>

# define VOXEL_VERTEX_PATH		"../shaders/voxelVertex.glsl"
# define VOXEL_FRAGMENT_PATH	"../shaders/voxelFragment.glsl"

bool	initGLFW(GLFWwindow	*&window);
bool	initGLAD();

bool	loadVoxelShader(u32 &shader);

mesh_t	loadCubeMesh();
