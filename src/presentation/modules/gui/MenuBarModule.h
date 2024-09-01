//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_MENUBARMODULE_H
#define OPENGL_SANDBOX_MENUBARMODULE_H

#include "../../../logic/state/AppState.h"
#include "../../../logic/serializer/Serializer.h"
#include "imgui.h"
#include "nfd.h"
#include "../../../logic/intersections/SurfaceIntersection.h"
#include "../../../logic/intersections/IntersectableSurface.h"

class MenuBarModule {
public:
    explicit MenuBarModule() {}

    void saveNfd(const std::function<void(const std::string &)> &func) {
        NFD_Init();

        nfdu8char_t *outPath;
        nfdu8filteritem_t filters[1] = { { "Scene file json", "json" } };
        nfdsavedialogu8args_t args = {0};
        args.filterList = filters;
        args.filterCount = 1;
        args.defaultName = "scene.json";
        nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &args);
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

        nfdu8char_t *outPath;
        nfdu8filteritem_t filters[1] = { { "Scene file json", "json" }};
        nfdopendialogu8args_t args = {0};
        args.filterList = filters;
        args.filterCount = 1;
        nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
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

    static bool isIntersectionEnabled(const std::vector<std::pair<int, EntityType>>& selected) {
        if( selected.empty() || selected.size() > 2)
            return false;
        if(!std::visit([](auto& a) {
            return ParametricSurface<decltype(a.get())> && Intersectable<decltype(a.get())>;
        }, selected[0].second))
            return false;
        if(selected.size() == 2 && !std::visit([](auto& b) {
            return ParametricSurface<decltype(b.get())> && Intersectable<decltype(b.get())>;
        }, selected[1].second))
            return false;
        return true;
    }

    static std::optional<IntersectableSurface> getIfIntersectableSurface(EntityType entity) {
        return std::visit([](auto& a) -> std::optional<IntersectableSurface> {
            using T = std::decay_t<decltype(a.get())>;
            if constexpr (ParametricSurface<T> && Intersectable<T>)
                return a;
            return {};
        }, entity);
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
                    appState.gregoryPatchSet.clear();
                    appState.intersectionSet.clear();
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
                bool intersectionEnabled = isIntersectionEnabled(appState.selectedEntities);
                if(ImGui::MenuItem("Find Any Intersection", "", false, intersectionEnabled)) {
                    auto& [idA, entityA] = appState.selectedEntities[0];
                    auto& [idB, entityB] = appState.selectedEntities[1 % appState.selectedEntities.size()];
                    auto sA = getIfIntersectableSurface(entityA).value();
                    auto sB = getIfIntersectableSurface(entityB).value();
                    std::visit([&](auto& a, auto& b) {
                        auto result = appState.surfaceIntersection.findIntersection(a.get(), b.get(), idA == idB, appState.cursorPosition);
                        if(!result.has_value()) {
                            appState.logger.logError(result.error());
                            return;
                        }
                        auto points = result->intersectionPoints;
                        appState.eventPublisher.publish(CreateIntersectionEvent(
                            result->intersectionPoints,
                            result->surfaces,
                            result->wrapped
                            ));
                        // TODO DEBUG
                        appState.surfaceIntersection.addToMask(sA, *appState.intersectionSet[appState.lastIdCreated], 0);
                        appState.surfaceIntersection.addToMask(sB, *appState.intersectionSet[appState.lastIdCreated], 1);
                    }, sA, sB);
                }
                if(ImGui::MenuItem("Find Intersection Near Cursor", "", false, intersectionEnabled)) {
                   // TODO
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

};

#endif //OPENGL_SANDBOX_MENUBARMODULE_H
