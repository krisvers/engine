#version 330 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_uv;

out vec3 pass_color;
out vec2 pass_uv;
uniform mat4 in_mvp;

void main() {
	gl_Position = in_mvp * vec4(in_pos, 1.0);
	pass_color = in_color;
	pass_uv = in_uv;
}