#version 420 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec4 color;
layout (location = 3) in float texId;

out vec4 Color;
out vec2 TexCoords;
out float TexId;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
	Color = color;
	TexCoords = texCoords;
	TexId = texId;

	gl_Position = projection * view * model * vec4(pos.x + (10 * gl_InstanceID),pos.y,pos.z, 1.0f);
}