$input a_position, a_color0, a_texcoord0
$output v_color0, v_texcoord0

#include "lib/bgfx_shader.sh"

void main()
{
	gl_Position = mul(u_modelViewProj, mul(vec4(a_position, 1.0), vec4(2, 2, 2, 1)));
	v_color0 = a_color0 * vec4(0, 1, 0, 1);
	v_texcoord0 = a_texcoord0;
}
