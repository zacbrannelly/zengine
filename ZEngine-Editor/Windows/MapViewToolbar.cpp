#include "MapViewToolbar.h"

MapViewToolbar::MapViewToolbar(Editor* editor, MapView* mapView) : _editor(editor), _mapView(mapView), _objectGizmoOp(ImGuizmo::TRANSLATE), _objectGizmoMode(ImGuizmo::LOCAL)
{
}

ImGuizmo::OPERATION MapViewToolbar::GetObjectGizmoOp() const
{
    return _objectGizmoOp;
}

ImGuizmo::MODE MapViewToolbar::GetObjectGizmoMode() const
{
    return _objectGizmoMode;
}

void MapViewToolbar::RenderElement()
{
    bool wasTranslateSelected = _objectGizmoOp & ImGuizmo::TRANSLATE;
    bool wasRotateSelected = _objectGizmoOp & ImGuizmo::ROTATE;
    bool wasScaleSelected = _objectGizmoOp & ImGuizmo::SCALE;
    
    bool translate = wasTranslateSelected;
    bool rotate = wasRotateSelected;
    bool scale = wasScaleSelected;
    
    auto translateButtonSize = ImGui::CalcTextSize("Translate");
    ImGui::Selectable("Translate", &translate, 0, translateButtonSize);
    
    ImGui::SameLine();
    
    auto rotateButtonSize = ImGui::CalcTextSize("Rotate");
    ImGui::Selectable("Rotate", &rotate, 0, rotateButtonSize);
    
    ImGui::SameLine();
    
    auto scaleButtonSize = ImGui::CalcTextSize("Scale");
    ImGui::Selectable("Scale", &scale, 0, scaleButtonSize);
    
    ImGui::SameLine();
    ImGui::SetCursorPosX(translateButtonSize.x + rotateButtonSize.x + scaleButtonSize.x + 50);
    
    bool local = _objectGizmoMode == ImGuizmo::LOCAL;
    bool world = _objectGizmoMode == ImGuizmo::WORLD;
    
    auto localButtonSize = ImGui::CalcTextSize("Local");
    if (ImGui::Selectable("Local", &local, 0, localButtonSize))
    {
        _objectGizmoMode = ImGuizmo::LOCAL;
    }
    
    ImGui::SameLine();
    
    auto worldButtonSize = ImGui::CalcTextSize("World");
    if (ImGui::Selectable("World", &world, 0, worldButtonSize))
    {
        _objectGizmoMode = ImGuizmo::WORLD;
    }
    
    if (translate && !wasTranslateSelected)
    {
        _objectGizmoOp = _objectGizmoOp | ImGuizmo::TRANSLATE;
    }
    
    if (!translate && wasTranslateSelected)
    {
        _objectGizmoOp = (ImGuizmo::OPERATION)(_objectGizmoOp & ~ImGuizmo::TRANSLATE);
    }
    
    if (rotate && !wasRotateSelected)
    {
        _objectGizmoOp = _objectGizmoOp | ImGuizmo::ROTATE;
    }
    
    if (!rotate && wasRotateSelected)
    {
        _objectGizmoOp = (ImGuizmo::OPERATION)(_objectGizmoOp & ~ImGuizmo::ROTATE);
    }
    
    if (scale && !wasScaleSelected)
    {
        _objectGizmoOp = _objectGizmoOp | ImGuizmo::SCALE;
    }
    
    if (!scale && wasScaleSelected)
    {
        _objectGizmoOp = (ImGuizmo::OPERATION)(_objectGizmoOp & ~ImGuizmo::SCALE);
    }
}

MapViewToolbar::~MapViewToolbar()
{
}
