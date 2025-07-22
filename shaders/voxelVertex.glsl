#version 460 core

layout (location = 0) in uint	packedVoxel;

uniform mat4	uCamera;
uniform mat4	uModel;

out	vec3		TexCoord;

const vec3 faceOffsets[6][4] = vec3[6][4](
	vec3[](vec3(1, 0, 0), vec3(1, 1, 0), vec3(1, 1, 1), vec3(1, 0, 1)),	// Right
	vec3[](vec3(0, 0, 1), vec3(0, 1, 1), vec3(0, 1, 0), vec3(0, 0, 0)),	// Left
	vec3[](vec3(0, 1, 0), vec3(0, 1, 1), vec3(1, 1, 1), vec3(1, 1, 0)),	// Top
	vec3[](vec3(0, 0, 1), vec3(0, 0, 0), vec3(1, 0, 0), vec3(1, 0, 1)),	// Bottom
	vec3[](vec3(0, 0, 1), vec3(1, 0, 1), vec3(1, 1, 1), vec3(0, 1, 1)),	// Back
	vec3[](vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0), vec3(1, 1, 0))	// Front
);

void	main()
{
	// --- Unpack Voxel Data ---
	const uint	blockID	= (packedVoxel >> 18) & 0x3FF;
	const uint	face	= (packedVoxel >> 15) & 0x07;
	const uint	z		= (packedVoxel >> 10) & 0x1F;
	const uint	y		= (packedVoxel >> 5) & 0x1F;
	const uint	x		= packedVoxel & 0x1F;

	const vec3	voxelPos = vec3(x, y, z);

	uint		vertexIndex = gl_VertexID % 4;

	// --- Position ---
	const vec3	pos = voxelPos + faceOffsets[face][vertexIndex];

	gl_Position = uCamera * uModel * vec4(pos, 1.0);

	// --- UV and Texture ---
	vec2	uv;

	switch (vertexIndex)
	{
		case (0): uv = vec2(0.0, 0.0); break ;
		case (1): uv = vec2(0.0, 1.0); break ;
		case (2): uv = vec2(1.0, 1.0); break ;
		case (3): uv = vec2(1.0, 0.0); break ;
	}

	if (face == 4 || face == 5)
		uv = vec2(uv.y, 1.0 + uv.x);

	TexCoord = vec3(uv, blockID);
}
