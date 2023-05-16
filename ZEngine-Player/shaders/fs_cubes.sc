
$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(texColor, 0);

void main()
{
	gl_FragColor = texture2D(texColor, v_texcoord0) * v_color0;
}
