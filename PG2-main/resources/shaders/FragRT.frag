#version 460 core

#define MAX_LIGHTS 4

uniform vec3 ambient_material, diffuse_material, specular_material;
uniform float specular_shininess;

uniform sampler2D uTexture;

uniform int num_point_lights;
uniform vec3 point_light_positions[MAX_LIGHTS];
uniform vec3 point_light_colors[MAX_LIGHTS];
uniform float point_light_constants[MAX_LIGHTS];
uniform float point_light_linears[MAX_LIGHTS];
uniform float point_light_quadratics[MAX_LIGHTS];

uniform int num_directional_lights;
uniform vec3 directional_light_directions[MAX_LIGHTS];
uniform vec3 directional_light_colors[MAX_LIGHTS];

uniform int num_spot_lights;
uniform vec3 spot_light_positions[MAX_LIGHTS];
uniform vec3 spot_light_directions[MAX_LIGHTS];
uniform vec3 spot_light_colors[MAX_LIGHTS];
uniform float spot_light_cutoffs[MAX_LIGHTS];

in VS_OUT {
    vec3 N;
    vec3 L[MAX_LIGHTS];
    vec3 cameraPosition;
    vec2 texCoord;
    vec3 directional_L[MAX_LIGHTS];
    vec3 spot_L[MAX_LIGHTS];
    vec3 spot_D[MAX_LIGHTS];
    float spot_cutoff[MAX_LIGHTS];
} fs_in;

void main(void)
{
    vec3 N = normalize(fs_in.N);
    vec3 viewVector = normalize(fs_in.cameraPosition - gl_FragCoord.xyz);

    vec3 ambient = ambient_material * vec3(1.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);

    // Point lights with attenuation
    for (int i = 0; i < num_point_lights; i++) {
        vec3 L = normalize(fs_in.L[i]);
        float distance = length(fs_in.L[i]);
        float attenuation = 1.0 / (point_light_constants[i] + point_light_linears[i] * distance + point_light_quadratics[i] * (distance * distance));
        vec3 R = reflect(-L, N);
        diffuse += attenuation * max(dot(N, L), 0.0) * diffuse_material * point_light_colors[i];
        specular += attenuation * pow(max(dot(R, viewVector), 0.0), specular_shininess) * specular_material * point_light_colors[i];
    }

    // Directional lights
    for (int i = 0; i < num_directional_lights; i++) {
        vec3 L = fs_in.directional_L[i];
        vec3 R = reflect(-L, N);
        diffuse += max(dot(N, L), 0.0) * diffuse_material * directional_light_colors[i];
        specular += pow(max(dot(R, viewVector), 0.0), specular_shininess) * specular_material * directional_light_colors[i];
    }

    // Spotlights
    for (int i = 0; i < num_spot_lights; i++) {
        vec3 L = normalize(fs_in.spot_L[i]);
        vec3 D = fs_in.spot_D[i];
        float theta = dot(L, D);
        if (theta > fs_in.spot_cutoff[i]) {
            vec3 R = reflect(-L, N);
            diffuse += max(dot(N, L), 0.0) * diffuse_material * spot_light_colors[i];
            specular += pow(max(dot(R, viewVector), 0.0), specular_shininess) * specular_material * spot_light_colors[i];
        }
    }

    vec3 finalColor = ambient + diffuse + specular;
    vec4 texColor = texture(uTexture, fs_in.texCoord);
    gl_FragColor = vec4(finalColor, 1.0) * texColor;
}
