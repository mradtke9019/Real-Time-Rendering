#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexture;
in vec4 vColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 LightColor;
uniform vec3 ObjectColor;
uniform vec3 LightPosition;
uniform vec3 LightDirection;

out vec4 color;
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
