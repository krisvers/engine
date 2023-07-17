#version 330 core

out vec4 out_color;
in vec3 pass_color;
in vec2 pass_uv;
uniform sampler2D in_sampler;

void main() {
	out_color = /*vec4(pass_color, 1.0) +*/ texture(in_sampler, pass_uv);
}