#version 420 core

layout (location = 0) out vec4 FragColor;

in vec4 Color;
in vec2 TexCoords;
in float TexId;

uniform sampler2D textures[2];

void main()
{
	
	if (TexId >= 0){

		int index = int(TexId);
		FragColor = texture(textures[index], TexCoords);
	}
	else{
		FragColor = Color;
	}
}

