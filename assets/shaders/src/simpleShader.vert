#version 450

#extension GL_ARB_separate_shader_objects:enable

vec2 postions[3] = vec2[](vec2(0.0, -1.0), vec2(0.5, 0.0), vec2(-0.5, 0.0));
vec3 colors[3] = vec3[](vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 1.0));

layout(location = 0) out vec3 outColor;

void main()
{
	gl_Position = vec4(postions[gl_VertexIndex], 0.0, 1.0);
	outColor = colors[gl_VertexIndex];
}