#version 330

in vec3 fNormal;
in vec3 fPosition;


uniform mat4 model;

in vec2 TexCoord;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

void main()                                                             
{
    gl_FragColor = texture(texture_diffuse1, TexCoord);
}