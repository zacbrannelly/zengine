#include "SceneGraphWindow.h"
#include "Editor.h"
#include <ZEngine-Core/Component/Transform.h>
#include "imgui-includes.h"
#include <iostream>


SceneGraphWindow::SceneGraphWindow(Editor* editor) : GUIWindow("Hierarchy Window", 800, 400, false)
{
	_context = editor;
}

void SceneGraphWindow::ProcessInput()
{

}

void SceneGraphWindow::RenderNode(Entity* node)
{
	bool hasChildren = node->GetTransform()->GetChildren().size() > 0;

	// Only open a node if it is double clicked or the arrow is clicked
	ImGuiTreeNodeFlags flags = (hasChildren ? 0 : ImGuiTreeNodeFlags_Leaf) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

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
	}
}

SceneGraphWindow::~SceneGraphWindow()
{
}
