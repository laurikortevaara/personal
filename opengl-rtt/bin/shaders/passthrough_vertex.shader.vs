#version 400 core

uniform mat4 mvp;
uniform mat4 DepthBiasMVP;

//uniform vec4 light;

layout(location = 0) in vec3 vert;
layout(location = 1) in vec2 vertTexCoord;
//layout(location = 2) in vec3 vertNormal;

out vec3 fragVert;
out vec2 fragTexCoord;
//out vec3 fragNormal;
//out vec4 fragLight;
//out vec3 halfVector;
out vec4 ShadowCoord;

void main() {
    // Pass some variables to the fragment shader
    fragTexCoord = vertTexCoord;
//    fragNormal = vertNormal;
    fragVert = vert;
//    fragLight = light;
    
    //vec3 l = light - model;
    //vec3 v = model * vert;
    
    //halfVector = normalize((l + v)/normalize(l+v));
    
    // Apply all matrix transformations to vert
    gl_Position = mvp * vec4(vert, 1);
    ShadowCoord = DepthBiasMVP * vec4(vert, 1);
}