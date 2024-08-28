#version 420 core

layout (location = 0) out vec4 FragColor;

in vec4 color;
in float texSlot;
in vec2 TexCoords;

//uniform sampler2D texture;
uniform sampler2D texture0;

void main(){
	
	if (texSlot == 0.0){
	
		FragColor = texelFetch(texture0, ivec2(TexCoords), 0);
	}	
	else{
	
		FragColor = color;
	}

	
}