#version 400 core

uniform mat4 mvp;

layout(location = 0) in vec3 vert;
layout(location = 1) in vec3 normal;
layout(location = 1) in vec2 vertTexCoord;
//layout(location = 2) in vec3 vertNormal;

out vec3 fragVert;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main()
{
    // Pass some variables to the fragment shader
    fragTexCoord = vertTexCoord;
    fragVert = vert;
    
    // Apply all matrix transformations to vert
    gl_Position = mvp * vec4(vert, 1);
}