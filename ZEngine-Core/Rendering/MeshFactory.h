#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace ZEngine
{
	class Mesh;

	enum PlaneOrientation
	{
		FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM
	};

	class MeshFactory
	{
	public:
		
		static Mesh* CreateCube(std::string name);
		static Mesh* CreateCubeOutline(std::string name);
		static Mesh* CreateSphere(std::string name, int resolution = 30);
		static Mesh* CreateSphereOutline(std::string name, int latitudeBands, int longitudeBands);
		static Mesh* CreateHemiSphereOutline(std::string name, int latitudeBands, int longitudeBands);
		static Mesh* CreateCylinderOutline(std::string name, int radialSegments, float height);
		static Mesh* CreateRectangle(std::string name);
		static Mesh* CreatePlane(std::string name, int width, int height, PlaneOrientation facing = PlaneOrientation::FRONT);

		static Mesh* CreateSphereStrip(std::string name);

	private:
		static void GeneratePlaneVertices(const glm::vec3& pos, int width, int height, std::vector<glm::vec3>& vertices, std::vector<uint32_t>& indices, PlaneOrientation facing);
	};
}
