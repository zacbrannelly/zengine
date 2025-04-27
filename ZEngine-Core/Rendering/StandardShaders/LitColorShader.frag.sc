$input v_position, v_color0, v_normal

#include "./lib/bgfx_shader.sh"
#include "./lib/standard_lighting.sh"

#define MAX_LIGHTS 16
#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

// Standard lighting uniforms (managed by the engine)
uniform vec4 u_lightCount;
uniform vec4 u_cameraPos;
uniform vec4 u_lightTypes[MAX_LIGHTS];
uniform vec4 u_lightColors[MAX_LIGHTS];
uniform vec4 u_lightIntensities[MAX_LIGHTS];
uniform vec4 u_lightPositions[MAX_LIGHTS];
uniform vec4 u_lightDirections[MAX_LIGHTS];
uniform vec4 u_lightRanges[MAX_LIGHTS];
uniform vec4 u_lightPrenumbraAndUmbra[MAX_LIGHTS];

// Material uniforms (managed by the programmer)
uniform vec4 albedoTint;
uniform vec4 roughness;
uniform vec4 metallic;

void main()
{
  vec3 normal = normalize(v_normal.xyz);
  vec3 vertexToCamera = normalize(u_cameraPos.xyz - v_position.xyz);

  // Start with the unlit color
  vec4 outColor = vec4(0.0, 0.0, 0.0, 1.0);

  for (int i = 0; i < int(u_lightCount.x); i++)
  {
    if (int(u_lightTypes[i].x) == LIGHT_TYPE_DIRECTIONAL)
    {
      // Directional light
      vec3 lightDirection = normalize(u_lightDirections[i].xyz);
      vec3 lightColor = u_lightIntensities[i].x * u_lightColors[i].xyz;

      outColor.rgb += calculateDirectionalLightContribution(
        normal,
        v_color0.rgb * albedoTint.rgb,
        roughness.x,
        metallic.x,
        vertexToCamera,
        lightDirection,
        lightColor
      );
    }
    else if (int(u_lightTypes[i].x) == LIGHT_TYPE_POINT)
    {
      // Point light
      vec3 lightPosition = u_lightPositions[i].xyz;
      vec3 lightColor = u_lightIntensities[i].x * u_lightColors[i].xyz;

      outColor.rgb += calculatePointLightContribution(
        normal,
        v_color0.rgb * albedoTint.rgb,
        v_position.xyz,
        roughness.x,
        metallic.x,
        vertexToCamera,
        lightPosition,
        lightColor,
        u_lightRanges[i].x
      );
    }
    else if (int(u_lightTypes[i].x) == LIGHT_TYPE_SPOT)
    {
      // Spot light
      vec3 lightPosition = u_lightPositions[i].xyz;
      vec3 lightDirection = normalize(u_lightDirections[i].xyz);
      vec3 lightColor = u_lightIntensities[i].x * u_lightColors[i].xyz;

      outColor.rgb += calculateSpotLightContribution(
        normal,
        v_color0.rgb * albedoTint.rgb,
        v_position.xyz,
        roughness.x,
        metallic.x,
        vertexToCamera,
        lightPosition,
        lightDirection,
        lightColor,
        u_lightRanges[i].x,
        u_lightPrenumbraAndUmbra[i].x,
        u_lightPrenumbraAndUmbra[i].y
      );
    }
  }

  gl_FragColor = outColor;
}
