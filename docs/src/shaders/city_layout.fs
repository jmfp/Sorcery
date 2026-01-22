#version 330 core
out vec4 FragColor;

in vec3 fragPos;

void main()
{
    // Road color - bright white for visibility
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
