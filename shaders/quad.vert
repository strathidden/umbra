#version 460 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in vec4 a_color;
layout(location = 3) in float a_texIndex;
layout(location = 4) in float a_tilingFactor;

uniform mat4 u_viewProjection;

out vec2 v_texCoord;
out vec4 v_color;
out float v_texIndex;
out float v_tilingFactor;

void main() {
    v_texCoord = a_texCoord;
    v_color = a_color;
    v_texIndex = a_texIndex;
    v_tilingFactor = a_tilingFactor;
    gl_Position = u_viewProjection * vec4(a_position, 1.0);
}
