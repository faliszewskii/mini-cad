//
// Created by faliszewskii on 29.05.24.
//

#include "GregoryPatchCreatorUI.h"
#include "../../../logic/geometry/GregoryPatch.h"

void GregoryPatchCreatorUI::render() {
    ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_AlwaysAutoResize;

    bool modified = false;
    static int holeSize = 3;

    if (ImGui::Begin("Gregory Patch Creator", &appState.gregoryCreatorOpen, window_flags)) {
        // Find Holes
        if(ImGui::Button("Find Holes")) {
            selectedHole = -1;
            auto &selected = appState.selectedEntities;
            std::vector<std::pair<int, EntityType>> patches;
            std::vector<int> patchIds;

            std::copy_if(selected.begin(), selected.end(), std::back_inserter(patches), [](auto &sel) {
                return std::holds_alternative<std::reference_wrapper<PatchC0>>(sel.second);
            });
            if(!patches.empty()) {
                std::transform(patches.begin(), patches.end(), std::back_inserter(patchIds), [](auto &patch) {
                    return std::get<std::reference_wrapper<PatchC0>>(patch.second).get().id;
                });

                std::stringstream ss;
                ss<<"Trying to find holes for patches ";
                for(auto &i : patchIds) ss << std::to_string(i) << " ";
                appState.logger.logDebug(ss.str());

                appState.gregoryPatchCreator.findHoles(appState, patchIds, holeSize);
            }
        }
        if(ImGui:: InputInt("Hole Size", &holeSize) && holeSize < 2)
            holeSize = 2;


        // Hole list
        if (ImGui::BeginListBox("Found holes")) {
            auto holes = appState.gregoryPatchCreator.getHolePoints();
            for(int i = 0; i < holes.size(); i++) {
                if (ImGui::Selectable(("Hole No." + std::to_string(i)).c_str(), i == selectedHole)) {
                    selectedHole = i;
                    appState.eventPublisher.publish(ResetSelectionEvent{});
                    auto temp = appState.keyboardCtrlMode;
                    appState.keyboardCtrlMode = true;
                    for(auto &point : holes[i])
                        appState.eventPublisher.publish(SelectEntityEvent{*appState.pointSet[point], 0});
                    appState.keyboardCtrlMode = temp;
                }
            }
            ImGui::EndListBox();
        }

        ImGui::BeginDisabled(selectedHole < 0);
        if(ImGui::Button("Fill hole")) {
            appState.gregoryPatchCreator.prepareHole(selectedHole);
            appState.gregoryPatchCreator.fillHole(appState);

            // TODO handle appState adding of Gregory patches and all ui stuff
            appState.eventPublisher.publish(CreateGregoryPatchEvent{appState.gregoryPatchCreator});
        }
        ImGui::EndDisabled();
        ImGui::End();
    }
}
