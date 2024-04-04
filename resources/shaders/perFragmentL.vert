#version 460 core

// Vertex attributes
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// Matrices
uniform mat4 projection = mat4(1.0);
uniform mat4 transform  = mat4(1.0);
uniform mat4 view       = mat4(1.0);

// Light properties
uniform vec3 light_position;

// Outputs to the fragment shader
out VS_OUT
{
    vec3 N; // normal vector
    vec3 L; // vector from point on object (vertex or rasterised point) towards light source
    vec3 V; // vector towards viewer
} vs_out;

out vec2 texCoord;

void main(void)
{
    // Create Model-View matrix
    mat4 transformView = view * transform;
    // Calculate view-space coordinate - in P point we are computing the color
    vec4 P = transformView * aPos;
    // Calculate normal in view space
    vs_out.N = mat3(transformView) * aNormal;
    // Calculate view-space light vector
    vs_out.L = light_position - P.xyz;
    // Calculate view vector (negative of the view-space position)
    vs_out.V = -P.xyz;
    // Calculate the clip-space position of each vertex
    gl_Position = projection * P;

    texCoord = aTexCoord;
}