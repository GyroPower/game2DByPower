#version 420 core 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in mat4 instanceModel;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 zoomMatrix;

out vec4 color;

void main(){

	gl_Position = projection * view * zoomMatrix * instanceModel * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	color = aColor;
}