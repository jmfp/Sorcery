#version 330 core
out vec4 FragColor;

in vec3 fragPos;

uniform vec3 roadColor = vec3(1.0, 1.0, 1.0); // Default white, can be changed per draw call

void main()
{
    FragColor = vec4(roadColor, 1.0);
}
