#version 420

layout(location = 0) in vec3 in_Pos;
layout(location = 1) in vec3 in_Color;

layout(location = 0) out vec3 out_Color;


layout(binding = 0, std140) uniform Settings
{
    vec4 TessLevels;
    mat4 WorldMatrix;
} u_Settings;

void main()
{
    gl_Position = u_Settings.WorldMatrix * vec4(in_Pos, 1.0f);
    out_Color = in_Color;
}