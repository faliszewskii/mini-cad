//
// Created by faliszewskii on 03.03.24.
//

#ifndef OPENGL_SANDBOX_ENTITYLISTWORKSPACE_H
#define OPENGL_SANDBOX_ENTITYLISTWORKSPACE_H

#include "../../../logic/state/AppState.h"
#include "../../../logic/concepts/has_name.h"

namespace EntityListWorkspace {

    void renderWorkspaceTorus(Torus &torus);
    void renderWorkspaceTransform(Transformation &transform);
    void renderWorkspacePoint(Point &point);

    template<typename T> requires has_name<T>
    void renderNameInput(T &el);
    template<typename T> requires has_name<T> && has_id<T>
    void renderListing(std::map<int, std::unique_ptr<T>> &list, const std::string &entityName, AppState &appState);
    void renderWorkspaceSelected(const AppState &appState);

    void renderDeleteButton(AppState &appState);

    inline void render(AppState &appState) {
        if (ImGui::Button("Add Torus")) {
            auto torus = std::make_unique<Torus>(appState.cursorPosition);
            appState.torusSet.emplace(torus->id, std::move(torus));
        } ImGui::SameLine();
        if (ImGui::Button("Add Point")) {
            auto point = std::make_unique<Point>(appState.cursorPosition);
            appState.pointSet.emplace(point->id, std::move(point));
        }
        renderDeleteButton(appState);
        renderListing(appState.torusSet, "Toruses", appState);
        renderListing(appState.pointSet, "Points", appState);

        renderWorkspaceSelected(appState);
    }

    inline void renderDeleteButton(AppState &appState) {
        ImGui::BeginDisabled(appState.selectedEntities.set.empty());
        if (ImGui::Button("Delete")) {
            for(auto &el : appState.selectedEntities.set) {
                std::visit(overloaded{
                       [&](Torus &torus) { appState.torusSet.erase(appState.torusSet.find(torus.id)); },
                       [&](Point &point) { appState.pointSet.erase(appState.pointSet.find(point.id)); }
                    }, el.second);
            }
            appState.selectedEntities.set.clear();
        }
        ImGui::EndDisabled();
    }

    inline void renderWorkspaceSelected(const AppState &appState) {
        if(ImGui::BeginChild("##SelectedEntities")) {
            auto &selected = appState.selectedEntities.set;
            if (selected.empty())
                ImGui::SeparatorText("Select entities");
            else if (selected.size() == 1) {
                std::visit(overloaded{
                        [](Torus &torus) { renderWorkspaceTorus(torus); },
                        [](Point &point) { renderWorkspacePoint(point); }
                }, selected.begin()->second);
            } else {
                ImGui::SeparatorText(("Selected " + std::to_string(selected.size()) + " entities").c_str());
            }
            ImGui::EndChild();
        }
    }

    inline void renderWorkspaceTorus(Torus &torus) {
        ImGui::SeparatorText(torus.name.c_str());
        renderNameInput(torus);

        renderWorkspaceTransform(torus.transform);

        ImGui::SeparatorText("Parameters");
                for(auto &parameter : torus.getParameters()) {
            std::visit(overloaded{
                    [&parameter](int& value) { ImGui::DragInt(parameter.first.c_str(), &value); },
                    [&parameter](float& value) { ImGui::DragFloat(parameter.first.c_str(), &value, 0.01); }
            }, parameter.second);
            if(ImGui::IsItemActive()) torus.generate();
        }
    }

    template<typename T> requires has_name<T>
    void renderNameInput(T &el) {
        const static int bufSize = 1024;
        static char nameBuffer[bufSize] = "\0";
        el.name.copy(nameBuffer, el.name.size());
        nameBuffer[el.name.size()] = '\0';
        if (ImGui::InputText("Name", nameBuffer, bufSize))
            el.name = std::string(nameBuffer);
    }

    inline void renderWorkspacePoint(Point &point) {
        ImGui::SeparatorText(point.name.c_str());
        renderNameInput(point);

        ImGui::Text("Position:");
        ImGui::DragFloat("x##position", static_cast<float *>(glm::value_ptr(point.position)) + 0, 0.01f);
        ImGui::DragFloat("y##position", static_cast<float *>(glm::value_ptr(point.position)) + 1, 0.01f);
        ImGui::DragFloat("z##position", static_cast<float *>(glm::value_ptr(point.position)) + 2, 0.01f);
    }

    inline void renderWorkspaceTransform(Transformation &transform) {
        ImGui::SeparatorText("Transform");

        auto position = static_cast<float *>(glm::value_ptr(transform.getTranslationRef()));
        ImGui::Text("Position:");
        ImGui::DragFloat("x##position", position + 0, 0.01f);
        ImGui::DragFloat("y##position", position + 1, 0.01f);
        ImGui::DragFloat("z##position", position + 2, 0.01f);

        auto oldAngle = transform.getRotationAngles();
        auto newAngle = glm::vec3(oldAngle);
        auto angleRef = static_cast<float *>(glm::value_ptr(newAngle));
        ImGui::Text("Rotation:");
        ImGui::DragFloat("x##orientation", angleRef + 0, 0.01f);
        if(ImGui::IsItemActive()) transform.addRotation(glm::vec3(newAngle.x - oldAngle.x, 0, 0));
        ImGui::DragFloat("y##orientation", angleRef + 1, 0.01f);
        if(ImGui::IsItemActive()) transform.addRotation(glm::vec3(0, newAngle.y - oldAngle.y, 0));
        ImGui::DragFloat("z##orientation", angleRef + 2, 0.01f);
        if(ImGui::IsItemActive()) transform.addRotation(glm::vec3(0, 0, newAngle.z - oldAngle.z));

        auto scale = static_cast<float *>(glm::value_ptr(transform.getScaleRef()));
        ImGui::Text("Scale:");
        ImGui::DragFloat("x##scale", scale + 0, 0.001f);
        ImGui::DragFloat("y##scale", scale + 1, 0.001f);
        ImGui::DragFloat("z##scale", scale + 2, 0.001f);
    }

    template<typename T> requires has_name<T> && has_id<T>
    inline void renderListing(std::map<int, std::unique_ptr<T>> &list, const std::string &entityName, AppState &appState) {
        if(ImGui::CollapsingHeader(entityName.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginListBox((entityName + std::string("#Workspace")).c_str(), ImVec2(-FLT_MIN, 0))) {
                for (auto &el: std::views::values(list)) {
                    if (ImGui::Selectable((el->name + "##" + std::to_string(el->id)).c_str(), appState.selectedEntities.set.contains(el->id))) {
                        appState.selectedEntities.add(appState, *el);
                    }
                }
                ImGui::EndListBox();
            }
        }
    }
}
#endif //OPENGL_SANDBOX_ENTITYLISTWORKSPACE_H
