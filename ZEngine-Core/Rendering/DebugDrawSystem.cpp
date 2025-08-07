#include "DebugDrawSystem.h"
#include "MaterialFactory.h"
#include "MeshFactory.h"
#include "Mesh.h"

#include <iostream>

using namespace ZEngine;

void DebugDrawSystem::Init()
{
  _debugMaterial = MaterialFactory::CreateUnlitVertexColorMaterial("DebugGizmoMaterial");
}

void DebugDrawSystem::DrawLines(const std::vector<glm::vec3>& points, const glm::vec4& color)
{
  DrawLines(glm::vec3(0.0f), points, color);
}

void DebugDrawSystem::DrawLines(const glm::vec3& position, const std::vector<glm::vec3>& points, const glm::vec4& color)
{
  DebugDrawData drawData;
  drawData.position = position;
  drawData.type = DebugMeshType::DEBUG_LINES;
  drawData.points = points;
  drawData.color = color;
  _debugDrawList.push_back(drawData);
}

void DebugDrawSystem::DrawPolygonOutline(const std::vector<glm::vec3>& points, const glm::vec4& color)
{
  // Each point is a vertex, and we need to connect them in a loop
  std::vector<glm::vec3> outlinePoints;
  for (size_t i = 0; i < points.size(); ++i)
  {
    outlinePoints.push_back(points[i]);
    outlinePoints.push_back(points[(i + 1) % points.size()]); // Connect to the next point
  }
  
  DrawLines(outlinePoints, color);
}

void DebugDrawSystem::DrawRectangle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
{
  std::vector<glm::vec3> rectPoints;
  // Bottom line
  rectPoints.push_back({ -0.5f, -0.5f, 0 });
  rectPoints.push_back({ 0.5f, -0.5f, 0 });

  // Left line
  rectPoints.push_back({ -0.5f,  0.5f, 0 });
  rectPoints.push_back({ -0.5f, -0.5f, 0 });

  // Right line
  rectPoints.push_back({ 0.5f,  0.5f, 0 });
  rectPoints.push_back({ 0.5f, -0.5f, 0 });

  // Top line
  rectPoints.push_back({ 0.5f,  0.5f, 0 });
  rectPoints.push_back({ -0.5f,  0.5f, 0 });

  // Transform the rectangle points to the specified position and size
  for (auto& point : rectPoints)
  {
    point.x *= size.x;
    point.y *= size.y;
    point += position;
  }

  DrawLines(rectPoints, color);
}

void DebugDrawSystem::Render(int viewId)
{
  _frameCount++;

  if (_frameCount % 30 == 0)
  {
    // Clean up the kill list every 10 frames
    for (auto& mesh : _killList)
    {
      delete mesh;
    }
    _killList.clear();
  }

  if (_debugDrawList.empty()) return;

  for (auto& drawData : _debugDrawList)
  {
    if (drawData.type == DEBUG_LINES)
    {
      auto mesh = MeshFactory::CreateOutline("DebugMesh", drawData.points, drawData.color);
      auto transform = glm::translate(glm::mat4(1.0f), drawData.position);
      mesh->Draw(viewId, { _debugMaterial }, transform);

      // Delete the mesh after rendering
      _killList.push_back(mesh);
    }
  }
  _debugDrawList.clear();
}

void DebugDrawSystem::Shutdown()
{
  _debugDrawList.clear();

  delete _debugMaterial;
  _debugMaterial = nullptr;

  for (auto& mesh : _killList)
  {
    delete mesh;
  }
  _killList.clear();
}
