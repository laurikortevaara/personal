#version 400 core

uniform sampler2D tex;
uniform sampler2D shadowMap;

in vec3 fragVert;
in vec2 fragTexCoord;
in vec3 fragNormal;

in vec4 fragLight;
in vec4 ShadowCoord;

out vec4 frag_color;

void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(tex, 0));
    if(gl_FragCoord.x*texelSize.x > 0.0005)
    {
        float z = texture(shadowMap, fragTexCoord).r;
        float n = 0.01;
        float f = 1.0;
        float c = (2.0 * n) / (f + n - z * (f - n));

        frag_color.rgb = vec3(z);
        frag_color.a = 1;
    }
    else
    {
        frag_color = texture(tex,fragTexCoord);
    }
}