#version 420 core

/* this and the frag shader are for the player but because a bug that make the renderer of the other entities cross data with
the player renderer is not in use, and I use instead the rendererIntancing shaders*/

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aTexCoords;
layout (location = 2) in float aTexSlot;
layout (location = 3) in vec4 aColor;
layout (location = 4) in mat4 instanceMatrix;


out vec4 color;
out float texSlot;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 modelZoomCamera;

void main()
{

	

	float left = aTexCoords.x;
	float top = aTexCoords.y;
	float right = aTexCoords.x + aTexCoords.z;
	float bottom = aTexCoords.y + aTexCoords.z;

	vec2 textureCoords[4] = {
		vec2(left,top),
		vec2(right,top),
		vec2(right,bottom),
		vec2(left,bottom)
	};

	gl_Position = projection * view * modelZoomCamera * instanceMatrix * vec4(aPos.x,aPos.y,aPos.z,1.0f);
	TexCoords = textureCoords[gl_VertexID];
	color = aColor;
	texSlot = aTexSlot;
}
