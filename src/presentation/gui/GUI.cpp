//
// Created by faliszewskii on 30.12.23.
//

#include <nfd.h>
#include "GUI.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

GUI::GUI(GLFWwindow *window, ApplicationState& state) : guiState(state){

    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
}

void GUI::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::render() {
    ImGui::ShowDemoWindow();

    renderMenuBar();
    renderDebugOverlay();

    if(guiState.activeViewsMask & ViewsMask::ModelsView) renderModelWindow();
    if(guiState.activeViewsMask & ViewsMask::ShadersView) renderShaderWindow();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::renderMenuBar() {
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Load Model")) {
                NFD_Init();

                nfdchar_t *outPath;
                nfdfilteritem_t filterItem[1] {{ "3D models", "gltf,fbx,FBX,obj" } };
                nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
                if (result == NFD_OKAY)
                {
                    std::optional<ModelNode> model = guiState.assetImporter.importModel(outPath);
                    if(model)
                        guiState.rootModelNode.getChildren().push_back(model.value());
                    else
                        printf("Failed to load a file: %s", outPath); // TODO log message board
                    NFD_FreePath(outPath);
                }
                else if (result == NFD_CANCEL)
                {
                }
                else
                {
                    printf("Error: %s\n", NFD_GetError());
                }

                NFD_Quit();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void GUI::renderDebugOverlay() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    const float PAD = 10.0f;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = work_pos.x + PAD;
    window_pos.y = work_pos.y + work_size.y - PAD;
    window_pos_pivot.x = 0.0f;
    window_pos_pivot.y = 1.0f;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    window_flags |= ImGuiWindowFlags_NoMove;
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Debug Overlay", nullptr, window_flags))
    {
        ImGui::Text("FPS: TODO");
        ImGui::Separator();
        ImGui::Text("mSPF: TODO");
    }
    ImGui::End();
}

void GUI::renderModelWindow() {
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
    ImGuiWindowFlags window_flags = 0;
    if (!ImGui::Begin("Models View", nullptr, window_flags))
    {
        ImGui::End();
        return;
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
//    ImGui::Spacing();

    renderModelTreeView();

    ImGui::PopItemWidth();
    ImGui::End();
}

void GUI::renderShaderWindow() {
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
    ImGuiWindowFlags window_flags = 0;
    if (!ImGui::Begin("Shaders View", nullptr, window_flags))
    {
        ImGui::End();
        return;
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
//    ImGui::Spacing();

    renderShaderListView();

    ImGui::PopItemWidth();
    ImGui::End();
}

void GUI::renderModelTreeView() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("Model View", ImVec2(0, 260), ImGuiChildFlags_Border, window_flags);

    ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
    traverseModelNode(guiState.rootModelNode, base_flags);

    // 'selection_mask' is dumb representation of what may be user-side selection state.
    //  You may retain selection state inside or outside your objects in whatever format you see fit.
    // 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
    /// of the loop. May be a pointer to your own node type, etc.
    /*
    static int selection_mask = (1 << 2);
    int node_clicked = -1;
    for (int i = 0; i < 6; i++)
    {
        // Disable the default "open on single-click behavior" + set Selected flag according to our selection.
        // To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
//            ImGuiTreeNodeFlags node_flags = base_flags;
        const bool is_selected = (selection_mask & (1 << i)) != 0;
//            if (is_selected)
//                node_flags |= ImGuiTreeNodeFlags_Selected;
        if (i < 3)
        {
            // Items 0..2 are Tree Node
//                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                node_clicked = i;
            if (ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("This is a drag and drop source");
                ImGui::EndDragDropSource();
            }
//                if (node_open)
//                {
//                    ImGui::BulletText("Blah blah\nBlah Blah");
//                    ImGui::TreePop();
//                }
        }
        else
        {
            // Items 3..5 are Tree Leaves
            // The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
            // use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
//                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
//                ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                node_clicked = i;
            if (ImGui::BeginDragDropSource())
            {
                ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
                ImGui::Text("This is a drag and drop source");
                ImGui::EndDragDropSource();
            }
        }
    }
    if (node_clicked != -1)
    {
        // Update selection state
        // (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
        if (ImGui::GetIO().KeyCtrl)
            selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
        else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
            selection_mask = (1 << node_clicked);           // Click to single-select
    }*/

    ImGui::EndChild();
    ImGui::PopStyleVar();
}

void GUI::traverseModelNode(ModelNode &node, ImGuiTreeNodeFlags flags) {
    std::string nodeName = node.getName().empty()? "[Unnamed node]": node.getName();
    bool node_open = ImGui::TreeNodeEx(uuids::to_string(node.getUuid()).c_str(), flags, "%s", nodeName.c_str());

    flags &= ~ImGuiTreeNodeFlags_DefaultOpen;
    if(node_open) {
        for(ModelNode& child : node.getChildren())
            traverseModelNode(child, flags);
        ImGui::TreePop();

        int meshCounter = 0;
        for(Mesh& mesh : node.getMeshes()) {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx(uuids::to_string(mesh.getUuid()).c_str(), flags, "%s.mesh.%d", node.getName().c_str(), meshCounter++);
        }
    }

}

void GUI::renderShaderListView() {
    if (ImGui::BeginListBox("available shader listing", ImVec2(-FLT_MIN, guiState.availableShaders.size() * ImGui::GetTextLineHeightWithSpacing())))
    {
        for (auto & shader : guiState.availableShaders) {
            const bool is_selected = ( std::addressof(shader) == std::addressof(*guiState.globalShader));
            ImGui::PushID(to_string(shader.getUuid()).c_str());
            if (ImGui::Selectable(shader.getName().c_str(), is_selected))
                guiState.globalShader = &shader;
            ImGui::PopID();
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
    if(ImGui::SmallButton("Hot reload"))
        guiState.globalShader->hotReload();
}
