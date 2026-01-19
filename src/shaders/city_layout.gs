#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 projection;
uniform mat4 view;

void main() {
    // Create a thick horizontal line from each point
    // These are in clip space, so make them bigger
    float lineLength = 0.1;  // Length of the line (in clip space)
    float lineWidth = 0.01;  // Width of the line (thickness in clip space)
    
    vec4 center = gl_in[0].gl_Position;
    
    // Create a quad for a thick horizontal line
    // Bottom-left
    gl_Position = center + vec4(-lineLength, -lineWidth, 0.0, 0.0);
    EmitVertex();
    
    // Bottom-right
    gl_Position = center + vec4(lineLength, -lineWidth, 0.0, 0.0);
    EmitVertex();
    
    // Top-left
    gl_Position = center + vec4(-lineLength, lineWidth, 0.0, 0.0);
    EmitVertex();
    
    // Top-right
    gl_Position = center + vec4(lineLength, lineWidth, 0.0, 0.0);
    EmitVertex();
    
    EndPrimitive();
} 