#version 460 core

out vec4	fragmentColor;

in	vec2	texCoord;

uniform	sampler2D	tex0;

void	main()
{
	fragmentColor = texture(tex0, texCoord);
}
