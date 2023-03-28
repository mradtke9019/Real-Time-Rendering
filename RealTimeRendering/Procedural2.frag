


in vec3 outTexCoords;
in vec3 fNormal;
in vec3 fPosition;
in float zrat;

uniform float rmin;
uniform float rmax;
uniform float hmax;
uniform int N;
uniform sampler2D ColorMap;
uniform sampler2D SpecularMap;
uniform sampler2D NormalMap;
uniform sampler2D PithRadiusMap;
uniform sampler2D KnotHeightMap;
uniform sampler2D KnotOrientMap;
uniform sampler2D KnotStateMap;
uniform vec3 LightPosition;
uniform vec3 viewPos;
uniform mat4 model;
uniform mat4 view;
uniform float time;
uniform float rand;




const float ambientCoeff   = 0.1;
const float specularCoeff  = 1.0;
const float specularExp = 32.0;
uniform vec3 ViewPosition;

float random (vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float noise (vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    vec2 u = f * f * (3.0 - 2.0 * f);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

void main() {
    vec3 LightColor = vec3(1.0,1.0,1.0);
    float scale =  1.0;
    vec2 TexCoords = vec2(outTexCoords.x,outTexCoords.y);
    vec2 uv = TexCoords * scale;
    float noiseValue = noise(vec2(uv.x + time, uv.y));
    float noiseValue1 = noise(vec2(rand * uv.x + time, uv.y));
    float noiseValue2 = noise(vec2(uv.x/rand + time, uv.y));
    float noiseValue3 = noise(vec2(uv.x * rand * 2.0 + time, uv.y));
    //vec4 color = texture2D(ColorMap, TexCoords) * noiseValue;
    //vec4 color = texture2D(ColorMap, vec2(noiseValue,0.5));
    vec4 color = vec4(noiseValue1,noiseValue2,noiseValue3,1.0);

    bool blinn = true;
    vec3 normal;

    normal = fNormal;

    vec3 lightDir = normalize(LightPosition - fPosition);
    vec3 viewDir = normalize(ViewPosition - fPosition);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ObjectColor = vec3(color);
    if(ObjectColor == vec3(0,0,0))
    {
        ObjectColor = vec3(0.5,0.5,0.5);
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



    gl_FragColor = vec4(ambient + diffuse + specular, 1.0); //+ texture(texture_diffuse1, TexCoord);
}