#version 330 core

in vec3 fNormal;
in vec3 fPosition;

uniform vec3 cameraPos;
uniform samplerCube cubemap;

// Reference: https://www.scss.tcd.ie/Michael.Manzke/CS7055/GLSL/GLSL-3rdEd-refraction.pdf
// Reference: https://learnopengl.com/Advanced-OpenGL/Cubemaps
const float indexOfRefraction = 1.00 / 1.52;
const float FresnelPower = 5.0;
const float F = ((1.0-indexOfRefraction) * (1.0-indexOfRefraction)) / ((1.0+indexOfRefraction) * (1.0+indexOfRefraction));



void main()
{    
    vec3 n = normalize(fNormal);
    vec3 I = normalize(fPosition - cameraPos);
    vec3 reflectionVec = reflect(I, n);
    vec3 refractionVec = refract(I, n, indexOfRefraction);

    vec3 reflectionColor = vec3(texture(cubemap, normalize(reflectionVec)));
    vec3 refractionColor = vec3(texture(cubemap, normalize(refractionVec)));
    
    float ratio = F + (1.0 - F) * pow((1.0 - dot(-I, n)), FresnelPower);

    vec3 color = mix(refractionColor, reflectionColor, ratio);
    //color = reflectionColor;
    gl_FragColor = vec4(color,1.0);
}