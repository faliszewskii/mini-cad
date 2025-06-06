//
// Created by faliszewskii on 13.02.24.
//

#ifndef OPENGL_SANDBOX_DEBUGOVERLAYMODULE_H
#define OPENGL_SANDBOX_DEBUGOVERLAYMODULE_H

#include "imgui.h"
#include "imgui_internal.h"

class DebugOverlayModule {
    const int workspaceWidth;
public:
    explicit DebugOverlayModule(const int workspaceWidth) : workspaceWidth(workspaceWidth) {}

    void run(AppState &appState) {
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
            auto &io = ImGui::GetIO();
            const static auto getter = [](void* data, int i){return static_cast<float*>(data)[i];};
            static float delta = 0;
            delta += io.DeltaTime;
            if(delta > 0.2) {
                delta = 0;
                appState.rollingMspf.first = (appState.rollingMspf.first + 1) % appState.rollingMspf.second.size();
                appState.rollingMspf.second[appState.rollingMspf.first] = io.Framerate ? 1000.0f / io.Framerate : 0.0f;
            }
            ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
            // TODO Ironically after implementing this I noticed a crash-like slowdown of the app.
            ImGui::PlotEx(ImGuiPlotType_Lines, "", getter, appState.rollingMspf.second.data(), appState.rollingMspf.second.size(), appState.rollingMspf.first+1, NULL, 0.0f, 100, ImVec2(0, 40));
        }
        ImGui::End();
    }
};

#endif //OPENGL_SANDBOX_DEBUGOVERLAYMODULE_H
