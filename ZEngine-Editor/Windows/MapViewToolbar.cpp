#include "MapViewToolbar.h"
#include "MapView.h"
#include <ZEngine-Core/Component/Camera.h>

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
    auto localWorldButtonPos = translateButtonSize.x + rotateButtonSize.x + scaleButtonSize.x + 50;
    ImGui::SetCursorPosX(localWorldButtonPos);
    
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

    ImGui::SameLine();

    auto perspectiveButtonSize = ImGui::CalcTextSize("Perspective");
    auto orthographicButtonSize = ImGui::CalcTextSize("Orthographic");
    ImGui::SetCursorPosX(_mapView->GetContentWidth() - perspectiveButtonSize.x - orthographicButtonSize.x - 4);

    bool perspective = _mapView->GetCamera()->GetProjectionMode() == Camera::PERSPECTIVE;
    if (ImGui::Selectable("Perspective", &perspective, 0, perspectiveButtonSize))
    {
        _mapView->GetCamera()->SetProjectionMode(Camera::PERSPECTIVE);
    }

    ImGui::SameLine();

    bool orthographic = _mapView->GetCamera()->GetProjectionMode() == Camera::ORTHOGRAPHIC;
    if (ImGui::Selectable("Orthographic", &orthographic, 0, orthographicButtonSize))
    {
        _mapView->GetCamera()->SetProjectionMode(Camera::ORTHOGRAPHIC);
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
