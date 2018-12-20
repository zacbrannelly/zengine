$input v_color0, v_texcoord0

uniform sampler2D texColor;

void main()
{
	gl_FragColor = texture2D(texColor, v_texcoord0) * v_color0; 
}
