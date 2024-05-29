//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_MENUBARMODULE_H
#define OPENGL_SANDBOX_MENUBARMODULE_H

#include "../../../logic/state/AppState.h"
#include "../../../logic/serializer/Serializer.h"
#include "imgui.h"
#include "nfd.h"

class MenuBarModule {
public:
    explicit MenuBarModule() {}

    void saveNfd(const std::function<void(const std::string &)> &func) {
        NFD_Init();

        nfdchar_t *outPath;
        nfdfilteritem_t filterItem[1] = { { "Scene file json", "json" } };
        nfdresult_t result = NFD_SaveDialogN(&outPath, filterItem, 1, NULL, "scene.json");
        if (result == NFD_OKAY)
        {
            std::string s(outPath);
            func(s);
            NFD_FreePath(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            puts("User pressed cancel.");
        }
        else
        {
            printf("Error: %s\n", NFD_GetError());
        }

        NFD_Quit();
    }

    void openNfd(const std::function<void(const std::string &)> &func) {
        NFD_Init();

        nfdchar_t *outPath;
        nfdfilteritem_t filterItem[1] = { { "Scene file json", "json" } };
        nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
        if (result == NFD_OKAY)
        {
            std::string s(outPath);
            func(s);
            NFD_FreePath(outPath);
        }
        else if (result == NFD_CANCEL)
        {
            puts("User pressed cancel.");
        }
        else
        {
            printf("Error: %s\n", NFD_GetError());
        }

        NFD_Quit();
    }

    void run(AppState &appState) {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "", false, true)) {
                    appState.torusSet.clear();
                    appState.pointSet.clear();
                    appState.bezierC0Set.clear();
                    appState.bezierC2Set.clear();
                    appState.interpolatedC2Set.clear();
                    appState.patchC0Set.clear();
                    appState.patchC2Set.clear();
                    appState.selectedEntities.clear();
                    appState.selectionContext.clear();
                    appState.gregoryPatchCreator.reset();
                }
                if (ImGui::MenuItem("Import", "", false, true)) {
                    openNfd([&](const std::string &path) {
                        appState.serializer->importScene(appState, path);
                    });
                }
                if (ImGui::MenuItem("Export", "", false, true)) {
                    saveNfd([&](const std::string &path) {
                        appState.serializer->exportScene(appState, path);
                    });
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if(ImGui::MenuItem("Generate Gregory Patches", "", appState.gregoryCreatorOpen, true)) {
                    appState.gregoryCreatorOpen = true;
                    appState.gregoryPatchCreator.reset();
                }
                ImGui::Separator();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

};

#endif //OPENGL_SANDBOX_MENUBARMODULE_H
