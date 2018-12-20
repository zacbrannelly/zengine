$input a_position, a_color0, a_normal, a_texcoord0
$output v_color0, v_texcoord0, v_normal, v_fragPos

#include "lib/bgfx_shader.sh"

void main()
{
    v_texcoord0 = a_texcoord0;
    v_fragPos = vec3(u_model[0] * vec4(a_position, 1.0));
    v_normal = mat3(u_model[0]) * a_normal;   
	v_color0 = a_color0;
	
    gl_Position =  u_proj * u_view * vec4(v_fragPos, 1.0); 
}