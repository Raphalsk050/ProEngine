
#type vertex
#version 450 core

// Inputs do vertex buffer
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec2 a_TexCoord;

// Uniform buffers
layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
    vec3 u_CameraPosition;
    float _padding;
};

layout(std140, binding = 1) uniform Light
{
    vec3 u_PointLightPosition;
    float u_PointLightIntensity;
    vec3 u_AmbientLightColor;
    float u_AmbientLightIntensity;
};

// Uniforms individuais
uniform mat4 u_Transform;

// Outputs para o fragment shader
layout(location = 0) out vec3 v_WorldPos;
layout(location = 1) out vec3 v_Normal;
layout(location = 2) out vec2 v_TexCoord;

void main()
{
    vec4 worldPos = u_Transform * vec4(a_Position, 1.0);
    v_WorldPos = worldPos.xyz;

    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;

    v_TexCoord = a_TexCoord;

    gl_Position = u_ViewProjection * worldPos;
}

#type fragment
#version 450 core

// Inputs do vertex shader
layout(early_fragment_tests) in;
layout(location = 0) in vec3 v_WorldPos;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec2 v_TexCoord;

// Output
layout(location = 0) out vec4 o_Color;

// Uniform buffers
layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
    vec3 u_CameraPosition;
    float _padding;
};

layout(std140, binding = 1) uniform Light
{
    vec3 u_PointLightPosition;
    float u_PointLightIntensity;
    vec3 u_AmbientLightColor;
    float u_AmbientLightIntensity;
};

// Material uniforms
uniform vec4 u_MaterialAlbedoColor;
uniform float u_MaterialMetallic;
uniform float u_MaterialRoughness;
uniform int u_EntityID;

// Texture samplers
layout(binding = 0) uniform sampler2D u_AlbedoMap;
layout(binding = 1) uniform sampler2D u_NormalMap;
layout(binding = 2) uniform sampler2D u_MetallicMap;
layout(binding = 3) uniform sampler2D u_RoughnessMap;

void main()
{
    // Sample material properties
    vec4 albedoSample = texture(u_AlbedoMap, v_TexCoord);
    vec4 finalAlbedo = albedoSample * u_MaterialAlbedoColor;

    // Normalize the normal
    vec3 normal = normalize(v_Normal);

    // Simple directional lighting
    vec3 lightDir = normalize(u_PointLightPosition - v_WorldPos);
    float NdotL = max(dot(normal, lightDir), 0.0);

    // Calculate lighting - CORRIGIDO: garantir que todos são vec3
    vec3 ambient = u_AmbientLightColor * u_AmbientLightIntensity;
    vec3 diffuse = vec3(NdotL) * vec3(u_PointLightIntensity); // Corrigido: explicit vec3 cast

    // Combine lighting with material
    vec3 lighting = ambient + diffuse;
    vec3 finalColor = finalAlbedo.rgb * lighting;

    // Output final color
    o_Color = vec4(finalColor, finalAlbedo.a);
}