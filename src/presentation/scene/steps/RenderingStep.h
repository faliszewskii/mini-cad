//
// Created by faliszewskii on 01.02.24.
//

#ifndef OPENGL_SANDBOX_RENDERINGSTEP_H
#define OPENGL_SANDBOX_RENDERINGSTEP_H


#define UUID_SYSTEM_GENERATOR
#include "../../../../lib/uuid/uuid.h"
#include "../nodes/material/Material.h"
#include "../nodes/transformation/Transformation.h"
#include "../nodes/camera/Camera.h"
#include "../nodes/shader/Shader.h"
#include "../nodes/frameBuffer/FrameBuffer.h"
#include "../nodes/mesh/Mesh.h"
#include "../nodes/light/Light.h"
#include "../visitor/StepVisitor.h"

struct RenderingStep {
    uuids::uuid uniqueObjectId;
    std::vector<std::unique_ptr<RenderingStep>> children;

    explicit RenderingStep() : uniqueObjectId(uuids::uuid_system_generator{}()) {}

    [[nodiscard]] uuids::uuid getUuid() const { return uniqueObjectId; };
    [[nodiscard]] std::vector<std::unique_ptr<RenderingStep>> &getChildren() { return children; };
    [[nodiscard]] virtual bool isLeaf() { return true; };
    [[nodiscard]] virtual std::string getTypeName() const = 0;

    template<typename T>
    RenderingStep& addChild(T &&childStep) {
        children.push_back(std::make_unique<T>(std::forward<T>(childStep)));
        return *children[children.size()-1];
    };
    RenderingStep& addChild(std::unique_ptr<RenderingStep> childStep) {
        children.push_back(std::move(childStep));
        return *children[children.size()-1];
    };

    static void visitTree(RenderingStep &treeNode, StepVisitor &visitor) {
        std::stack<std::pair<std::reference_wrapper<RenderingStep>, int>> stack{};
        stack.emplace(treeNode, 0);
        while (!stack.empty()) {
            RenderingStep &node = stack.top().first;
            int state = stack.top().second;
            stack.pop();
            if (state == 0) {
                stack.emplace(node, 1);
                if (!node.acceptVisit(visitor))
                    for (auto &child: std::ranges::views::reverse(node.children))
                        stack.emplace(*child, 0);
            }
            if (state == 1) node.acceptLeave(visitor);
        }
    };

    virtual int acceptVisit(StepVisitor &visitor) = 0;
    virtual int acceptLeave(StepVisitor &visitor) { return 0; }

    friend bool operator==(RenderingStep &lhs, RenderingStep &rhs) { return std::addressof(lhs) == std::addressof(rhs); }

    virtual ~RenderingStep() = default;
    RenderingStep (const RenderingStep&) = delete;
    RenderingStep& operator= (const RenderingStep&) = delete;
    RenderingStep(RenderingStep &&other) noexcept {
        uniqueObjectId = other.uniqueObjectId;
        children.insert(children.end(), std::make_move_iterator(other.children.begin()), std::make_move_iterator(other.children.end()));
    }
    RenderingStep& operator=(RenderingStep &&other) noexcept = delete;
};

struct RootStep : RenderingStep {
    [[nodiscard]] std::string getTypeName() const override { return "RootStep"; };
    [[nodiscard]] bool isLeaf() override { return false; };

    int acceptVisit(StepVisitor &visitor) override { return visitor.visitRootStep(*this); }
    int acceptLeave(StepVisitor &visitor) override { return visitor.leaveRootStep(*this); }
};

struct AddLight : RenderingStep {
    Light& light; // TODO Deleting of original light should not be possible until all steps are detached.

    explicit AddLight(Light& light) : light(light) {};

    [[nodiscard]] std::string getTypeName() const override { return "AddLight"; };

    int acceptVisit(StepVisitor &visitor) override { return visitor.visitAddLight(*this); }
    int acceptLeave(StepVisitor &visitor) override { return 0; }
};

struct AddCamera : RenderingStep {
    Camera& camera;

    explicit AddCamera(Camera& camera) : camera(camera) {};

    [[nodiscard]] std::string getTypeName() const override { return "AddCamera"; };

    int acceptVisit(StepVisitor &visitor) override { return visitor.visitAddCamera(*this); }
    int acceptLeave(StepVisitor &visitor) override { return 0; }
};

struct AddFrameBuffer : RenderingStep {
    FrameBuffer& frameBuffer;

    explicit AddFrameBuffer(FrameBuffer &frameBuffer) : frameBuffer(frameBuffer) {};

    [[nodiscard]] std::string getTypeName() const override { return "AddFrameBuffer"; };

    int acceptVisit(StepVisitor &visitor) override { return visitor.visitAddFrameBuffer(*this); }
    int acceptLeave(StepVisitor &visitor) override { return 0; }
};

struct AddMaterial : RenderingStep {
    Material& material;

    explicit AddMaterial(Material &material) : material(material) {};

    [[nodiscard]] std::string getTypeName() const override { return "AddMaterial"; };

    int acceptVisit(StepVisitor &visitor) override { return visitor.visitAddMaterial(*this); }
    int acceptLeave(StepVisitor &visitor) override { return 0; }
};

struct PushTransformation : RenderingStep {
    Transformation& transformation;

    explicit PushTransformation(Transformation &transformation) : transformation(transformation) {};

    [[nodiscard]] std::string getTypeName() const override { return "PushTransformation"; };
    [[nodiscard]] bool isLeaf() override { return false; };

    int acceptVisit(StepVisitor &visitor) override { return visitor.visitPushTransformation(*this); }
    int acceptLeave(StepVisitor &visitor) override { return visitor.leavePushTransformation(*this); }
};

struct DrawMesh : RenderingStep {
    Mesh& mesh;

    explicit DrawMesh(Mesh &mesh) : mesh(mesh) {};

    [[nodiscard]] std::string getTypeName() const override { return "DrawMesh"; };

    int acceptVisit(StepVisitor &visitor) override { return visitor.visitDrawMesh(*this); }
    int acceptLeave(StepVisitor &visitor) override { return 0; }
};

struct ActivateShader : RenderingStep {
    Shader& shader;

    explicit ActivateShader(Shader &shader) : shader(shader) {};

    [[nodiscard]] std::string getTypeName() const override { return "ActivateShader"; };

    int acceptVisit(StepVisitor &visitor) override { return visitor.visitActivateShader(*this); }
    int acceptLeave(StepVisitor &visitor) override { return 0; }
};


#endif //OPENGL_SANDBOX_RENDERINGSTEP_H
