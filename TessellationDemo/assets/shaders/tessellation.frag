#version 420

layout(location = 0) in vec3 in_Color;

layout(location = 0) out vec3 out_FragColor;

void main()
{
    out_FragColor = in_Color;
}