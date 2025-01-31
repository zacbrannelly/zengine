#include "SceneGraphWindow.h"
#include "../Editor.h"
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Rendering/MeshFactory.h>
#include <ZEngine-Core/Component/MeshRenderer.h>
#include <ZEngine-Core/Component/Transform.h>
#include <ZEngine-Core/Map/Objects/Entity.h>
#include <ZEngine-Core/ImmediateUI/imgui-includes.h>
#include <iostream>

using namespace ZEngine;

SceneGraphWindow::SceneGraphWindow(Editor* editor) : GUIWindow("Hierarchy Window", 800, 400, false)
{
	_context = editor;

	SetFlags(ImGuiWindowFlags_MenuBar);
}

void SceneGraphWindow::ProcessInput()
{

}

void SceneGraphWindow::RenderNode(Entity* node)
{
	bool hasChildren = node->GetTransform()->GetChildren().size() > 0;

	// Only open a node if it is double clicked or the arrow is clicked
	ImGuiTreeNodeFlags flags = (hasChildren ? 0 : ImGuiTreeNodeFlags_Leaf) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_FramePadding;

	// If the node is the selected entity, render it as selected
	if (_context->GetSelectedEntity() == node)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
	}

	// Render the node
	bool nodeOpen = ImGui::TreeNodeEx(node->GetName().c_str(), flags);

	// If the node is clicked, select the entity in the editor
	if (ImGui::IsItemClicked())
	{
		_context->SetSelectedEntity(node);
	}

	if (nodeOpen)
	{
		if (hasChildren)
		{
			// Render the children of this node (recursively)
			for (auto child : node->GetTransform()->GetChildren())
			{
				RenderNode(child->GetOwner());
			}
		}

		ImGui::TreePop();
	}
}

void SceneGraphWindow::RenderInWindow()
{
	if (_context->GetSelectedMap() != nullptr)
	{
		auto map = _context->GetSelectedMap();

		ImGui::BeginMenuBar();

		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::BeginMenu("3D Objects"))
			{
				if (ImGui::MenuItem("Cube"))
				{

				}

				if (ImGui::MenuItem("Sphere"))
				{
					// TODO: Create sphere object
				}

				if (ImGui::MenuItem("Plane"))
				{
					// TODO: Create pop-up dialog for setting width and height
				}

				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("2D Objects"))
			{
				if (ImGui::MenuItem("Rectangle / Quad"))
				{
					// TODO: Create rectangle entity
				}

				if (ImGui::MenuItem("Sprite"))
				{
					// TODO: Create general sprite
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Lighting"))
			{
				// TODO: Come up with lighting options

				if (ImGui::MenuItem("Directional Light"))
				{

				}

				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Empty Entity"))
			{
				auto newEntity = Factory::CreateInstance<Entity>("New Entity", ENTITY);
				map->Add(newEntity);
			}

			if (ImGui::MenuItem("Camera"))
			{
				// TODO: Create camera object
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 5));
		if (ImGui::CollapsingHeader(map->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto entity : map->GetEntities())
			{
				// Render the root entities (the children will be handled inside the function)
				if (entity->GetTransform()->GetParent() == nullptr)
				{
					RenderNode(entity);
				}
			}
		}
		ImGui::PopStyleVar();
	}
}

SceneGraphWindow::~SceneGraphWindow()
{
}
