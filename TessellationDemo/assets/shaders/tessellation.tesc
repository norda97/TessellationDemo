#version 420
layout(vertices = 3) out;

layout(location = 0) in vec3 in_Color[gl_MaxPatchVertices];

layout(location = 0) out vec3 tese_Color[3];

layout(binding = 0, std140) uniform Settings
{
    vec4 TessLevels;
    mat4 WorldMatrix;
} u_Settings;

void main()
{
    if(gl_InvocationID == 0)
    {
        vec4 tessLevels = u_Settings.TessLevels;
        
        gl_TessLevelInner[0] = tessLevels.x;
        gl_TessLevelOuter[0] = tessLevels.y;
        gl_TessLevelOuter[1] = tessLevels.z;
        gl_TessLevelOuter[2] = tessLevels.w;
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tese_Color[gl_InvocationID] = in_Color[gl_InvocationID];
}