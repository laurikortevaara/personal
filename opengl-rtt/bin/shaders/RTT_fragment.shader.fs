#version 400 core

uniform sampler2D tex;
uniform sampler2D tex2;

uniform float isFboRender;

uniform vec3 lightPos;
uniform vec3 param;

in vec3 fragVert;
in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 frag_color;

void main()
{
    if(isFboRender==0)
    {
        vec4 color1 = texture(tex2, fragTexCoord);
        frag_color = 0.5*(texture(tex, fragTexCoord+sin(fragVert.x+param.x)-cos(fragVert.z+param.x*2.0))+abs(sin(fragVert.z))*color1);
        float l = length(vec4(0.0,0.0,0.0,1.0)-vec4(fragVert,1.0));
        frag_color = frag_color / max(1.0,(l/5.0));
    }
    else
    {
        
    }
    frag_color.a = 1.0;
}