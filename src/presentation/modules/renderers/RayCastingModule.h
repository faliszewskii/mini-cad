//
// Created by faliszewskii on 21.02.24.
//

#ifndef OPENGL_SANDBOX_RAYCASTINGMODULE_H
#define OPENGL_SANDBOX_RAYCASTINGMODULE_H

#include "../Module.h"
#include "../../scene/nodes/shader/Shader.h"
#include "../../../logic/io/IOUtils.h"
#include "../../scene/nodes/mesh/Mesh.h"
#include "../../../logic/generator/MeshGeneratorHelpers.h"
#include "../../../logic/state/AppState.h"
#include <glm/gtc/type_ptr.hpp>
#include "RenderHelpers.h"

class RayCastingModule : public Module {
    const int workspaceWidth;
    Shader shader;
    std::unique_ptr<Mesh> mesh;
    float specularFactor;

    int textureWidth;
    int textureHeight;
    int texelSize;
    unsigned int textureId;
    float* coordinatesTexture = nullptr;

    int startingPixelSize;
    int pixelSize;
    bool shouldUpdate;

    int pixelsPerLoop;
    int currentX;
    int currentY;

    float semiAxisA;
    float semiAxisB;
    float semiAxisC;

    glm::mat4 viewInv;
    glm::mat4 projInv;
    glm::mat4 view;
    glm::mat4 proj;

public:
    explicit RayCastingModule(int workspaceWidth) : Module(true), workspaceWidth(workspaceWidth), specularFactor(128),
        semiAxisA(0.75), semiAxisB(0.5), semiAxisC(1), shouldUpdate(true), startingPixelSize(1 << 5), pixelSize(startingPixelSize),
        pixelsPerLoop(100), currentX(0), currentY(0),
        shader(Shader("RayCasting", IOUtils::getResource("shaders/rayCasting/rayCasting.vert"), IOUtils::getResource("shaders/rayCasting/rayCasting.frag"))){
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        auto normal = glm::vec3(0,1,0);

        vertices.push_back(Vertex(glm::vec3(1,1,0), glm::vec3(0,1,0), glm::vec2(1,1)));
        vertices.push_back(Vertex(glm::vec3(1,-1,0), glm::vec3(0,1,0), glm::vec2(1,0)));
        vertices.push_back(Vertex(glm::vec3(-1,-1,0), glm::vec3(0,1,0), glm::vec2(0,0)));
        vertices.push_back(Vertex(glm::vec3(-1,1,0), glm::vec3(0,1,0), glm::vec2(0,1)));

        MeshGeneratorHelpers::addQuad(indices,0, 1, 2, 3);

        mesh = std::make_unique<Mesh>(Mesh("Plane mesh", vertices, indices, {}, GL_TRIANGLES));
    }

    void run(AppState &appState) override {
        if(coordinatesTexture == nullptr) setupCoordinateTexture();
        adaptiveRendering(appState);
        if(!appState.currentCamera) return;
        auto &io = ImGui::GetIO();
        glViewport(workspaceWidth, 0, io.DisplaySize.x - workspaceWidth, io.DisplaySize.y);
        Camera &camera = appState.currentCamera->get();
        shader.use();
        shader.setUniform("near", camera.nearPlane);
        shader.setUniform("far", camera.farPlane);
        shader.setUniform("specularFactor", specularFactor);
        RenderHelpers::setUpCamera(camera, shader);
        RenderHelpers::setUpLights(appState.lights, shader);
        shader.setUniform("coordinatesTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        shader.setUniform("pixelSize", pixelSize);
        shader.setUniform("viewportOffsetX", workspaceWidth);
        shader.setUniform("viewportOffsetY", 0);
        shader.setUniform("semiAxisA", semiAxisA);
        shader.setUniform("semiAxisB", semiAxisB);
        shader.setUniform("semiAxisC", semiAxisC);
        mesh->render();
    }

    [[nodiscard]] std::string getName() const override {
        return "Ray Casting Module";
    }

    void setupCoordinateTexture() {
        auto &io = ImGui::GetIO();

        textureWidth = io.DisplaySize.x - workspaceWidth;
        textureHeight =  io.DisplaySize.y;
        texelSize = 3;
        coordinatesTexture = new float [texelSize * textureHeight * textureWidth];
        for(int y = 0; y < textureHeight; y++) {
            for(int x = 0; x < textureWidth; x++) {
                assert(texelSize == 3);
                coordinatesTexture[(x + y * textureWidth)*texelSize + 0] = 0;//(x%3 == 0? 1.f: 0.f);
                coordinatesTexture[(x + y * textureWidth)*texelSize + 1] = 0;//(x%3 == 1? 1.f: 0.f) ;
                coordinatesTexture[(x + y * textureWidth)*texelSize + 2] = 0;//(x%3 == 2? 1.f: 0.f);
            }
        }

        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, textureWidth, textureHeight, 0, GL_RGB, GL_FLOAT, coordinatesTexture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void adaptiveRendering(AppState &appState) {
        if(!appState.currentCamera) return;
        Camera& camera = appState.currentCamera->get();
        viewInv = glm::inverse(camera.getViewMatrix());
        projInv = glm::inverse(camera.getProjectionMatrix());
        view = camera.getViewMatrix();
        proj = camera.getProjectionMatrix();
        if(shouldUpdate) {
            pixelSize = startingPixelSize;
            for(int currentY = 0;currentY < textureHeight; currentY+=pixelSize) {
                for(int currentX = 0;currentX < textureWidth; currentX+=pixelSize) {
                    glm::vec3 vPixel = findElipsoidCoords( (float)currentX/textureWidth, (float)currentY/textureHeight);
                    glTexSubImage2D (GL_TEXTURE_2D, 0, currentX, currentY, 1, 1, GL_RGB, GL_FLOAT, glm::value_ptr(vPixel));
                }
            }
            shouldUpdate = false;
        } else if (pixelSize > 1) {
            pixelSize /= 2;
            for(int currentY = 0;currentY < textureHeight; currentY+=pixelSize) {
                for(int currentX = 0;currentX < textureWidth; currentX+=pixelSize) {
                    if(currentX/pixelSize%2 == 0 && currentY/pixelSize%2 == 0) continue;
                    glm::vec3 vPixel = findElipsoidCoords((float)currentX/textureWidth, (float)currentY/textureHeight);
                    glTexSubImage2D (GL_TEXTURE_2D, 0, currentX, currentY, 1, 1, GL_RGB, GL_FLOAT, glm::value_ptr(vPixel));
                }
            }
        }
        shouldUpdate = !appState.guiFocus;
    }

    glm::vec3 findElipsoidCoords(float x, float y) {
        x = x*2-1;
        y = y*2-1;
        float a2 = 1/(semiAxisA*semiAxisA);
        float b2 = 1/(semiAxisB*semiAxisB);
        float c2 = 1/(semiAxisC*semiAxisC);
        glm::vec3 nearPoint = unprojectPoint(x, y, 0.0);
        glm::vec3 farPoint = unprojectPoint(x, y, 1.0);
        float x21 = farPoint.x - nearPoint.x;
        float y21 = farPoint.y - nearPoint.y;
        float z21 = farPoint.z - nearPoint.z;
        float a = a2*pow(x21,2) + b2*pow(y21,2) + c2*pow(z21,2);
        float b =2 * (a2*x21*(nearPoint.x - 0) + b2*y21*(nearPoint.y - 0) + c2*z21*(nearPoint.z - 0));
        float c = 0 + 0 + 0 + a2*nearPoint.x*nearPoint.x + b2*nearPoint.y*nearPoint.y + c2*nearPoint.z*nearPoint.z -
                  2 * (0 + 0 + 0 ) - 1*1;

        auto fragPos3D = glm::vec3(0);
        if(b * b - 4 * a * c >= 0) {
            float t1 = ((-b + sqrt(b * b - 4 * a * c)) / (2 * a));
            float t2 = ((-b - sqrt(b * b - 4 * a * c)) / (2 * a));
            glm::vec3 fragPos3D1 = nearPoint + t1 * (farPoint - nearPoint);
            glm::vec3 fragPos3D2 = nearPoint + t2 * (farPoint - nearPoint);
            float d1 = computeDepth(fragPos3D1);
            float d2 = computeDepth(fragPos3D2);
            fragPos3D = d1 < d2 ? fragPos3D1 : fragPos3D2;
        } else return {0, 0, 0}; // TODO Float MIN or INF
        return fragPos3D;
    }

    float computeDepth(glm::vec3 pos) {
        glm::vec4 clip_space_pos = proj * view * glm::vec4(pos, 1.0);
        return (clip_space_pos.z / clip_space_pos.w);
    }

    glm::vec3 unprojectPoint(float x, float y, float z) {
        glm::vec4 unprojectedPoint =  viewInv * projInv * glm::vec4(x, y, z, 1.0);
        return glm::vec3(unprojectedPoint) / unprojectedPoint.w;
    }

    ~RayCastingModule() override {
        delete coordinatesTexture;
    }
};
#endif //OPENGL_SANDBOX_RAYCASTINGMODULE_H
