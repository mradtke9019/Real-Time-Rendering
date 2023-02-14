#version 330 core

in vec3 fNormal;
in vec3 fPosition;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

void main()
{
	gl_FragColor = texture(texture_diffuse1, TexCoord);
}