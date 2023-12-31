#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_textureCoord;

uniform mat4 u_viewProjection;
uniform mat4 u_transform;

out vec2 v_textureCoord;

void main()
{
	v_textureCoord = a_textureCoord;
	gl_Position = u_viewProjection * u_transform * vec4(a_position, 1.0);
}



#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_textureCoord;

uniform sampler2D u_texture;

void main()
{
	color = texture(u_texture, v_textureCoord);
}