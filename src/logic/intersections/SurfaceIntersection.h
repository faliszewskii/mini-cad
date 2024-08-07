//
// Created by faliszewskii on 03.08.24.
//

#ifndef OPENGL_SANDBOX_SURFACEINTERSECTION_H
#define OPENGL_SANDBOX_SURFACEINTERSECTION_H

#include "ParametricSurface.h"

class SurfaceIntersection {
public:
    int subdivisionCount = 64;
    int subdivisionIterations = 10;

    int gradientIterationLimit = 1000;
    float gradientStartStep = 0.1;
    float gradientPrecision = std::numeric_limits<float>::epsilon() * 50;

    float intersectionPointsDistance = 0.01;
    float intersectionStartStep = 0.001;
    int intersectionIterations = 1000;
    float intersectionPrecision = std::numeric_limits<float>::epsilon() * 50;


    template<ParametricSurface S>
    glm::vec2 getClosestPoint(const S &surfaceA, glm::vec3 referencePoint) {
        float div = 1.0f / subdivisionCount;
        float minU = 0.0f, minV = 0.0f;

        for(int it = 0; it < subdivisionIterations; it++) {
            float minDist = std::numeric_limits<float>::max();
            int bestI = 0, bestJ = 0;
            float div2 = div / 2.f;

            for(int i = 0; i < subdivisionCount; i++) {
                for(int j = 0; j < subdivisionCount; j++) {
                    float u = minU + div2 + i * div;
                    float v = minV + div2 + j * div;
                    float dist = glm::distance(surfaceA.evaluate(u, v), referencePoint);
//                    std::cout <<"u: " << u << ", v: " << v << ". Dist: " << dist << std::endl;
                    if(dist < minDist) {
                        minDist = dist;
                        bestI = i;
                        bestJ = j;
                    }
                }
            }
            minU += bestI * div;
            minV += bestJ * div;
            div /= subdivisionCount;
        }

        return {minU + div / 2.0f, minV + div / 2.0f};
    }

    template<ParametricSurface S1, ParametricSurface S2>
    glm::vec4 getGradient(const S1 &surfaceA, float u, float v, const S2 &surfaceB, float s, float t) {
        glm::vec3 evalA = surfaceA.evaluate(u, v);
        glm::vec3 evalB = surfaceB.evaluate(s, t);
        glm::vec3 evalDiff = evalA - evalB;

        glm::vec3 evalAU = surfaceA.evaluateDU(u, v);
        glm::vec3 evalAV = surfaceA.evaluateDV(u, v);

        glm::vec3 evalBU = surfaceA.evaluateDU(s, t);
        glm::vec3 evalBV = surfaceA.evaluateDV(s, t);

        return {
                glm::dot(evalDiff, evalAU) * 2.0f,
                glm::dot(evalDiff, evalAV) * 2.0f,
                glm::dot(-evalDiff, evalBU) * 2.0f,
                glm::dot(-evalDiff, evalBV) * 2.0f
        };
    }

    template<ParametricSurface S1, ParametricSurface S2>
    std::vector<glm::vec3> findIntersection(const S1 &surfaceA, const S2 &surfaceB, bool sameSurface, glm::vec3 cursor) {
        if(sameSurface)
            throw std::runtime_error("Self-intersections not supported!");

        auto pointA = getClosestPoint(surfaceA, cursor);
        auto pointB = getClosestPoint(surfaceB, cursor);
        float step = gradientStartStep;
        float dist = glm::distance(surfaceA.evaluate(pointA.x, pointA.y), surfaceB.evaluate(pointB.x, pointB.y));

        std::vector<glm::vec3> intersectionPoints;
        for(int k = 0; k < gradientIterationLimit && dist > gradientPrecision; k++) {
            glm::vec4 gradient = getGradient(surfaceA, pointA.x, pointA.y, surfaceB, pointB.x, pointB.y);
            pointA.x -= gradient.x * step;
            pointA.y -= gradient.y * step;
            pointB.x -= gradient.z * step;
            pointB.y -= gradient.w * step;

            float currDist = glm::distance(surfaceA.evaluate(pointA.x, pointA.y), surfaceB.evaluate(pointB.x, pointB.y));
            if(currDist > dist)
                step /= 2;
            dist = currDist;
        }
        if(dist > gradientPrecision)
            return {};

        intersectionPoints.push_back(surfaceA.evaluate(pointA.x, pointA.y)); // First point on the intersection
        intersectionPoints.push_back(surfaceB.evaluate(pointB.x, pointB.y)); // First point on the intersection

        float u = pointA.x, v = pointA.y, s = pointB.x, t = pointB.y;
        for(int k = 0; k < 1; k++) {
            float step = intersectionStartStep;
            glm::vec3 p0 = surfaceA.evaluate(u, v);
            glm::vec3 p = surfaceA.evaluate(u, v);
            glm::vec3 q = surfaceB.evaluate(s, t);

            glm::vec3 PdU = surfaceA.evaluateDU(u, v);
            glm::vec3 PdV = surfaceA.evaluateDV(u, v);
            glm::vec3 QdU = surfaceB.evaluateDU(s, t);
            glm::vec3 QdV = surfaceB.evaluateDV(s, t);

            glm::vec3 pN = glm::normalize(glm::cross(PdU, PdV));
            glm::vec3 qN = glm::normalize(glm::cross(QdU, QdV));

            glm::vec3 T = glm::normalize(glm::cross(pN, qN));
            for(int it = 0; it < intersectionIterations; it++) {
                glm::vec3 p = surfaceA.evaluate(u, v);
                glm::vec3 q = surfaceB.evaluate(s, t);

                glm::vec3 PdU = surfaceA.evaluateDU(u, v);
                glm::vec3 PdV = surfaceA.evaluateDV(u, v);
                glm::vec3 QdU = surfaceB.evaluateDU(s, t);
                glm::vec3 QdV = surfaceB.evaluateDV(s, t);

//                if(glm::length(glm::vec4(p - q, glm::dot(p - p0, T) - step)) > dist)
//                    step /= 2;
                glm::vec4 f = glm::vec4(p - q, glm::dot(p - p0, T) - step);
                dist = glm::length(f);

                glm::vec4 FdPu = glm::vec4(PdU, glm::dot(PdU, T));
                glm::vec4 FdPv = glm::vec4(PdV, glm::dot(PdV, T));
                glm::vec4 FdQu = glm::vec4(-QdU, 0);
                glm::vec4 FdQv = glm::vec4(-QdV, 0);

                glm::mat4 m = glm::mat4(FdPu, FdPv, FdQu, FdQv);
                m = glm::inverse(m);

                glm::vec4 x = {u, v, s, t};
                glm::vec4 dx = -m * f;
                x = x + dx;

                u = x.x;
                v = x.y;
                s = x.z;
                t = x.w;
                if(glm::length(dx) < intersectionPrecision)
                    break;
            }
            intersectionPoints.push_back(surfaceA.evaluate(u, v));
            intersectionPoints.push_back(surfaceB.evaluate(s, t));
        }



        return intersectionPoints;
    }

};


#endif //OPENGL_SANDBOX_SURFACEINTERSECTION_H
