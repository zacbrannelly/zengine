$input a_position, a_color0, a_texcoord0, a_normal
$output v_position, v_color0, v_normal

#include "lib/bgfx_shader.sh"

void main()
{
  // Transform the vertex position to world space
  vec4 worldPos = mul(u_model[0], vec4(a_position, 1.0));
  v_position = worldPos;

  // Transform the normal to world space (making sure to ignore translation).
  v_normal = mul(mat3(u_model[0]), a_normal);

  // Transfer the color to the fragment shader
  v_color0 = a_color0;

  // Transform the world position to clip space
  gl_Position = mul(u_viewProj, worldPos);
}
