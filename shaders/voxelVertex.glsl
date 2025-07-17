#version 460 core

// ! Send Voxel/Vertex Pack instead, so GPU can Unpack it
layout (location = 0) in vec3	aPos;
layout (location = 1) in vec2	aTex;
layout (location = 2) in float	blockID;

out	vec3	texCoord;

uniform mat4	uCamera;
uniform mat4	uModel;

void	main()
{
	gl_Position = uCamera * uModel * vec4(aPos, 1.0);

	// Texture UV + Texture ID
	texCoord = vec3(aTex, blockID);
}
