#version 460 core
in vec2 v_texCoord;
in vec4 v_color;
in float v_texIndex;
in float v_tilingFactor;

uniform sampler2D u_textures[32];

out vec4 color;

void main() {
    vec4 texColor = v_color;
    if (v_texIndex > 0.0) {
        int index = int(v_texIndex);
        texColor *= texture(u_textures[index], v_texCoord * v_tilingFactor);
    }
    color = texColor;
}
