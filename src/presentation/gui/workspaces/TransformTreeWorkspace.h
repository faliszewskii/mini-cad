//
// Created by faliszewskii on 17.02.24.
//

#ifndef OPENGL_SANDBOX_TRANSFORMTREEWORKSPACE_H
#define OPENGL_SANDBOX_TRANSFORMTREEWORKSPACE_H

#include "nfd.h"
#include <glm/gtc/type_ptr.hpp>
#include "../../../logic/state/AppState.h"
#include "imgui.h"
#include "../../../logic/generator/CircleGenerator.h"
#include "../../../logic/generator/SphereGenerator.h"
#include "../../../logic/generator/TorusGenerator.h"
#include "../../../logic/generator/CylinderGenerator.h"

namespace TransformTreeWorkspace {

    void loadModelModal(AppState &appState);
    void addTransformNode(AppState &appState);
    void generateCylinderModel(AppState &appState, float R = 1, float H = 1);
    void generateSphereModel(AppState &appState, float R = 1);
    void generateTorusModel(AppState &appState, float radius = 1, float thickness = 0.25);
    void addPoint(AppState &appState);


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
        // TODO combobox to choose material from available

        // TODO Mesh UI
    }

    inline void renderWorkspaceGenerator(MeshGenerator &generator) {
        renderWorkspaceMesh(generator.getTargetMesh());
        ImGui::SeparatorText(generator.getName().c_str());

        for(auto &parameter : generator.getParameters()) {
            std::visit(overloaded{
                    [&parameter](int& value) { ImGui::DragInt(parameter.first.c_str(), &value); },
                    [&parameter](float& value) { ImGui::DragFloat(parameter.first.c_str(), &value, 0.01); }
            }, parameter.second);
            if(ImGui::IsItemActive()) generator.generate();
        }

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
                        [&](std::unique_ptr<Mesh<Vertex>>& mesh) {
                            if (appState.selectionGroup.getSelectedMesh() && &appState.selectionGroup.getSelectedMesh().value().get() == &*mesh) localFlagsMesh |= ImGuiTreeNodeFlags_Selected;
                            ImGui::TreeNodeEx(uuids::to_string(mesh->getUuid()).c_str(), localFlagsMesh | ImGuiTreeNodeFlags_Leaf,
                                              "[M] %s", mesh->getName().c_str());
                            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                                appState.selectionGroup.setFocus(*mesh);
                            ImGui::TreePop();
                        },
                        [&](std::unique_ptr<MeshGenerator>& generator) {
                            if (appState.selectionGroup.getSelectedMeshGenerator() && &appState.selectionGroup.getSelectedMeshGenerator().value().get() == &*generator) localFlagsMesh |= ImGuiTreeNodeFlags_Selected;
                            ImGui::TreeNodeEx(uuids::to_string(generator->getTargetMesh().getUuid()).c_str(), localFlagsMesh | ImGuiTreeNodeFlags_Leaf,
                                              "[G] %s", generator->getTargetMesh().getName().c_str());
                            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                                appState.selectionGroup.setFocus(*generator);
                            ImGui::TreePop();
                        },
                        [&](std::unique_ptr<Point>& point) {
                            if (appState.selectionGroup.getSelectedPoint() && &appState.selectionGroup.getSelectedPoint().value().get() == &*point) localFlagsMesh |= ImGuiTreeNodeFlags_Selected;
                            ImGui::TreeNodeEx(uuids::to_string(point->mesh->getUuid()).c_str(), localFlagsMesh | ImGuiTreeNodeFlags_Leaf,
                                              "[P] %s", point->mesh->getName().c_str());
                            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                                appState.selectionGroup.setFocus(*point);
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
        if(ImGui::Button("Import Model")) loadModelModal(appState);
        if(ImGui::Button("Add Transform Node")) addTransformNode(appState);
        if(ImGui::Button("Generate Cylinder")) generateCylinderModel(appState);
        if(ImGui::Button("Generate Sphere")) generateSphereModel(appState);
        if(ImGui::Button("Generate Torus")) generateTorusModel(appState);
        if(ImGui::Button("Add Point")) addPoint(appState);
        ImGui::EndDisabled();

        auto &selectedTransform = appState.selectionGroup.getSelectedTransformTree();
        if(selectedTransform) renderWorkspaceTransform(selectedTransform->get().transform);
        auto &selectedMesh = appState.selectionGroup.getSelectedMesh();
        if(selectedMesh) renderWorkspaceMesh(selectedMesh->get());
        auto &selectedGenerator = appState.selectionGroup.getSelectedMeshGenerator();
        if(selectedGenerator) renderWorkspaceGenerator(selectedGenerator->get());
        // TODO Point Workspace
    }

    inline void loadModelModal(AppState &appState) {
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


    inline void generateCylinderModel(AppState &appState, float R, float H) {
        // TODO naming of the entities maybe shouldn't be a base class. Maybe trait or concept. Or just a field.
        TransformTree &parent = appState.selectionGroup.getSelectedTransformTree().value();
        auto &transform = parent.addChild(std::make_unique<TransformTree>("Cylinder"));
        transform.addChild(std::unique_ptr<MeshGenerator>(std::make_unique<CircleGenerator>(glm::vec3{0,-H/2,0}, R)));
        transform.addChild(std::unique_ptr<MeshGenerator>(std::make_unique<CylinderGenerator>(glm::vec3{0,0,0}, R, H)));
        transform.addChild(std::unique_ptr<MeshGenerator>(std::make_unique<CircleGenerator>(glm::vec3{0,H/2,0}, R)));
        appState.selectionGroup.setFocus(transform);
    }

    inline void generateSphereModel(AppState &appState, float R) {
        TransformTree &parent = appState.selectionGroup.getSelectedTransformTree().value();
        auto &transform = parent.addChild(std::make_unique<TransformTree>("Sphere"));
        transform.addChild(std::unique_ptr<MeshGenerator>(std::make_unique<SphereGenerator>(glm::vec3{0,0,0}, R)));
        appState.selectionGroup.setFocus(transform);
    }

    inline void generateTorusModel(AppState &appState, float radius, float thickness) {
        TransformTree &parent = appState.selectionGroup.getSelectedTransformTree().value();
        auto &transform = parent.addChild(std::make_unique<TransformTree>("Torus"));
        transform.addChild(std::unique_ptr<MeshGenerator>(std::make_unique<TorusGenerator>(glm::vec3{0,0,0}, radius, thickness)));
        appState.selectionGroup.setFocus(transform);
    }

    inline void addTransformNode(AppState &appState) {
        TransformTree &parent = appState.selectionGroup.getSelectedTransformTree().value();
        auto &transform = parent.addChild(std::make_unique<TransformTree>("Transform"));
        appState.selectionGroup.setFocus(transform);
    }

    inline void addPoint(AppState &appState) {
        TransformTree &parent = appState.selectionGroup.getSelectedTransformTree().value();
        auto &point = parent.addChild(std::make_unique<Point>());
        appState.selectionGroup.setFocus(*point);

    }
};

#endif //OPENGL_SANDBOX_TRANSFORMTREEWORKSPACE_H
