#ifndef ZENGINE_STANDARD_LIGHTING_H
#define ZENGINE_STANDARD_LIGHTING_H

#include "bgfx_shader.sh"

#define PI 3.14159265359

vec3 fernelSchlick(vec3 f0, float cosTheta)
{
  return f0 + (1.0 - f0) * pow(1.0 - cosTheta, 5.0);
}

float g1(float NdotV, float k)
{
  return NdotV / (NdotV * (1.0 - k) + k);
}

float roughnessMappingForImageBasedLighting(float roughness)
{
  // Mapping of roughness source: https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
  return pow(roughness, 2) / 2;
}

float roughnessMappingForDirectLighting(float rougness) 
{
  // Mapping of roughness source: https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
  return pow(rougness + 1.0, 2) / 8;
}

float geometricAttenuation(float NdotL, float NdotV, float mappedRoughness)
{
  float gV = g1(NdotV, mappedRoughness);
  float gL = g1(NdotL, mappedRoughness);
	return gV * gL;
}

float ggxNormalDistribution(float roughness, float NdotH)
{
  float alpha = roughness * roughness;
  float alphaSqr = alpha * alpha;
  float NdotHSqr = NdotH * NdotH;

  float denom = NdotHSqr * (alphaSqr - 1.0) + 1.0;

  return alphaSqr / (PI * denom * denom);
}

vec3 diffuseLambertsCosine(float NdotL, vec3 surfaceColor)
{
  return (surfaceColor * NdotL) / PI;
}

vec3 calculateLightContribution(
  vec3 lightDir,
  vec3 surfaceNormal,
  vec3 viewDir,
  vec3 lightColor,
  vec3 surfaceColor,
  float roughness,
  float mappedRoughness,
  float metallic
) {
  // Half vector between light and view direction
  vec3 h = normalize(lightDir + viewDir);

  float NdotL = max(dot(surfaceNormal, lightDir), 0.0);
  float NdotV = max(dot(surfaceNormal, viewDir), 0.0);

  float NdotH = max(dot(surfaceNormal, h), 0.0);
  float VdotH = max(dot(viewDir, h), 0.0);

  // Calculate the Fresnel-Schlick approximation for specular reflection (F)
  // F(v,h) = F0 + (1 - F0) * (1 - (v.h))^5
  vec3 F0 = mix(vec3(0.04), surfaceColor, metallic);
  vec3 F = fernelSchlick(F0, VdotH);

  // Calculate the geometric attenuation (G)
  float G = geometricAttenuation(NdotL, NdotV, mappedRoughness);

  // Calculate the specular reflection / normal distribution function (D)
  float D = ggxNormalDistribution(roughness, NdotH);

  // Calculate the specular reflection
  float denom = 4.0 * NdotL * NdotV + 0.001;
  vec3 specular = (F * G * D) / denom;

  // Calculate the diffuse reflection
  vec3 diffuse = diffuseLambertsCosine(NdotL, surfaceColor);

  return (diffuse + specular) * lightColor * NdotL;
}

vec3 calculateDirectLightContribution(
  vec3 lightDir,
  vec3 surfaceNormal,
  vec3 viewDir,
  vec3 lightColor,
  vec3 surfaceColor,
  float roughness,
  float metallic
) {
  float mappedRoughness = roughnessMappingForDirectLighting(roughness);
  return calculateLightContribution(
    lightDir,
    surfaceNormal,
    viewDir,
    lightColor,
    surfaceColor,
    roughness,
    mappedRoughness,
    metallic
  );
}

vec3 calculateImageBasedLightContribution(
  vec3 lightDir,
  vec3 surfaceNormal,
  vec3 viewDir,
  vec3 lightColor,
  vec3 surfaceColor,
  float roughness,
  float metallic
) {
  float mappedRoughness = roughnessMappingForImageBasedLighting(roughness);
  return calculateLightContribution(
    lightDir,
    surfaceNormal,
    viewDir,
    lightColor,
    surfaceColor,
    roughness,
    mappedRoughness,
    metallic
  );
}

vec3 calculatePointLightContribution(
  vec3 surfaceNormal,
  vec3 surfaceColor,
  vec3 surfacePos,
  float roughness,
  float metallic,
  vec3 viewDir,
  vec3 lightPos,
  vec3 lightColor,
  float lightRadius
) {
  vec3 lightDir = normalize(lightPos - surfacePos);
  float distance = length(lightPos - surfacePos);
  float attenuation = clamp(1.0 - (distance / lightRadius), 0.0, 1.0);

  vec3 lightContribution = calculateDirectLightContribution(
    lightDir,
    surfaceNormal,
    viewDir,
    lightColor,
    surfaceColor,
    roughness,
    metallic
  );

  return lightContribution * attenuation;
}

vec3 calculateDirectionalLightContribution(
  vec3 surfaceNormal,
  vec3 surfaceColor,
  float roughness,
  float metallic,
  vec3 viewDir,
  vec3 lightDir,
  vec3 lightColor
) {
  return calculateDirectLightContribution(
    lightDir,
    surfaceNormal,
    viewDir,
    lightColor,
    surfaceColor,
    roughness,
    metallic
  );
}

vec3 calculateSpotLightContribution(
  vec3 surfaceNormal,
  vec3 surfaceColor,
  vec3 surfacePos,
  float roughness,
  float metallic,
  vec3 viewDir,
  vec3 lightPos,
  vec3 spotLightDir,
  vec3 lightColor,
  float lightRadius,
  float innerAngle,
  float outerAngle
) {
  vec3 lightDir = normalize(lightPos - surfacePos);
  float distance = length(lightPos - surfacePos);
  float attenuation = clamp(1.0 - (distance / lightRadius), 0.0, 1.0);

  float spotLightAngle = dot(spotLightDir, -lightDir);
  float penumbraAngle = cos(innerAngle);
  float umbraAngle = cos(outerAngle);

  float tNom = spotLightAngle - umbraAngle;
  float tDenom = penumbraAngle - umbraAngle;
  float t = clamp(tNom / tDenom, 0.0, 1.0);

  attenuation *= t * t;

  vec3 lightContribution = calculateDirectLightContribution(
    lightDir,
    surfaceNormal,
    viewDir,
    lightColor,
    surfaceColor,
    roughness,
    metallic
  );

  return lightContribution * attenuation;
}

#endif // ZENGINE_STANDARD_LIGHTING_H
