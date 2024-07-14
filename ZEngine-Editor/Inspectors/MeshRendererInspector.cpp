#include "MeshRendererInspector.h"
#include <ZEngine-Core/Component/MeshRenderer.h>
#include <ZEngine-Core/Rendering/Mesh.h>
#include <ZEngine-Core/Rendering/MeshFactory.h>
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Assets/Objects/ModelAsset.h>
#include <ZEngine-Core/Assets/Objects/MaterialAsset.h>
#include <ZEngine-Core/Rendering/Material.h>
#include <ZEngine-Core/ImmediateUI/imgui-includes.h>

#include <iostream>
#include <sstream>

using namespace std;

MeshRendererInspector::MeshRendererInspector() 
{
}

void MeshRendererInspector::Inspect(Component* comp)
{
	if (comp != GetValue())
	{
		Inspector::Inspect(comp);
		_neededMaterials = static_cast<MeshRenderer*>(comp)->GetMaterials().size();
	}
}

void MeshRendererInspector::RenderElement()
{
	if (GetValue() == nullptr) return;

	auto comp = static_cast<MeshRenderer*>(GetValue());
	auto assetManager = AssetManager::GetInstance();
	auto catalog = assetManager->GetCatalog();

	auto inspectorHeight = 40 * (_neededMaterials + 2);
	ImGui::BeginChild(UniqueLabel("Mesh Renderer").c_str(), ImVec2(0, inspectorHeight));

	auto meshName = comp->GetMesh() != nullptr ? comp->GetMesh()->GetName() : "None";
	if (ImGui::BeginCombo("Mesh", meshName.c_str()))
	{
		if (ImGui::Selectable("Cube"))
		{
			auto newMesh = MeshFactory::CreateCube(comp->GetOwner()->GetName() + "_cube");
			comp->SetMesh(newMesh);
		}

		if (ImGui::Selectable("Sphere"))
		{
			auto newMesh = MeshFactory::CreateSphereStrip(comp->GetOwner()->GetName() + "_sphere");
			comp->SetMesh(newMesh);
		}

		if (ImGui::Selectable("Rectangle"))
		{
			auto newMesh = MeshFactory::CreateRectangle(comp->GetOwner()->GetName() + "_rectangle");
			comp->SetMesh(newMesh);
		}

		if (ImGui::Selectable("Plane"))
		{
			auto newMesh = MeshFactory::CreatePlane(comp->GetOwner()->GetName() + "_plane", 10, 10);
			comp->SetMesh(newMesh);
		}

		if (catalog != nullptr)
		{
			auto available = catalog->GetAssetsByType(MODEL_ASSET);
			for (auto& entry : available)
			{
				if (ImGui::Selectable(entry.name.c_str()))
				{
					auto asset = assetManager->FindAssetFromPath(entry.path);

					if (asset == nullptr)
						asset = assetManager->LoadAsset(entry.path, entry.path, MODEL_ASSET);

					if (asset != nullptr)
						comp->SetMesh(asset->Cast<ModelAsset>()->GetMesh());
				}
			}
		}

		ImGui::EndCombo();
	}

	ImGui::DragInt("Materials", &_neededMaterials);

	for (int i = 0; i < _neededMaterials; i++)
	{
		auto current = i >= comp->GetMaterials().size() ? nullptr : comp->GetMaterials()[i];

		stringstream materialName;
		materialName << "Material " << i;

		if (ImGui::BeginCombo(materialName.str().c_str(), current != nullptr ? current->GetName().c_str() : "None"))
		{
			if (catalog != nullptr)
			{
				auto available = catalog->GetAssetsByType(MATERIAL_ASSET);
				for (auto& entry : available)
				{
					if (ImGui::Selectable(entry.name.c_str()))
					{
						auto asset = assetManager->FindAssetFromPath(entry.path);

						if (asset == nullptr)
							asset = assetManager->LoadAsset(entry.path, entry.path, MATERIAL_ASSET);

						if (asset != nullptr)
						{
							auto material = static_cast<MaterialAsset*>(asset);
							comp->SetMaterial(material->GetMaterial(), i);
						}
					}
				}
			}

			ImGui::EndCombo();
		}
	}

	ImGui::EndChild();

}

MeshRendererInspector::~MeshRendererInspector()
{
}
