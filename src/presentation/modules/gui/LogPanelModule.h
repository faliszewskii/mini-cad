//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_LOGPANELMODULE_H
#define OPENGL_SANDBOX_LOGPANELMODULE_H

#include "../Module.h"

class LogPanelModule : public Module {
public:
    explicit LogPanelModule() : Module(true) {};

    void run(AppState &appState) final {
        ImGuiWindowFlags window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        const float PAD = 10.0f;
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = work_pos.x + work_size.x - PAD;
        window_pos.y = work_pos.y + work_size.y - PAD;
        window_pos_pivot.x = 1.0f;
        window_pos_pivot.y = 1.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        ImGui::SetNextWindowSize(ImVec2(600, 50));
        if (ImGui::Begin("Log Overlay", nullptr, window_flags)) {
            ImGui::Text("Log messages will go here");
        }
        ImGui::End();
    }

    [[nodiscard]] std::string getName() const final {
        return "Log Panel Module";
    }
};

#endif //OPENGL_SANDBOX_LOGPANELMODULE_H
