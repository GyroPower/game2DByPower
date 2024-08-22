#version 420 core

layout (location = 0) out vec4 FragColor;

in vec4 color;
in float texSlot;
in vec2 TexCoords;

//uniform sampler2D texture;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main(){
	
	if (texSlot == 0.0){
	
		//int index = int(texSlot);
		FragColor = texelFetch(texture0, ivec2(TexCoords), 0);
		//FragColor = texture(textures[index], TexCoords);
	}
	else if (texSlot == 1.0f){
		FragColor = texture(texture1,TexCoords);
	}
	
	else{
	
		FragColor = color;
	}

	
}