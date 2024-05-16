//
// Created by faliszewskii on 03.05.24.
//

#include "BezierPatchCreatorUI.h"

void BezierPatchCreatorUI::render() {
    ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_AlwaysAutoResize;

    auto params = appState.bezierPatchCreator.getParams();
    bool modified = false;

    if (ImGui::Begin("Bezier Patch Creator", &appState.bezierCreatorOpen, window_flags)) {
        modified |= ImGui::InputInt("Patch Count Width", &params.patchCountWidth);
        modified |= ImGui::InputInt("Patch Count Length", &params.patchCountLength);

        modified |= ImGui::Checkbox("Wrapped", &params.wrapped);
        if(params.wrapped) {
            modified |= ImGui::DragFloat("Cylinder Height", &params.cylinderHeight, 0.01);
            modified |= ImGui::DragFloat("Cylinder Radius", &params.cylinderRadius, 0.01);
        } else {
            modified |= ImGui::DragFloat("Plane Width", &params.planeWidth, 0.01);
            modified |= ImGui::DragFloat("Plane Length", &params.planeLength, 0.01);
        }

        modified |= ImGui::Checkbox("C2", &params.C2);

        modified |= ImGui::Checkbox("dance", &params.dance);

        if(ImGui::Button("Create")) {
            std::vector<int> points;
            for(auto &point : appState.bezierPatchCreator.getPointVertices()) {
                appState.eventPublisher.publish(CreatePointEvent{point.position});
                points.push_back(appState.lastIdCreated);
            }
            appState.eventPublisher.publish(CreateBezierPatch{
                appState.bezierPatchCreator.getPatchIndices(),
                appState.bezierPatchCreator.getGridIndices(),
                points,
                appState.bezierPatchCreator.getParams().C2
            });
            appState.bezierCreatorOpen = false;
        }
    }
    ImGui::End();

    if(modified) {
        if(params.C2 && params.patchCountWidth < 3) params.patchCountWidth = 3;
        if(params.cylinderHeight <= 0) params.cylinderHeight = 0.01f;
        if(params.cylinderRadius <= 0) params.cylinderRadius = 0.01f;
        if(params.planeLength <= 0) params.planeLength = 0.01f;
        if(params.planeWidth <= 0) params.planeWidth = 0.01f;
        if(params.patchCountWidth <= 0) params.patchCountWidth = 1;
        if(params.patchCountLength <= 0) params.patchCountLength = 1;
        appState.bezierPatchCreator.update(params);
    }
}
