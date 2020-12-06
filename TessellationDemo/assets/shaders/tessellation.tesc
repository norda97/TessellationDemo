#version 420
layout(vertices = 3) out;

layout(location = 0) in vec3 in_Color[gl_MaxPatchVertices];

layout(location = 0) out vec3 tese_Color[3];

void main()
{
    if(gl_InvocationID == 0)
    {
        float outerTess = 1.0f;
        gl_TessLevelInner[0] = 2.0f;
        gl_TessLevelOuter[0] = outerTess;
        gl_TessLevelOuter[1] = outerTess;
        gl_TessLevelOuter[2] = outerTess;
    }

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tese_Color[gl_InvocationID] = in_Color[gl_InvocationID];
}