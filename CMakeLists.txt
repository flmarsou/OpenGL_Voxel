cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)
project(voxelGame)

add_subdirectory(libraries/GLFW)

# C/C++ Files
add_executable(${CMAKE_PROJECT_NAME}
	libraries/GLAD/glad.c
	src/main.cpp
	src/onLoad/initGLFW.cpp
	src/onLoad/initGLAD.cpp
	src/onLoad/loadVoxelShader.cpp
	src/onLoad/loadMeshes.cpp
)

# Header Files
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/includes
	${CMAKE_SOURCE_DIR}/libraries
)

# Link Libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
	glfw
)
