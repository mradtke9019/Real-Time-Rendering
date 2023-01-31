#version 330

in vec4 color;
in vec3 fNormal;
in vec3 fPosition;

#define PI 3.14159265

uniform float time;
uniform mat4 MVP;
uniform mat4 Identity;
uniform float rand;

uniform float F0 = 0.8;
uniform float roughness = 0.1;
uniform float k = 0.2;


const float ambientCoeff   = 0.1;
uniform vec3 LightColor;
uniform vec3 ObjectColor;
uniform vec3 LightPosition;
uniform vec3 LightDirection;
uniform vec3 ViewPosition;

in vec3 FragPos;
// References: https://github.com/pboechat/cook_torrance/blob/master/application/shaders/cook_torrance_textured.fs.glsl 
// https://www.youtube.com/watch?v=hX1FFqmPV5s
void main()
{
    vec3 lightDir = normalize(LightPosition - FragPos);
    vec3 viewDir = normalize(ViewPosition - FragPos);

	float NdotL = max(0, dot(fNormal, lightDir));
	float Rs = 0.0;
	// Ony consider if the dot product is non zero as we only want to compute if it matters
	if (NdotL > 0)
	{
		vec3 H = normalize(lightDir + viewDir);
		float HdotN = max(0, dot(fNormal, H));
		float NdotV = max(0, dot(fNormal, viewDir));
		float VdotH = max(0, dot(lightDir, H));

		// Fresnel reflectance equatiokns
		float F = pow(1.0 - VdotH, 5.0);
		F *= (1.0 - F0);
		F += F0;

		// Beckmann roughness microfacet distribution function
		float m_squared = roughness * roughness;
		float r1 = 1.0 / (4.0 * m_squared * pow(HdotN, 4.0));
		float r2 = (HdotN * HdotN - 1.0) / (m_squared * HdotN * HdotN);
		float D = r1 * exp(r2);

		// Geometric attenuation
		float temp = 2.0 * HdotN;
		float gb = (temp * NdotV) / VdotH;
		float gc = (temp * NdotL) / VdotH;
		float G = min(1.0, min(gb, gc));

		Rs = (F * D * G) / (PI * NdotL * NdotV);
	}
	

	gl_FragColor = vec4(ObjectColor * LightColor * NdotL + LightColor * ObjectColor * NdotL * (k + Rs * (1.0 - k)), 1.0);
}