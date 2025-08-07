$input v_position, v_color0, v_normal

#include "./lib/bgfx_shader.sh"
#include "./lib/standard_lighting.sh"

#define MAX_LIGHTS 16
#define MAX_CASCADES 4
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

uniform vec4 u_cascadeDepths[MAX_CASCADES];
uniform mat4 u_lightViewProjections[MAX_CASCADES];
uniform vec4 u_depthBiasSlope;
uniform vec4 u_depthBiasConstant;

SAMPLER2DARRAY(s_cascadeShadowMap, 0);

// Material uniforms (managed by the programmer)
uniform vec4 albedoTint;
uniform vec4 roughness;
uniform vec4 metallic;

void main()
{
  vec3 normal = normalize(v_normal.xyz);
  vec3 vertexToCamera = normalize(u_cameraPos.xyz - v_position.xyz);

  // TODO: Move this to the vertex shader
  vec4 fragPosInViewSpace = mul(u_view, vec4(v_position.xyz, 1.0));

  // Figure out which cascade the fragment is in
  int cascadeIndex = 0;
  for (int i = 0; i < MAX_CASCADES; i++)
  {
    float near = u_cascadeDepths[i].z;
    float far = i == MAX_CASCADES - 1 ? 100 : u_cascadeDepths[i + 1].z;

    // Check if the fragment is in the current cascade
    if (fragPosInViewSpace.z >= near && fragPosInViewSpace.z < far)
    {
      cascadeIndex = i;
      break;
    }
  }

  // Start with the unlit color
  vec4 outColor = vec4(0.0, 0.0, 0.0, 1.0);

  for (int i = 0; i < int(u_lightCount.x); i++)
  {
    if (int(u_lightTypes[i].x) == LIGHT_TYPE_DIRECTIONAL)
    {
      // Directional light
      vec3 lightDirection = normalize(u_lightDirections[i].xyz);
      vec3 lightColor = u_lightIntensities[i].x * u_lightColors[i].xyz;

      // Calculate the shadow factor
      float shadowFactor = 1.0;

      // Calculate the frag pos in the cascade's NDC space.
      vec4 fragPosInLightClipSpace = mul(u_lightViewProjections[cascadeIndex], vec4(v_position.xyz, 1.0));
      vec3 fragPosInLightNDC = fragPosInLightClipSpace.xyz / fragPosInLightClipSpace.w;

      // TODO remap z from [-1,1] → [0,1] when not using homogenous depth.
      float currentDepth = fragPosInLightNDC.z;

      // Sample the shadow map
      vec2 shadowMapCoords = fragPosInLightNDC.xy * 0.5 + 0.5;

      // TODO: This is only needed when texture origin is top-left (BGFX caps originBottomLeft is false).
      // TODO: Make this optional.
      shadowMapCoords.y = 1.0 - shadowMapCoords.y;

      vec4 shadowMapSample = texture2DArray(s_cascadeShadowMap, vec3(shadowMapCoords.xy, cascadeIndex));
      float shadowMapDepth = shadowMapSample.r;

      float depthBiasSlope = u_depthBiasSlope.x;
      float depthBiasConstant = u_depthBiasConstant.x;

      float nDotL = max(dot(normal, -lightDirection), 0.0);
      float slopeBias = depthBiasSlope * (1.0 - nDotL);

      float bias = max(depthBiasConstant, slopeBias);

      if (currentDepth - bias > shadowMapDepth)
      {
        // The fragment is in shadow
        shadowFactor = 0.2;
      }
      else
      {
        // The fragment is in light
        shadowFactor = 1.0;
      }

      outColor.rgb += calculateDirectionalLightContribution(
        normal,
        v_color0.rgb * albedoTint.rgb,
        roughness.x,
        metallic.x,
        vertexToCamera,
        lightDirection,
        lightColor
      ) * shadowFactor;
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
