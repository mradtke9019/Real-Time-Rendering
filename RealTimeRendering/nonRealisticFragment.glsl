#version 330

in vec4 color;
in vec3 fNormal;
in vec3 fPosition;



uniform float time;
uniform mat4 MVP;
uniform mat4 Identity;
uniform float rand;

const float ambientCoeff   = 1.5;
const float specularCoeff  = 14.1;
const float specularExp    = 2.0;

uniform vec3 LightColor;
uniform vec3 ObjectColor;
uniform vec3 LightPosition;
uniform vec3 LightDirection;
in vec3 FragPos;


void main()
{
	float intensity;
	vec4 color;
	intensity = dot(LightDirection,normalize(fNormal));

	if (intensity > 0.6)
		color = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		color = vec4(0.66,0.33,0.33,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.2,0.2,1.0);
	else
		color = vec4(0.2,0.1,0.1,1.0);
	gl_FragColor = color;

}