#version 400 core

// Interpolated values from the vertex shaders
in vec2 fragTexCoord;
in vec4 ShadowCoord;

// Ouput data
layout(location = 0) out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D tex;
uniform sampler2D shadowMap;

void main()
{
    vec3 shadowTexCoord = ShadowCoord.xyz/ShadowCoord.w;
    float depth =  texture(shadowMap, shadowTexCoord.xy).r;
    float depth2 = texture(shadowMap, shadowTexCoord.xy+vec2(0.000001,0.000001)).r;
    float d3 = sqrt(depth*depth2);
    // Light emission properties
    
    float inShadow = (depth < (shadowTexCoord.z-0.005)) ? 0.5 : 1.0;
    vec3 LightColor = vec3(1,1,1);
    
    // Material properties
    vec4 MaterialDiffuseColor = texture( tex, fragTexCoord );
    
    color = MaterialDiffuseColor*inShadow;
    color.a = 1;
}