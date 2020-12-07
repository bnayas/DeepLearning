#shader vertex
#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float a_ColorIdx;

out vec2 v_TexCoord;
out float v_ColorIdx;
uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_ColorIdx = a_ColorIdx;
};

#shader fragment
#version 430 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_ColorIdx;

uniform	sampler2D u_Texture;

void main()
{
	if (v_ColorIdx == 0)
	{
		vec4 texColor = texture(u_Texture, v_TexCoord);
		color = texColor;
		
	}
	else
	{
		color = vec4(1);
	}
};