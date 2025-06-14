#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

layout(std140, binding = 0) uniform Camera
{
    mat4 u_ViewProjection;
    vec3 u_CameraPosition;
    float _padding;
};

uniform mat4 u_Transform;

void main()
{
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

void main()
{
    // Shader vazio - só queremos escrever depth
    // O OpenGL automaticamente escreve gl_FragDepth
}