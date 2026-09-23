#version 450 core

in vec2 a_position;
in vec2 a_uv;
in float a_layer;

out vec2 v_uv;
out float v_layer;

uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * vec4(a_position, 0.0, 1.0);

    v_uv = a_uv;
    v_layer = a_layer;
}
