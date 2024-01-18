//
// Created by faliszewskii on 17.01.24.
//

#include "EditorNodeVisitor.h"
#include "imgui.h"
#include "../../../../lib/imgui-node-editor/imgui_node_editor.h"
#include "../../scene/nodes/material/Material.h"
#include "../../scene/nodes/transformation/Transformation.h"
#include "../../scene/nodes/mesh/Mesh.h"
#include "../../scene/nodes/camera/Camera.h"
#include "../../scene/nodes/light/Light.h"
#include "../../scene/nodes/frameBuffer/FrameBuffer.h"

namespace NE = ax::NodeEditor;
int EditorNodeVisitor::visitFrameBuffer(FrameBuffer &frameBuffer) {
    return drawNode(frameBuffer);
}

int EditorNodeVisitor::visitTransformation(Transformation &transformation) {
    return drawNode(transformation);
}

int EditorNodeVisitor::visitMesh(Mesh &mesh) {
    return drawNode(mesh);
}

int EditorNodeVisitor::visitLight(Light &light) {
    return drawNode(light);
}

int EditorNodeVisitor::visitCamera(Camera &camera) {
    return drawNode(camera);
}

int EditorNodeVisitor::visitShader(Shader &shader) {
    return drawNode(shader);
}

int EditorNodeVisitor::visitMaterial(Material &material) {
    return drawNode(material);
}

int EditorNodeVisitor::drawNode(SceneNode& node) {
    // Start drawing nodes.
    NE::BeginNode(uniqueId++);
    ImGui::Text("%s", node.getName().c_str());
    ImGui::BeginGroup();
    NE::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Input);
    ImGui::Text("-> In");
    NE::EndPin();
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    node.acceptVisit(nodeDetailsVisitor);
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    NE::BeginPin(uniqueId++, ax::NodeEditor::PinKind::Output);
    ImGui::Text("Out ->");
    NE::EndPin();
    ImGui::EndGroup();
    NE::EndNode();
    return 0;
}