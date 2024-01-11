//
// Created by faliszewskii on 07.01.24.
//

#ifndef OPENGL_SANDBOX_SCENENODEVISITOR_H
#define OPENGL_SANDBOX_SCENENODEVISITOR_H


class Transformation;
class Mesh;
class Light;
class PointLight;
class Camera;
class Shader;

class SceneNodeVisitor {
public:
    virtual int visitTransformation(Transformation& transformation) { return 0; };
    virtual int visitMesh(Mesh& mesh) { return 0; };
    virtual int visitLight(Light& light) { return 0; };
    virtual int visitPointLight(PointLight& pointLight) { return 0; };
    virtual int visitCamera(Camera& camera) { return 0; };
    virtual int visitShader(Shader& shader) { return 0; }

    virtual int leaveTransformation(Transformation& transformation) { return 0; };
    virtual int leaveMesh(Mesh& mesh) { return 0; };
    virtual int leaveLight(Light& light) { return 0; };
    virtual int leavePointLight(PointLight& pointLight) { return 0; };
    virtual int leaveCamera(Camera& camera) { return 0; };
    virtual int leaveShader(Shader& shader) { return 0; }

protected:
    SceneNodeVisitor() = default;
};


#endif //OPENGL_SANDBOX_SCENENODEVISITOR_H
