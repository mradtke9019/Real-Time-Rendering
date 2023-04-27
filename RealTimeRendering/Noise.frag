#version 330

in vec4 color;
in vec3 fNormal;
in vec3 fPosition;

uniform mat4 model;

uniform float time;
uniform float rand;

const float ambientCoeff   = 0.1;
const float specularCoeff  = 1;
uniform float specularExp = 64;

uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 ViewPosition;

float random(vec3 seed) 
{
    return fract(sin(dot(seed, vec3(12.9898, 78.233, 45.543))) * 43758.5453);
}

void main()                                                             
{
    bool blinn = true;
    vec3 normal = fNormal;

    vec3 lightDir = normalize(LightPosition - fPosition);
    vec3 viewDir = normalize(ViewPosition - fPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    float scale =  random(fPosition + time * 0.001);
    vec3 ObjectColor = vec3(scale,scale,scale);

    if(ObjectColor == vec3(0,0,0))
    {
        ObjectColor = vec3(1.0,0.0,0.0);
    }

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