#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Move points forward so they're visible
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, -2.0, 1.0); 
}