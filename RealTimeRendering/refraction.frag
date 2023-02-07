#version 330 core

in vec3 fNormal;
in vec3 fPosition;

uniform vec3 cameraPos;
uniform samplerCube cubemap;

// Reference: https://www.scss.tcd.ie/Michael.Manzke/CS7055/GLSL/GLSL-3rdEd-refraction.pdf
// Reference: https://learnopengl.com/Advanced-OpenGL/Cubemaps
const float indexOfRefraction = 1.00 / 1.52;


void main()
{    
    vec3 n = normalize(fNormal);
    vec3 I = normalize(fPosition - cameraPos);
    vec3 refractionVec = refract(I, n, indexOfRefraction);
    vec3 refractionColor = vec3(texture(cubemap, normalize(refractionVec)));

    gl_FragColor = vec4(refractionColor,1.0);
}