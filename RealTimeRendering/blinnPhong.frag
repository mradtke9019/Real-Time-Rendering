#version 330

in vec4 color;
in vec3 fNormal;
in vec3 fPosition;



uniform float time;
uniform mat4 model;
uniform mat4 Identity;
uniform float rand;

const float ambientCoeff   = 0.1;
const float specularCoeff  = 1;

uniform int UseNormalMap;
uniform float specularExp;

vec3 ObjectColor;
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 LightDirection;
uniform vec3 ViewPosition;


in vec2 TexCoord;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

void main()                                                             
{
    bool blinn = true;
    vec3 normal;
    if(UseNormalMap == 1)
    {
        // obtain normal from normal map in range [0,1]
        // transform normal vector to range [-1,1]
        normal = texture(texture_normal1, TexCoord).xyz;
        normal = (normal * 2.0 - 1.0);
        normal = normalize(transpose(inverse(mat3(model))) * normal);
    }
    else
    {
        normal = fNormal;
    }

    vec3 lightDir = normalize(LightPosition - fPosition);
    vec3 viewDir = normalize(ViewPosition - fPosition);
    vec3 reflectDir = reflect(-lightDir, normal);

    ObjectColor = texture(texture_diffuse1, TexCoord).xyz;

    // Ambient
    vec3 ambient = ambientCoeff * ObjectColor;


    // Diffuse
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * ObjectColor;


    // Specular
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), specularExp);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExp);
    }
    vec3 specular = specularCoeff * LightColor * spec;



    gl_FragColor = vec4(ambient + diffuse + specular, 1.0); //+ texture(texture_diffuse1, TexCoord);
}