#version 460 core

// Maximum number of light sources
#define MAX_LIGHTS 4

// Material properties
uniform vec3 ambient_material, diffuse_material, specular_material;
uniform float specular_shininess;

// Texture sampler
uniform sampler2D uTexture;

// Light properties
uniform int num_lights;
uniform vec3 light_positions[MAX_LIGHTS];
uniform vec3 light_colors[MAX_LIGHTS]; // Add light colors

// Input from vertex shader
in VS_OUT
{
    vec3 N; // normal vector
    vec3 L[MAX_LIGHTS]; // vector from point on object (vertex or rasterized point) towards light source
    vec3 cameraPosition; // Position of the camera in world space
    vec2 texCoord;
} fs_in;

void main(void)
{
    // Normalize the incoming normal vector
    vec3 N = normalize(fs_in.N);

    // Accumulate the contributions from each light source
    vec3 ambient = ambient_material * vec3(1.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    for (int i = 0; i < num_lights; i++) {
        // Normalize the incoming light vector
        vec3 L = normalize(fs_in.L[i]);
        
        // Calculate the reflection vector
        vec3 R = reflect(-L, N);

        // Calculate the view vector in world space
        vec3 viewVector = normalize(fs_in.cameraPosition - gl_FragCoord.xyz);

        // Calculate the diffuse and specular contributions
        diffuse += max(dot(N, L), 0.0) * diffuse_material * light_colors[i]; // Multiply diffuse by light color
        specular += pow(max(dot(R, viewVector), 0.0), specular_shininess) * specular_material * light_colors[i]; // Multiply specular by light color
    }

    // Combine ambient, diffuse, and specular components
    vec3 finalColor = ambient + diffuse + specular;

    // Apply texture
    vec4 texColor = texture(uTexture, fs_in.texCoord);

    // Final color output
    gl_FragColor = vec4(finalColor, 1.0) * texColor;
}
