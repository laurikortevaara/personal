#version 400 core


// Interpolated values from the vertex shaders
in vec3 fragVert;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 ShadowCoord;
in float distToCamera;

// Ouput data
layout(location = 0) out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D tex;
uniform sampler2D shadowMap;
uniform float     shadowDelta;
uniform vec3      lightPos;
uniform vec3      CameraPos;


vec3 applyFog(in vec3  rgb,      // original color of the pixel
              in float distance, // camera to point distance
              in vec3  rayOri,   // camera position
              in vec3  rayDir )  // camera to point vector
{
    float c = 0.1;
    float b = 0.1;
    float fogAmount = c*exp(-rayOri.y*b)*(1.0-exp(-distance*rayDir.y*b))/rayDir.y;
    vec3  fogColor  = vec3(0.5,0.6,0.7);
    return mix( rgb, fogColor, fogAmount );
}

void main()
{
    vec3 shadowTexCoord = ShadowCoord.xyz/ShadowCoord.w;
    float depth =  texture(shadowMap, shadowTexCoord.xy).r;

    // Light emission properties
    float f = max(0.15,normalize(dot(lightPos,fragNormal)));
    float inShadow = (depth < (shadowTexCoord.z+shadowDelta)) ? 0.15 : 1.0;
    vec3 LightColor = vec3(1,1,1);
    
    vec3 v = CameraPos;
    
    // Material properties
    vec4 MaterialDiffuseColor = texture( tex, fragTexCoord );
    
    color.rgb = mix(vec3(f*MaterialDiffuseColor*inShadow), vec3(0.5,0.6,0.7), min(0.9,0.9*(distToCamera/20.0)));
    color.a = 1;
}