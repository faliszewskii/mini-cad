//
// Created by faliszewskii on 30.12.23.
//

#include <nfd.h>
#include <glm/gtc/type_ptr.hpp>
#include "GUI.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "tree/TreeViewVisitor.h"
#include "node/NodeDetailsVisitor.h"
#include "../../../lib/imguizmo/ImGuizmo.h"
#include "editor/EditorNodeVisitor.h"
#include "../../logic/generator/ModelGenerator.h"
#include "tree/StepTreeViewVisitor.h"
#include "node/StepParametersVisitor.h"

GUI::GUI(GLFWwindow *window, AppState &state) : guiState(state) {
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ax::NodeEditor::Config config;
    config.SettingsFile = "Simple.json";
    m_Context = ax::NodeEditor::CreateEditor(&config);
}

void GUI::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
    ImGuizmo::Enable(true);
}

void GUI::render() {
    ImGui::ShowDemoWindow();

    renderMenuBar();
    renderDebugOverlay();
    renderLogOverlay();

    if (guiState.activeViewsMask & ViewsMask::MainView) renderMainWindow();
    renderEditorWindow();
    renderGizmo();
//    if(guiState.activeViewsMask & ViewsMask::ShadersView) renderShaderWindow();


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::renderMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::Separator();
            renderMenuItemLoadModel();
            if (ImGui::MenuItem("Add Axis") && guiState.selectedStep) { // TODO Grey out on no select with tooltip.
                guiState.scene.merge(ModelGenerator::generateAxis());
            }
            if (ImGui::MenuItem("Add Sphere") && guiState.selectedStep) {
                guiState.scene.merge(ModelGenerator::generateSphere(50, 50));
            }
            if (ImGui::MenuItem("Add Torus") && guiState.selectedStep) {
                guiState.scene.merge(ModelGenerator::generateTorus(50, 50));
            }
            if (ImGui::MenuItem("Add Hyperbolic Paraboloid") && guiState.selectedStep) {
                float a = 1, b = 1, c = -0.5;
                auto generator = ModelGenerator::HyperbolicParaboloidGenerator(a, b, c);
                auto mesh = generator.generateParametrisedMesh("Hyperbolic Paraboloid Mesh", 10, 10);
                auto model = ModelGenerator::generateSolid(std::move(mesh), "Hyperbolic Paraboloid model");
                guiState.scene.merge(std::move(model));
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
//void GUI::addModel(std::unique_ptr<SceneNode> &&model) {
//    guiState.selectedNode->get().addChild(*model);
//    guiState.allNodes.push_back(std::move(model));
//}

//void GUI::addModel(std::vector<std::unique_ptr<SceneNode>> &&model) {
//    guiState.selectedNode->get().addChild(*model[0]);
//    guiState.allNodes.insert(guiState.allNodes.begin(), std::make_move_iterator(model.begin()),
//                             std::make_move_iterator(model.end()));
//}

void GUI::renderMenuItemLoadModel() {
    if (ImGui::MenuItem("Load Model") && guiState.selectedStep && !guiState.selectedStep->get().isLeaf()) { // TODO Grey out on no select with tooltip.
        NFD_Init();

        nfdchar_t *outPath;
        nfdfilteritem_t filterItem[1]{{"3D models", "gltf,fbx,FBX,obj"}};
        nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
        if (result == NFD_OKAY) {
            try {
                auto model = guiState.assetImporter.importModel(outPath);
                guiState.scene.merge(std::move(model), guiState.selectedStep->get());
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
}

void GUI::renderDebugOverlay() {
    ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    const float PAD = 10.0f;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = work_pos.x + PAD + guiState.guiWidth;
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

void GUI::renderLogOverlay() {
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

void GUI::renderMainWindow() {
    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y));
    ImGui::SetNextWindowSize(ImVec2(guiState.guiWidth, main_viewport->WorkSize.y));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
    if (!ImGui::Begin("Main View", nullptr, window_flags)) {
        ImGui::End();
        return;
    }
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    renderModelTreeView();

    ImGui::PopItemWidth();
    ImGui::End();
}

void GUI::renderEditorWindow() {
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGuiWindowFlags window_flags = 0;
    if (!ImGui::Begin("Editor View", nullptr, window_flags)) {
        ImGui::End();
        return;
    }
    ImGui::PushItemWidth(ImGui::GetFontSize());

    auto &io = ImGui::GetIO();



    EditorNodeVisitor editorNodeVisitor;

    ax::NodeEditor::SetCurrentEditor(m_Context);
    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
    ImGui::SameLine();
    if(ImGui::Button("To content")) ax::NodeEditor::NavigateToContent();
    ImGui::Separator();
    ax::NodeEditor::Begin("My Editor", ImVec2(0.0, 0.0f));
    for(auto& node : guiState.allNodes)
        node->acceptVisit(editorNodeVisitor);
    ax::NodeEditor::End();
    firstTime = false;
    ax::NodeEditor::SetCurrentEditor(nullptr);

    //ImGui::ShowMetricsWindow();

    ImGui::PopItemWidth();
    ImGui::End();
}

//void GUI::renderShaderWindow() {
//    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
//    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
//    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
//    ImGuiWindowFlags window_flags = 0;
//    if (!ImGui::Begin("Shaders View", nullptr, window_flags))
//    {
//        ImGui::End();
//        return;
//    }
//    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
//
//    ImGui::PopItemWidth();
//    ImGui::End();
//}

void GUI::renderModelTreeView() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("Model View", ImVec2(0, 260), ImGuiChildFlags_Border, window_flags);

    StepTreeViewVisitor treeViewVisitor(guiState.selectedStep);
    RenderingStep::visitTree(guiState.scene.modelStepRoot->root, treeViewVisitor);

//    traverseModelNode(guiState.rootSceneNode, base_flags);

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

    if (guiState.selectedStep) {
        StepParametersVisitor stepParametersVisitor;
        guiState.selectedStep->get().acceptVisit(stepParametersVisitor);
    }
}

GUI::~GUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ax::NodeEditor::DestroyEditor(m_Context);
}

void GUI::renderGizmo() {
    if (!guiState.selectedNode || !guiState.currentCamera) return;
    auto *transformation = dynamic_cast<Transformation *>(&guiState.selectedNode.value().get());
    if (transformation == nullptr) return;

    glm::mat4 mat = transformation->getTransformation();
    auto *matrix = static_cast<float *>(glm::value_ptr(mat));
    glm::mat4 deltaMatrix;

    ImGuiIO &io = ImGui::GetIO();
    ImGuizmo::SetRect(guiState.guiWidth, 0, io.DisplaySize.x - guiState.guiWidth, io.DisplaySize.y);
    ImGuizmo::Manipulate(
            static_cast<const float *>(glm::value_ptr(guiState.currentCamera.value().get().getViewMatrix())),
            static_cast<const float *>(glm::value_ptr(guiState.currentCamera.value().get().getProjectionMatrix())),
            ImGuizmo::ROTATE, ImGuizmo::LOCAL, matrix, static_cast<float *>(glm::value_ptr(deltaMatrix)),/* useSnap ? &snap.x :*/ NULL
    );
    glm::quat q = glm::normalize(glm::quat_cast(deltaMatrix));
    transformation->setOrientation(q * transformation->getOrientationRef());

}
//
//void GUI::renderShaderListView() {
//    if (ImGui::BeginListBox("available shader listing", ImVec2(-FLT_MIN, guiState.availableShaders.size() * ImGui::GetTextLineHeightWithSpacing())))
//    {
//        for (auto & shader : guiState.availableShaders) {
//            const bool is_selected = ( std::addressof(shader) == std::addressof(guiState.globalShader.get()));
//            ImGui::PushID(to_string(shader.getUuid()).c_str());
//            if (ImGui::Selectable(shader.getName().c_str(), is_selected))
//                guiState.globalShader.get() = shader;
//            ImGui::PopID();
//            if (is_selected)
//                ImGui::SetItemDefaultFocus();
//        }
//        ImGui::EndListBox();
//    }
//    if(ImGui::SmallButton("Hot reload"))
//        guiState.globalShader.get().hotReload();
//}
