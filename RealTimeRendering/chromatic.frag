#version 330 core

in vec3 fNormal;
in vec3 fPosition;

uniform vec3 cameraPos;
uniform samplerCube cubemap;

// Reference: https://www.scss.tcd.ie/Michael.Manzke/CS7055/GLSL/GLSL-3rdEd-refraction.pdf
// Reference: https://learnopengl.com/Advanced-OpenGL/Cubemaps
const float iorR = 0.65;
const float iorG = 0.67;
const float iorB = 0.69;

const float FresnelPower = 5.0;

const float F = ((1.0-iorG) * (1.0-iorG)) / ((1.0+iorG) * (1.0+iorG));



void main()
{    
    vec3 n = normalize(fNormal);
    vec3 I = normalize(fPosition - cameraPos);
    vec3 reflectionVec = reflect(I, n);
    vec3 reflectionColor = vec3(texture(cubemap, reflectionVec));
    
    vec3 refractionVecR = refract(I, n, iorR);
    vec3 refractionVecG = refract(I, n, iorG);
    vec3 refractionVecB = refract(I, n, iorB);



    vec3 refractionR = vec3(texture(cubemap, refractionVecR));
    vec3 refractionG = vec3(texture(cubemap, refractionVecG));
    vec3 refractionB = vec3(texture(cubemap, refractionVecB));
    
    float ratio = F + (1.0 - F) * pow((1.0 - dot(-I, n)), FresnelPower);

    vec3 refractionColor;
    refractionColor.r = refractionR.r;
    refractionColor.g = refractionG.g;
    refractionColor.b = refractionB.b;

    vec3 color = mix(refractionColor, reflectionColor, ratio);
    //color = refractionColor;
    gl_FragColor = vec4(color,1.0);
}