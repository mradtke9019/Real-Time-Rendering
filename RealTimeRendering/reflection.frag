#version 330 core

in vec3 fNormal;
in vec3 fPosition;

uniform vec3 cameraPos;
uniform samplerCube cubemap;

// Reference: https://www.scss.tcd.ie/Michael.Manzke/CS7055/GLSL/GLSL-3rdEd-refraction.pdf
// Reference: https://learnopengl.com/Advanced-OpenGL/Cubemaps

void main()
{    
    vec3 n = normalize(fNormal);
    vec3 I = normalize(fPosition - cameraPos);
    vec3 reflectionVec = reflect(I, n);

    vec3 reflectionColor = vec3(texture(cubemap, normalize(reflectionVec)));
    
    gl_FragColor = vec4(reflectionColor,1.0);
}