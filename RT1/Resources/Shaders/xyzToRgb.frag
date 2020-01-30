#version 450

layout(location = 0) out vec4 colour;

void main() {
    colour = vec4(abs(gl_FragCoord.xyz), 1.0);
}
