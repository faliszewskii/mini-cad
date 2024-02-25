//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_TRANSFORMTREEWORKSPACE_H
#define OPENGL_SANDBOX_TRANSFORMTREEWORKSPACE_H

#include "nfd.h"
#include <glm/gtc/type_ptr.hpp>
#include "../../../logic/state/AppState.h"
#include "imgui.h"

namespace TransformTreeWorkspace {
    inline void renderWorkspaceTransform(Transformation &transform) {
        ImGui::SeparatorText(transform.getName().c_str());
        ImGui::Text("Position:");
        ImGui::DragFloat("x##position", static_cast<float *>(glm::value_ptr(transform.getTranslationRef())) + 0, 0.01f);
        ImGui::DragFloat("y##position", static_cast<float *>(glm::value_ptr(transform.getTranslationRef())) + 1, 0.01f);
        ImGui::DragFloat("z##position", static_cast<float *>(glm::value_ptr(transform.getTranslationRef())) + 2, 0.01f);
        ImGui::Text("Orientation:");
        ImGui::DragFloat("x##orientation", static_cast<float *>(glm::value_ptr(transform.getOrientationRef())) + 0, 0.01f);
        ImGui::DragFloat("y##orientation", static_cast<float *>(glm::value_ptr(transform.getOrientationRef())) + 1, 0.01f);
        ImGui::DragFloat("z##orientation", static_cast<float *>(glm::value_ptr(transform.getOrientationRef())) + 2, 0.01f);
        ImGui::DragFloat("w", static_cast<float *>(glm::value_ptr(transform.getOrientationRef())) + 3, 0.01f);
        ImGui::Text("Scale:");
        ImGui::DragFloat("x##scale", static_cast<float *>(glm::value_ptr(transform.getScaleRef())) + 0, 0.001f);
        ImGui::DragFloat("y##scale", static_cast<float *>(glm::value_ptr(transform.getScaleRef())) + 1, 0.001f);
        ImGui::DragFloat("z##scale", static_cast<float *>(glm::value_ptr(transform.getScaleRef())) + 2, 0.001f);
    }

    inline void renderWorkspaceMesh(Mesh<Vertex> &mesh) {
        ImGui::SeparatorText(mesh.getName().c_str());
        ImGui::Text("Material: %s", mesh.material? mesh.material->get().getName().c_str() : "No material");

        // TODO Mesh UI
    }

    inline void render(AppState &appState) {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::BeginChild("Scene Tree#Workspace", ImVec2(-FLT_MIN, 0), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                   ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth;

        std::function<void(TransformTree &)> lambda;
        lambda = [&](TransformTree &node){
            auto localFlags = flags;
            if (appState.selectionGroup.getSelectedTransformTree() && &appState.selectionGroup.getSelectedTransformTree().value().get() == &node) localFlags |= ImGuiTreeNodeFlags_Selected;
            bool nodeOpen = ImGui::TreeNodeEx(uuids::to_string(node.transform.getUuid()).c_str(), localFlags, "[T] %s", node.transform.getName().c_str());
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                appState.selectionGroup.setFocus(node);

            if(nodeOpen) {
                for(auto &child : node.getChildren())
                    lambda(*child);
                for (auto &entity: node.getEntities()) {
                    auto localFlagsMesh = flags;
                    std::visit(overloaded{
                        [&](auto &el) {
                            if (appState.selectionGroup.getSelectedMesh() && &appState.selectionGroup.getSelectedMesh().value().get() == &*el) localFlagsMesh |= ImGuiTreeNodeFlags_Selected;
                            ImGui::TreeNodeEx(uuids::to_string(el->getUuid()).c_str(), localFlagsMesh | ImGuiTreeNodeFlags_Leaf,
                                              "[M] %s", el->getName().c_str());
                            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                                appState.selectionGroup.setFocus(*el);
                            ImGui::TreePop();
                        }
                    }, entity);
                }
                ImGui::TreePop();
            }
        };

        lambda(appState.transformTree); // TODO Maybe use here enumerator design pattern

        ImGui::EndChild();
        ImGui::PopStyleVar();

        ImGui::BeginDisabled(!appState.selectionGroup.getSelectedTransformTree().has_value());
        if(ImGui::Button("Import Model")) {
            NFD_Init();

            nfdchar_t *outPath;
            nfdfilteritem_t filterItem[1]{{"3D models", "gltf,fbx,FBX,obj"}};
            nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
            if (result == NFD_OKAY) {
                try {
                    appState.assetImporter.importModel(outPath).addToState(appState, appState.selectionGroup.getSelectedTransformTree().value());
                } catch (FailedToLoadModelException &ex) {
                    // TODO Log error to log window.
                    std::cerr << ex.what() << std::endl;
                }
                NFD_FreePath(outPath);
            } else if (result == NFD_CANCEL) {
            } else {
                printf("Error: %s\n", NFD_GetError());
            }

            NFD_Quit();
        }
        ImGui::EndDisabled();

        auto &selectedTransform = appState.selectionGroup.getSelectedTransformTree();
        if(selectedTransform) renderWorkspaceTransform(selectedTransform->get().transform);
        auto &selectedMesh = appState.selectionGroup.getSelectedMesh();
        if(selectedMesh) renderWorkspaceMesh(selectedMesh->get());
    }
};

#endif //OPENGL_SANDBOX_TRANSFORMTREEWORKSPACE_H
