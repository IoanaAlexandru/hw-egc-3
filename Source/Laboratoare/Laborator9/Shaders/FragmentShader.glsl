#version 330
 
uniform sampler2D texture;
uniform float time;
 
in vec2 texcoord;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = texture2D(texture, texcoord + vec2(time / 4, 0));
	if (out_color.a < 0.75)
		discard;
}