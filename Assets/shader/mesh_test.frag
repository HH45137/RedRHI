#version 460 core

out vec4 FragColor;

in vec3 outNorm;
in vec2 outUV;

void main()
{
   vec3 color = outNorm * vec3(1.0f);
   FragColor = vec4(color, 1.0f);
}
