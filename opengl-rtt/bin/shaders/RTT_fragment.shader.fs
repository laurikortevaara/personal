#version 400 core

uniform sampler2D tex;

in vec3 fragVert;
in vec2 fragTexCoord;

out vec4 frag_color;

void main()
{
    vec4 col = texture(tex,fragTexCoord);
    frag_color.rgb = vec3(gl_FragCoord.z);
    frag_color.a = 1;
}