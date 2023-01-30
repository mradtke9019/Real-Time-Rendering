#version 330

in vec4 color;
in vec3 fNormal;
in vec3 fPosition;



uniform float time;
uniform mat4 MVP;
uniform mat4 Identity;
uniform float rand;

uniform float thresholds[3];
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

	if (intensity > thresholds[0])
		mult = 1.0f;
	else if (intensity > thresholds[1])
		mult = 0.75f;
	else if (intensity > thresholds[2])
		mult = 0.4f;
	else
		mult = 0.2f;

	gl_FragColor = vec4(mult * ObjectColor, 1.0);
}