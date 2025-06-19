
#type vertex
#version 410 core

// Inputs do vertex buffer
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec2 a_TexCoord;

// Uniform buffers
layout(std140) uniform Camera
{
    mat4 u_ViewProjection;
    vec3 u_CameraPosition;
    float _padding;
};

layout(std140) uniform Light
{
    vec3 u_PointLightPosition;
    float u_PointLightIntensity;
    vec3 u_AmbientLightColor;
    float u_AmbientLightIntensity;
};

// Uniforms individuais
uniform mat4 u_Transform;
uniform int u_EntityID;

// Outputs para o fragment shader
layout(location = 0) out vec3 v_WorldPos;
layout(location = 1) out vec3 v_Normal;
layout(location = 2) out vec2 v_TexCoord;
layout(location = 3) flat out int v_EntityID;

void main()
{
    vec4 worldPos = u_Transform * vec4(a_Position, 1.0);
    v_WorldPos = worldPos.xyz;

    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;

    v_TexCoord = a_TexCoord;

    gl_Position = u_ViewProjection * worldPos;

    v_EntityID = u_EntityID;
}

#type fragment
#version 410 core

// Inputs do vertex shader
layout(location = 0) in vec3 v_WorldPos;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec2 v_TexCoord;
layout(location = 3) flat in int v_EntityID;

// Output
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

// Uniform buffers
layout(std140) uniform Camera
{
    mat4 u_ViewProjection;
    vec3 u_CameraPosition;
    float _padding;
};

layout(std140) uniform Light
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
uniform vec2 u_MaterialTilingFactor;
uniform int u_EntityID;

// Texture samplers
uniform sampler2D u_AlbedoMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_MetallicMap;
uniform sampler2D u_RoughnessMap;
uniform int u_UseGrid;

vec4 GridColor(vec2 uv)
{
    float thickness = 0.02;
    vec2 grid = abs(fract(uv - 0.5) - 0.5);
    float line = step(grid.x, thickness) + step(grid.y, thickness);
    line = clamp(line, 0.0, 1.0);
    vec3 bg = vec3(0.2);
    vec3 color = mix(bg, u_MaterialAlbedoColor.rgb, line);
    return vec4(color, 1.0);
}

void main()
{
    // Sample material properties
    vec4 albedoSample = texture(u_AlbedoMap, v_TexCoord * u_MaterialTilingFactor);
    vec4 finalAlbedo = albedoSample * u_MaterialAlbedoColor;
    if(u_UseGrid == 1)
        finalAlbedo = GridColor(v_TexCoord * u_MaterialTilingFactor);

    // Normalize the normal
    vec3 normal = normalize(v_Normal);

    // Simple directional lighting
    vec3 lightDir = normalize(u_PointLightPosition - v_WorldPos);
    float NdotL = max(dot(normal, lightDir), 0.0);

    // Calculate lighting - CORRIGIDO: garantir que todos são vec3
    vec3 ambient = u_AmbientLightColor * u_AmbientLightIntensity;
    vec3 diffuse = vec3(NdotL) * vec3(u_PointLightIntensity);

    // Combine lighting with material
    vec3 lighting = ambient + diffuse;
    vec3 finalColor = finalAlbedo.rgb * lighting;
    if(finalAlbedo.a <= 0.1){
       discard;
    }
    // Output final color
    o_Color = vec4(finalColor,1.0f);
    o_EntityID = v_EntityID;
}
