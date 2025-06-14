#type vertex
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Tangent;
layout(location = 3) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
    vec3 u_CameraPosition;
    float _padding;
};

uniform mat4 u_Transform;
uniform int u_EntityID; // ← ADICIONADO: Uniform para entity ID

layout(location = 0) out flat int v_EntityID;

void main()
{
    vec4 worldPosition = u_Transform * vec4(a_Position, 1.0);
    gl_Position = u_ViewProjection * worldPosition;

    v_EntityID = u_EntityID;
}

#type fragment
#version 450 core
layout(early_fragment_tests) in;
layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

layout(location = 0) in flat int v_EntityID;

// Material uniform buffer
uniform vec4 u_Color;

void main()
{
    vec4 finalColor = u_Color;

    finalColor = vec4(1.0, 0.0, 1.0, 1.0);
    o_Color = finalColor;
    o_EntityID = v_EntityID;
}