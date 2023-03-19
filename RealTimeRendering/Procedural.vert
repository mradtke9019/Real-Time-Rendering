#version 330
#extension GL_ARB_explicit_attrib_location : require
#extension GL_ARB_explicit_uniform_location : require

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float rmin;
uniform float hmax;

out vec3 outTexCoords;
out vec3 normal;
out vec3 fragPos;
out float zrat;


out vec3 fNormal;
out vec3 fPosition;
out vec2 TexCoord;

void main()
{
    gl_Position =  view * model * vec4(vPosition, 1.0);

    
	gl_Position = projection * view * model * vec4(vPosition, 1.0);

	fPosition = vec3(gl_Position);
	fNormal =  transpose(inverse(mat3(model))) * vNormal;

    zrat = hmax/rmin;
    outTexCoords = vec3( vPosition.x, vPosition.y, vPosition.z);
    normal = mat3(transpose(inverse(model))) * vNormal;
}
