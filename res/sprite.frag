#version 450 core

in vec2 v_uv;
in float v_layer;

out vec4 finalColor;

uniform sampler2DArray u_textures;

void main() {
    vec4 color = texture(u_textures, vec3(v_uv, v_layer));
    finalColor = color;
}
