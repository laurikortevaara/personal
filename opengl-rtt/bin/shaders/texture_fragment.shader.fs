#version 400 core

uniform sampler2D tex;

in vec3 fragVert;
in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 frag_color;

void main()
{
    frag_color = texture( tex, fragTexCoord );
    frag_color.a = 1;
    frag_color.r = 1;
}