#version 330 core
layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 projection;
uniform mat4 view;
uniform float roadWidth;

out vec3 fragPos;

void main() {
    // Get world space positions (before projection)
    vec4 startWorld = gl_in[0].gl_Position;
    vec4 endWorld = gl_in[1].gl_Position;
    
    // Calculate direction vector
    vec3 dir = normalize((endWorld - startWorld).xyz);
    
    // Calculate perpendicular vector (assuming Y is up)
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(dir, up));
    
    // Create offset for road width
    vec3 offset = right * roadWidth;
    
    // Create quad vertices in world space
    vec4 v1 = startWorld - vec4(offset, 0.0);
    vec4 v2 = endWorld - vec4(offset, 0.0);
    vec4 v3 = startWorld + vec4(offset, 0.0);
    vec4 v4 = endWorld + vec4(offset, 0.0);
    
    // Transform to clip space and emit
    gl_Position = projection * view * v1;
    fragPos = v1.xyz;
    EmitVertex();
    
    gl_Position = projection * view * v2;
    fragPos = v2.xyz;
    EmitVertex();
    
    gl_Position = projection * view * v3;
    fragPos = v3.xyz;
    EmitVertex();
    
    gl_Position = projection * view * v4;
    fragPos = v4.xyz;
    EmitVertex();
    
    EndPrimitive();
} 
