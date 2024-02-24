//
// Created by faliszewskii on 21.02.24.
//

#ifndef OPENGL_SANDBOX_RAYCASTINGMODULE_H
#define OPENGL_SANDBOX_RAYCASTINGMODULE_H

#include "../Module.h"
#include "../../../logic/io/IOUtils.h"
#include "../../../logic/generator/MeshGeneratorHelpers.h"
#include "../../../logic/state/AppState.h"
#include <glm/gtc/type_ptr.hpp>
#include "RenderHelpers.h"

class RayCastingModule : public Module {
    const int workspaceWidth;
    Shader shader;
    std::unique_ptr<Mesh> mesh;
    float specularFactor;

    int textureWidth{};
    int textureHeight{};
    int texelSize{};
    unsigned int textureId{};
    float* coordinatesTexture = nullptr;

    int startingPixelSize;
    int pixelSize;
    int shaderPixelSize;
    bool isFirstPass;
    bool firstPassFull;

    int pixelsPerLoop;
    int currentX;
    int currentY;

    float semiAxisA;
    float semiAxisB;
    float semiAxisC;

    glm::vec3 scale;
    glm::vec3 translation;
    glm::vec3 eulerRotation;

    glm::mat4 viewInv{};
    glm::mat4 projInv{};
    glm::mat4 view{};
    glm::mat4 proj{};

public:

    float& getSemiAxisA() { return semiAxisA; };
    float& getSemiAxisB() { return semiAxisB; };
    float& getSemiAxisC() { return semiAxisC; };
    int& getPixelsPerLoop() { return pixelsPerLoop; };
    bool& getFirstPassFull() { return firstPassFull; };
    int& getStartingPixelSize() { return startingPixelSize; };
    float& getSpecularFactor() { return specularFactor; };
    glm::vec3& getScale() { return scale; };
    glm::vec3& getTranslation() { return translation; };
    glm::vec3& getEulerRotation() { return eulerRotation; };

    explicit RayCastingModule(int workspaceWidth) : Module(true), workspaceWidth(workspaceWidth), specularFactor(256),
            semiAxisA(0.5), semiAxisB(0.25), semiAxisC(1), isFirstPass(false), startingPixelSize(16), pixelSize(startingPixelSize),
            shaderPixelSize(pixelSize), pixelsPerLoop(2000), currentX(0), currentY(0),
            firstPassFull(false), scale(glm::vec3(1.f)), translation(0.f), eulerRotation(0.f),
            shader(Shader("Ray Casting", IOUtils::getResource("shaders/rayCasting/rayCasting.vert"), IOUtils::getResource("shaders/rayCasting/rayCasting.frag"))){

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
        shader.setUniform("pixelSize", shaderPixelSize);
        shader.setUniform("viewportOffsetX", workspaceWidth);
        shader.setUniform("viewportOffsetY", 0);
        shader.setUniform("D", DPrim);
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
                coordinatesTexture[(x + y * textureWidth)*texelSize + 0] = 0;
                coordinatesTexture[(x + y * textureWidth)*texelSize + 1] = 0;
                coordinatesTexture[(x + y * textureWidth)*texelSize + 2] = 0;
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
        if(isFirstPass) {
            pixelSize = startingPixelSize;
            shaderPixelSize = pixelSize;
            for(int i = 0; currentY < textureHeight && (i < pixelsPerLoop || firstPassFull); ) {
                for(;currentX < textureWidth && (i < pixelsPerLoop || firstPassFull); currentX+=pixelSize, i++) {
                    glm::vec3 vPixel = findEllipsoidCoords((float) currentX / textureWidth,(float) currentY / textureHeight);
                    glTexSubImage2D (GL_TEXTURE_2D, 0, currentX, currentY, 1, 1, GL_RGB, GL_FLOAT, glm::value_ptr(vPixel));
                }
                if(currentX >= textureWidth) {
                    currentX = 0;
                    currentY+=pixelSize;
                }
            }
            if(currentY >= textureHeight) {
                isFirstPass = false;
                currentY = 0;
                pixelSize /= 2;
            }
        } else {
            if (pixelSize >= 1) {
                for (int i = 0; currentY < textureHeight && i < pixelsPerLoop;) {
                    for (; currentX < textureWidth && i < pixelsPerLoop; currentX += pixelSize, i++) {
                        if (currentX / pixelSize % 2 == 0 && currentY / pixelSize % 2 == 0) continue;
                        glm::vec3 vPixel = findEllipsoidCoords((float) currentX / textureWidth,
                                                               (float) currentY / textureHeight);
                        glTexSubImage2D(GL_TEXTURE_2D, 0, currentX, currentY, 1, 1, GL_RGB, GL_FLOAT,
                                        glm::value_ptr(vPixel));
                    }
                    if (currentX >= textureWidth) {
                        currentY += pixelSize;
                        currentX = 0;
                    }
                }
                if (currentY >= textureHeight) {
                    currentY = 0;
                    shaderPixelSize /= 2;
                    pixelSize /= 2;
                }
            }
            if (!appState.guiFocus && appState.currentCamera) triggerUpdate(appState.currentCamera.value());
        }
    }

    void triggerUpdate(Camera &camera) {
        if(isFirstPass) return;
        isFirstPass = true;
        currentX = 0;
        currentY = 0;

        viewInv = glm::inverse(camera.getViewMatrix());
        projInv = glm::inverse(camera.getProjectionMatrix());
        view = camera.getViewMatrix(); // TODO Manually view and proj matrices
        proj = camera.getProjectionMatrix();

        float a2 = 1/(semiAxisA*semiAxisA);
        float b2 = 1/(semiAxisB*semiAxisB);
        float c2 = 1/(semiAxisC*semiAxisC);

        D = {
                {a2, 0, 0, 0},
                {0, b2, 0, 0},
                {0, 0, c2, 0},
                {0, 0, 0, -1}
        };
        glm::mat4 T = {
                {1,0,0,0},
                {0, 1, 0, 0},
                {0, 0, 1, 0},
                {translation.x, translation.y, translation.z, 1}
        };
        glm::mat4 Rx = {
                {1,0,0,0},
                {0, cos(eulerRotation.x), sin(eulerRotation.x), 0},
                {0, -sin(eulerRotation.x), cos(eulerRotation.x), 0},
                {0, 0, 0, 1}
        };
        glm::mat4 Ry = {
                {cos(eulerRotation.y),0,-sin(eulerRotation.y),0},
                {0, 1, 0, 0},
                {sin(eulerRotation.y), 0, cos(eulerRotation.y), 0},
                {0, 0, 0, 1}
        };
        glm::mat4 Rz = {
                {cos(eulerRotation.z),sin(eulerRotation.z),0,0},
                {-sin(eulerRotation.z), cos(eulerRotation.z), 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1}
        };
        glm::mat4 S = {
                {scale.x,0,0,0},
                {0, scale.y, 0, 0},
                {0, 0, scale.z, 0},
                {0, 0, 0, 1}
        };

        M = T * Rx * Ry * Rz * S;
        invM = glm::inverse(M);
        DPrim =  glm::transpose(invM) * D * invM;

    };

    glm::vec3 findEllipsoidCoords(float x, float y) {
        x = x*2-1;
        y = y*2-1;
        glm::vec3 nearPoint = unprojectPoint(x, y, 0.0);
        glm::vec3 farPoint = unprojectPoint(x, y, 1.0);

        // Here nearPoint and farPoint required.
        float x21 = farPoint.x - nearPoint.x;
        float y21 = farPoint.y - nearPoint.y;
        float z21 = farPoint.z - nearPoint.z;
        float x1 = nearPoint.x;
        float y1 = nearPoint.y;
        float z1 = nearPoint.z;

        A = {
                {x21*x21, x21*y21, x21*z21,0},
                {x21*y21, y21*y21, y21*z21, 0},
                {x21*z21, y21*z21, z21*z21, 0},
                {0, 0, 0, 0}
        };

        B = {
                {2*x1*x21, x1*y21+x21*y1, x1*z21+x21*z1,x21},
                {x1*y21+x21*y1, 2*y1*y21, y1*z21+z1*y21, y21},
                {x1*z21+x21*z1, y1*z21+z1*y21, 2*z1*z21, z21},
                {x21, y21, z21, 0}
        };

        C = {
                {x1*x1, x1*y1, x1*z1,x1},
                {x1*y1, y1*y1, y1*z1, y1},
                {x1*z1, y1*z1, z1*z1, z1},
                {x1, y1, z1, 1}
        };

        float a =0;
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                a += DPrim[i][j] * A[i][j];

        float b = 0;
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                b += DPrim[i][j] * B[i][j];

        float c = 0;
        for(int i = 0; i < 4; i++)
            for(int j = 0; j < 4; j++)
                c += DPrim[i][j] * C[i][j];

        // Here a, b, c required
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

    glm::mat4 A{};
    glm::mat4 B{};
    glm::mat4 C{};
    glm::mat4 D{};
    glm::mat4 DPrim{};
    glm::mat4 M{};
    glm::mat4 invM{};

    float computeDepth(glm::vec3 pos) {
        glm::vec4 clip_space_pos = proj * view * glm::vec4(pos, 1.0);
        return (clip_space_pos.z / clip_space_pos.w);
    }

    glm::vec3 unprojectPoint(float x, float y, float z) {
        glm::vec4 unprojectedPoint = viewInv * projInv * glm::vec4(x, y, z, 1.0);
        return glm::vec3(unprojectedPoint) / unprojectedPoint.w;
    }

    ~RayCastingModule() override {
        delete coordinatesTexture;
    }
};
#endif //OPENGL_SANDBOX_RAYCASTINGMODULE_H
