#version 450 core

in vec2 v_uv;
in vec4 v_color;
in float v_layer;

out vec4 finalColor;

uniform sampler2DArray u_textures;

void main() {
    float color = texture(u_textures, vec3(v_uv, v_layer)).r;
    finalColor = v_color * color;
}
