#version 460 core

#define MAX_LIGHTS 4

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 projection = mat4(1.0);
uniform mat4 view = mat4(1.0); // View matrix
uniform mat4 transform = mat4(1.0); // Model matrix

uniform int num_point_lights;
uniform vec3 point_light_positions[MAX_LIGHTS];
uniform vec3 point_light_colors[MAX_LIGHTS];

uniform int num_directional_lights;
uniform vec3 directional_light_directions[MAX_LIGHTS];
uniform vec3 directional_light_colors[MAX_LIGHTS];

uniform int num_spot_lights;
uniform vec3 spot_light_positions[MAX_LIGHTS];
uniform vec3 spot_light_directions[MAX_LIGHTS];
uniform float spot_light_cutoffs[MAX_LIGHTS]; // cosine of the cutoff angle
uniform vec3 spot_light_colors[MAX_LIGHTS];

// Outputs to the fragment shader
out VS_OUT {
    vec3 N;
    vec3 L[MAX_LIGHTS];
    vec3 cameraPosition;
    vec2 texCoord;
    vec3 directional_L[MAX_LIGHTS];
    vec3 spot_L[MAX_LIGHTS];
    vec3 spot_D[MAX_LIGHTS];
    float spot_cutoff[MAX_LIGHTS];
} vs_out;

void main(void)
{
    vec4 worldPos = transform * aPos;
    vs_out.N = mat3(transpose(inverse(transform))) * aNormal;
    vs_out.cameraPosition = vec3(view[3]);

    for (int i = 0; i < num_point_lights; i++) {
        vs_out.L[i] = point_light_positions[i] - worldPos.xyz;
    }
    
    for (int i = 0; i < num_directional_lights; i++) {
        vs_out.directional_L[i] = -normalize(directional_light_directions[i]);
    }
    
    for (int i = 0; i < num_spot_lights; i++) {
        vs_out.spot_L[i] = spot_light_positions[i] - worldPos.xyz;
        vs_out.spot_D[i] = normalize(spot_light_directions[i]);
        vs_out.spot_cutoff[i] = spot_light_cutoffs[i];
    }

    gl_Position = projection * view * worldPos;
    vs_out.texCoord = aTexCoord;
}
