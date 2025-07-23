#version 460 core
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in vec4 a_color;
layout(location = 3) in float a_texIndex;
layout(location = 4) in float a_tilingFactor;

uniform mat4 u_viewProjection;

out vec4 v_color;

void main() {
    v_color = a_color;
    gl_Position = u_viewProjection * vec4(a_position, 1.0);
}
