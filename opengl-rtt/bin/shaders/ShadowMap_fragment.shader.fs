#version 400 core

// Ouput data
//layout(location = 0) out float fragmentdepth;
layout(location = 0) out vec4 frag_color;

void main(){
    // Not really needed, OpenGL does it anyway
    //fragmentdepth = gl_FragCoord.z;
    frag_color.rgb = vec3(gl_FragCoord.z);
    frag_color.a = 1;
}