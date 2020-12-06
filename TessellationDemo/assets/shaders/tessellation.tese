#version 420

layout (triangles, equal_spacing, ccw) in;

layout(location = 0) in vec3 tese_Color[gl_MaxPatchVertices];

layout(location = 0) out vec3 out_Color;

void main()
{
    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +
    gl_TessCoord.y * gl_in[1].gl_Position +
    gl_TessCoord.z * gl_in[2].gl_Position);

    out_Color = (gl_TessCoord.x * tese_Color[0] +
    gl_TessCoord.y * tese_Color[1] +
    gl_TessCoord.z * tese_Color[2]);
}