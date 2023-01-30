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
	
    vec3 lightDir = normalize(LightPosition - FragPos);
	intensity = dot(lightDir,normalize(fNormal));
	float mult = 1.0f;

	if (intensity > 0.6)
		mult = 1.0f;
	else if (intensity > 0.5)
		mult = 0.8f;
	else if (intensity > 0.25)
		mult = 0.4f;
	else
		mult = 0.2f;

	gl_FragColor = vec4(mult * ObjectColor, 1.0);
}