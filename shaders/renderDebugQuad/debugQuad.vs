#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec4 aColor;
layout (location = 4) in mat4 instanceMatrix;


out vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 modelZoomCamera;

void main(){

	
	gl_Position = projection * view * instanceMatrix * vec4(aPos.x,aPos.y,aPos.z,1.0f);
	
	color = aColor;
	
}
