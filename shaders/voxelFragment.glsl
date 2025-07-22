#version 460 core

in	vec3	TexCoord;

out vec4	fragmentColor;

uniform	sampler2DArray	texArray;

void	main()
{
	fragmentColor = texture(texArray, TexCoord);
}
