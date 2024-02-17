//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_MENUBARMODULE_H
#define OPENGL_SANDBOX_MENUBARMODULE_H

#include "../Module.h"
#include "../../../logic/state/AppState.h"
#include "imgui.h"
#include "nfd.h"

class MenuBarModule : public Module {
public:
    explicit MenuBarModule() : Module(true) {}

    void run(AppState &appState) final {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "CTRL+Z", false, false)) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X", false, false)) {}
                if (ImGui::MenuItem("Copy", "CTRL+C", false, false)) {}
                if (ImGui::MenuItem("Paste", "CTRL+V", false, false)) {}
                ImGui::Separator();
//                renderMenuItemLoadModel();
//                if (ImGui::MenuItem("Add Axis") && guiState.selectedStep) { // TODO Grey out on no select with tooltip.
//                    guiState.scene.merge(ModelGenerator::generateAxis());
//                }
//                if (ImGui::MenuItem("Add Sphere") && guiState.selectedStep) {
//                    guiState.scene.merge(ModelGenerator::generateSphere(50, 50));
//                }
//                if (ImGui::MenuItem("Add Torus") && guiState.selectedStep) {
//                    guiState.scene.merge(ModelGenerator::generateTorus(50, 50));
//                }
//                if (ImGui::MenuItem("Add Hyperbolic Paraboloid") && guiState.selectedStep) {
//                    float a = 1, b = 1, c = -0.5;
//                    auto generator = ModelGenerator::HyperbolicParaboloidGenerator(a, b, c);
//                    auto mesh = generator.generateParametrisedMesh("Hyperbolic Paraboloid Mesh", 10, 10);
//                    auto model = ModelGenerator::generateSolid(std::move(mesh), "Hyperbolic Paraboloid model");
//                    guiState.scene.merge(std::move(model));
//                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

//    static void renderMenuItemLoadModel() {
//        if (ImGui::MenuItem("Load Model") && guiState.selectedStep && !guiState.selectedStep->get().isLeaf()) { // TODO Grey out on no select with tooltip.
//            NFD_Init();
//
//            nfdchar_t *outPath;
//            nfdfilteritem_t filterItem[1]{{"3D models", "gltf,fbx,FBX,obj"}};
//            nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
//            if (result == NFD_OKAY) {
//                try {
//                    auto model = guiState.assetImporter.importModel(outPath);
//                    guiState.scene.merge(std::move(model), guiState.selectedStep->get());
//                } catch (FailedToLoadModelException &ex) {
//                    // TODO Log error to log window.
//                    std::cerr << ex.what() << std::endl;
//                }
//                NFD_FreePath(outPath);
//            } else if (result == NFD_CANCEL) {
//            } else {
//                printf("Error: %s\n", NFD_GetError());
//            }
//
//            NFD_Quit();
//        }
//    }

    [[nodiscard]] std::string getName() const final {
        return "Menu Bar Module";
    }
};

#endif //OPENGL_SANDBOX_MENUBARMODULE_H
