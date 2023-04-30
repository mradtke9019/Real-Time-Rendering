#version 330

in vec4 color;
in vec3 fNormal;
in vec3 fPosition;
in vec3 mPosition;


uniform int smoothness = 1;
uniform mat4 model;

uniform float time;
uniform float rand;

const float ambientCoeff   = 0.1;
const float specularCoeff  = 1;
uniform float specularExp = 64;

uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform vec3 ViewPosition;

float DeterministicRandom(vec3 v) 
{
    return fract(sin(dot(v, vec3(12.9898, 78.233, 45.543))) * 43758.5453);
}

float InterpolatedNoise(vec3 v)
{
    vec3 lv = fract(v);
    vec3 id = floor(v);
    
    lv = lv*lv*(3.-2.*lv);
    
    float blFront = DeterministicRandom(id + vec3(0,0,0));
    float brFront = DeterministicRandom(id+vec3(1,0,0));
    float bFront = mix(blFront, brFront, lv.x);
    
    float tlFront = DeterministicRandom(id+vec3(0,1,0));
    float trFront = DeterministicRandom(id+vec3(1,1,0));
    float tFront = mix(tlFront, trFront, lv.x);
    
    float front = mix(bFront, tFront, lv.y);
    
    float blBack = DeterministicRandom(id+ vec3(0,0,1));
    float brBack = DeterministicRandom(id+vec3(1,0,1));
    float bBack = mix(blBack, brBack, lv.x);
    
    float tlBack = DeterministicRandom(id+vec3(0,1,1));
    float trBack = DeterministicRandom(id+vec3(1,1,1));
    float tBack = mix(tlBack, trBack, lv.x);
    
    float back = mix(bBack, tBack, lv.y);
    
    
    return mix(front,back,lv.z);
}

void main()                                                             
{
    bool blinn = true;
    vec3 normal = fNormal;

    vec3 lightDir = normalize(LightPosition - fPosition);
    vec3 viewDir = normalize(ViewPosition - fPosition);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 inputVal = vec3(mPosition.x,mPosition.y,1);

    float scale =  InterpolatedNoise((mPosition + time * 0.1)* smoothness);
    vec3 ObjectColor = vec3(scale);
    
    if(scale < 0.1)
    {
        ObjectColor = vec3(1,0,1) - vec3(0.1, 0 ,0.1);
    }
    else if(scale < 0.4)
    {
        ObjectColor =vec3(1,0,1)- vec3(0.4, 0 ,0.4);
    }
    else if(scale < 0.9)
    {
        ObjectColor = vec3(1,0,1)- vec3(0.9, 0 ,0.9);
    }
    else {
        ObjectColor = vec3(0);
    }
    //ObjectColor = vec3(scale,0,scale);


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