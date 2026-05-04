#version 460 core

layout (location = 0) in vec3 aPos;

void main()
{
   vec3 offset = vec3(0.0f, -0.8f, 0.0f);
   vec3 position = vec3(aPos.x, aPos.y, aPos.z) + offset;
   gl_Position = vec4(position, 1.0);
};
