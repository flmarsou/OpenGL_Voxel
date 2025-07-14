cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
project(voxelGame)

add_subdirectory(libraries/GLFW)

# C/C++ Files
add_executable(${CMAKE_PROJECT_NAME}
	libraries/GLAD/glad.c

	src/main.cpp

	src/core/Window.cpp
	src/core/Renderer.cpp

	src/gfx/VoxelMesh.cpp
	src/gfx/VoxelShader.cpp
	src/gfx/Camera.cpp

	src/world/Chunk.cpp
	)

# Header Files
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes
	${CMAKE_SOURCE_DIR}/libraries
	${CMAKE_SOURCE_DIR}/src/core
	${CMAKE_SOURCE_DIR}/src/gfx
	${CMAKE_SOURCE_DIR}/src/world
)

# Link Libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
	glfw
)
