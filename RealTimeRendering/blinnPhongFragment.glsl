#version 330

in vec4 color;
in vec3 fNormal;
in vec3 fPosition;



uniform float time;
uniform mat4 MVP;
uniform mat4 Identity;
uniform float rand;

const float ambientCoeff   = 0.1;
const float specularCoeff  = 1;


uniform float specularExp;

uniform vec3 ObjectColor;
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 LightDirection;
uniform vec3 ViewPosition;
in vec3 FragPos;

void main()                                                             
{
    bool blinn = true;
    vec3 normal = normalize(fNormal);
    vec3 lightDir = normalize(LightPosition - FragPos);
    vec3 viewDir = normalize(ViewPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);


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



    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}