//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_DEBUGOVERLAYMODULE_H
#define OPENGL_SANDBOX_DEBUGOVERLAYMODULE_H

#include "../Module.h"
#include "imgui.h"

class DebugOverlayModule : public Module {
    const int workspaceWidth;
public:
    explicit DebugOverlayModule(const int workspaceWidth) : Module(true), workspaceWidth(workspaceWidth) {}

    void run(AppState &appState) final {
        ImGuiWindowFlags window_flags =
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        const float PAD = 10.0f;
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = work_pos.x + PAD + workspaceWidth;
        window_pos.y = work_pos.y + work_size.y - PAD;
        window_pos_pivot.x = 0.0f;
        window_pos_pivot.y = 1.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Debug Overlay", nullptr, window_flags)) {
            ImGui::Text("FPS: TODO");
            ImGui::Separator();
            ImGui::Text("mSPF: TODO");
        }
        ImGui::End();
    }

    [[nodiscard]] std::string getName() const final {
        return "Debug Overlay Module";
    }
};

#endif //OPENGL_SANDBOX_DEBUGOVERLAYMODULE_H
