#pragma once

#include "Component.h"
#include <vector>

class Mesh;
class Material;

class MeshRenderer : public Component
{
public:
	MeshRenderer();
	~MeshRenderer();

	void SetMesh(Mesh* mesh);
	Mesh* GetMesh() const;

	void SetMaterial(Material* material);
	Material* GetMaterial() const;

	void SetMaterials(const std::vector<Material*>& materials);
	const std::vector<Material*>& GetMaterials() const;

	virtual void Init();
	virtual void Update();
	virtual void Render(int viewId);

	static ZObject* CreateInstance(std::string name, ObjectType type);

private:
	Mesh* _mesh;
	std::vector<Material*> _materials;
};

