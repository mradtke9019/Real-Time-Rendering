#version 330 core

in vec3 vNormal;
in vec3 vPosition;
in vec2 vTexture;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fNormal;
out vec3 fPosition;
out vec2 TexCoord;

void main()
{

	gl_Position = projection * view * model * vec4(vPosition, 1.0);

	fPosition = vec3(gl_Position);
	fNormal =  transpose(inverse(mat3(model))) * vNormal;

	TexCoord = vec2(vTexture.x, vTexture.y);
}