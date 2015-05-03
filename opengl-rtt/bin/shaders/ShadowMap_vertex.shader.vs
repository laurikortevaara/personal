#version 400 core

uniform mat4 mvp;

layout(location = 0) in vec3 vert;
layout(location = 1) in vec2 vertTexCoord;

void main(){
    gl_Position =  mvp * vec4(vert,1);
}