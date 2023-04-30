#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


uniform vec3 LightColor;
uniform vec3 LightPosition;

out vec3 fNormal;
out vec3 fPosition;
out vec2 TexCoord;
out vec3 mPosition;

void main()                                                              
{
	mPosition = vec3(model * vec4(vPosition, 1.0));
	gl_Position = projection * view *vec4(mPosition,1);

	fPosition = vec3(gl_Position);
	fNormal =  transpose(inverse(mat3(model))) * vNormal;
	TexCoord = vec2(vTexture.x, vTexture.y);
}
