#version 330

in vec4 color;
in vec3 fNormal;
in vec3 fPosition;

uniform mat4 model;

uniform float time;
uniform float rand;

const float ambientCoeff   = 0.1;
const float specularCoeff  = 1;

uniform int UseNormalMap;
uniform float specularExp;

uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 ViewPosition;


in vec2 TexCoord;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

void main()                                                             
{

    // Ambient
    vec3 ambient = ambientCoeff * LightColor;


    // Diffuse
    float roughness = 0.5;
    vec3 lightDir = normalize(LightPosition - fPosition);
    float diff = max(dot(fNormal, lightDir), 0.0);
    vec3 diffuse = roughness * diff * LightColor;


    //Specular
    float specularStrength = 0.4;
    float shininess = 1.0;
    vec3 viewDir = normalize(ViewPosition-fPosition);
    vec3 reflectDir = reflect(-lightDir, fNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),32);
    vec3 specular = shininess * specularStrength * spec * LightColor;
    
    vec3 ObjectColor = texture(texture_diffuse1, TexCoord).rgb;

    vec3 result = (ambient + diffuse + specular) * ObjectColor;
    gl_FragColor = vec4(result, 1.0);
}