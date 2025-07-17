#version 460 core

out vec4	fragmentColor;

in	vec3	texCoord;

uniform	sampler2DArray	texArray;

void	main()
{
	fragmentColor = texture(texArray, texCoord);
}
