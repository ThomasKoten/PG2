#version 460 core

// Maximum number of light sources
#define MAX_LIGHTS 4

// Vertex attributes
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

// Matrices
uniform mat4 projection = mat4(1.0);
uniform mat4 view       = mat4(1.0); // View matrix
uniform mat4 transform  = mat4(1.0); // Model matrix

// Light properties
uniform int num_lights;
uniform vec3 light_positions[MAX_LIGHTS];
uniform vec3 light_colors[MAX_LIGHTS];

// Outputs to the fragment shader
out VS_OUT
{
    vec3 N; // normal vector
    vec3 L[MAX_LIGHTS]; // vector from point on object (vertex or rasterized point) towards light source
    vec3 cameraPosition; // Position of the camera in world space
    vec2 texCoord;
} vs_out;

void main(void)
{
    // Transform the vertex position into world space
    vec4 worldPos = transform * aPos;

    // Calculate normal in world space
    vs_out.N = mat3(transpose(inverse(transform))) * aNormal;

    // Calculate view vector (direction from the vertex to the camera) in world space
    vs_out.cameraPosition = vec3(view[3]); // Extract camera position from view matrix

    // Calculate light vectors and sum up their contributions
    for (int i = 0; i < num_lights; i++) {
        // Calculate world-space light vector for each light source
        vs_out.L[i] = light_positions[i] - worldPos.xyz;
    }

    // Calculate the clip-space position of each vertex
    gl_Position = projection * view * worldPos;

    // Pass texture coordinates to fragment shader
    vs_out.texCoord = aTexCoord;
}
