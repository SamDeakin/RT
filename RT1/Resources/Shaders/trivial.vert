#version 450

layout(location = 0) in vec4 vertex;
layout(location = 0) out vec3 position;

void main() {
    gl_Position = vertex;
    position = vertex.xyz / vertex.w;
}
