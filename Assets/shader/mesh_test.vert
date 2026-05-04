#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNorm;
layout (location = 2) in vec2 inUV;

out vec3 outNorm;
out vec2 outUV;

void main()
{
   vec3 pos = vec3(0.0f, -0.8f, 0.0f);
   vec3 scale = vec3(1.0f);

   vec3 position = inPos * scale + pos;

   gl_Position = vec4(position, 1.0);

   outNorm = inNorm;
   outUV = inUV;
};
