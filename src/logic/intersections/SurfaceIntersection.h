//
// Created by faliszewskii on 03.08.24.
//

#ifndef OPENGL_SANDBOX_SURFACEINTERSECTION_H
#define OPENGL_SANDBOX_SURFACEINTERSECTION_H

#include <expected>

#include "ParametricSurface.h"
#include "../algorithms/BresenhamAlgorithm.h"

class SurfaceIntersection {
public:
    int subdivisionCount = 64;
    int subdivisionIterations = 100;

    int gradientIterationLimit = 2000;
    float gradientStartStep = 0.1;
    float gradientPrecision = std::numeric_limits<float>::epsilon() * 1000;

    float intersectionPointsDistance = 0.1;
    int intersectionIterations = 1000;
    float intersectionPrecision = std::numeric_limits<float>::epsilon() * 100;
    int maxIntersectionPoints = 1000;
    int smallerStepTriesLimit = 10;

    struct IntersectionResult {
        std::vector<glm::vec3> intersectionPoints;
        std::vector<std::pair<IntersectableSurface, std::vector<glm::vec2>>> surfaces;
        bool wrapped;
    };

    template<ParametricSurface S>
    glm::vec2 getClosestPoint(S &surfaceA, glm::vec3 referencePoint, std::optional<glm::vec2> repulser) {
        float divU = surfaceA.rangeU() / subdivisionCount;
        float divV = surfaceA.rangeV() / subdivisionCount;
        float minU = 0.0f, minV = 0.0f;

        for(int it = 0; it < subdivisionIterations; it++) {
            float minDist = std::numeric_limits<float>::max();
            int bestI = 0, bestJ = 0;

            for(int i = 0; i < subdivisionCount; i++) {
                for(int j = 0; j < subdivisionCount; j++) {
                    float u = minU + divU/2.f + i * divU;
                    float v = minV + divV/2.f + j * divV;
                    float repulsion = repulser.has_value()? std::pow( glm::distance(glm::vec2(u,v), repulser.value()), 1): 0;
                    float dist = glm::distance(surfaceA.evaluate(u, v), referencePoint) - repulsion;
//                    std::cout <<"u: " << u << ", v: " << v << ". Dist: " << dist << std::endl;
                    if(dist < minDist) {
                        minDist = dist;
                        bestI = i;
                        bestJ = j;
                    }
                }
            }
            minU += bestI * divU;
            minV += bestJ * divV;
            divU /= subdivisionCount;
            divV /= subdivisionCount;
        }

        return {minU + divU / 2.0f, minV + divV / 2.0f};
    }

    template<ParametricSurface S1, ParametricSurface S2>
    glm::vec4 getGradient(S1 &surfaceA, float u, float v, S2 &surfaceB, float s, float t) {
        glm::vec3 evalA = surfaceA.evaluate(u, v);
        glm::vec3 evalB = surfaceB.evaluate(s, t);
        glm::vec3 evalDiff = evalA - evalB;

        glm::vec3 evalAU = surfaceA.evaluateDU(u, v);
        glm::vec3 evalAV = surfaceA.evaluateDV(u, v);

        glm::vec3 evalBU = surfaceB.evaluateDU(s, t);
        glm::vec3 evalBV = surfaceB.evaluateDV(s, t);

        return {
                glm::dot(evalDiff, evalAU) * 2.0f,
                glm::dot(evalDiff, evalAV) * 2.0f,
                glm::dot(-evalDiff, evalBU) * 2.0f,
                glm::dot(-evalDiff, evalBV) * 2.0f
        };
    }

    bool cap(float &point, float lowerLimit, float upperLimit, bool wrap) {
        if(wrap) {
            while(point < lowerLimit) point += upperLimit - lowerLimit;
            while(point > upperLimit) point -= upperLimit - lowerLimit;
        } else {
            if(point < lowerLimit) {
                point = lowerLimit;
                return true;
            }
            if(point > upperLimit) {
                point = upperLimit;
                return true;
            }
        }
        return false;
    }

    template<ParametricSurface S1, ParametricSurface S2>
    std::expected<IntersectionResult, std::string> findIntersection(S1 &surfaceA, S2 &surfaceB, bool sameSurface, glm::vec3 cursor) {
        // if(sameSurface)
            // return std::unexpected<std::string>("Self-intersections not supported!");

        auto pointA = getClosestPoint(surfaceA, cursor, {});
        auto pointB = getClosestPoint(surfaceB, cursor, sameSurface ? std::optional(pointA):  std::nullopt);

        float step = gradientStartStep;
        float prevDist = glm::distance(surfaceA.evaluate(pointA.x, pointA.y), surfaceB.evaluate(pointB.x, pointB.y));

        std::vector<glm::vec3> intersectionPoints;
        std::vector<glm::vec2> intersectionUV1;
        std::vector<glm::vec2> intersectionUV2;



        for(int k = 0; k < gradientIterationLimit && prevDist > gradientPrecision; k++) {
            glm::vec4 gradient = getGradient(surfaceA, pointA.x, pointA.y, surfaceB, pointB.x, pointB.y);

            float newU = pointA.x - gradient.x * step;
            float newV = pointA.y - gradient.y * step;
            float newS = pointB.x - gradient.z * step;
            float newT = pointB.y - gradient.w * step;
            cap(newU, 0, surfaceA.rangeU(), surfaceA.wrapU());
            cap(newV, 0, surfaceA.rangeV(), surfaceA.wrapV());
            cap(newS, 0, surfaceB.rangeU(), surfaceB.wrapU());
            cap(newT, 0, surfaceB.rangeV(), surfaceB.wrapV());
            pointA.x = newU;
            pointA.y = newV;
            pointB.x = newS;
            pointB.y = newT;
            // float currDist = glm::distance(surfaceA.evaluate(pointA.x - gradient.x * step, pointA.y - gradient.y * step), surfaceB.evaluate(pointB.x - gradient.z * step, pointB.y - gradient.w * step));
            float currDist = glm::distance(surfaceA.evaluate(newU, newV), surfaceB.evaluate(newS, newT));
            if(currDist > prevDist) {
                std::cout<<"Smaller step"<<std::endl;
                step /= 2;
                prevDist = std::numeric_limits<float>::max();
                continue;
            }
            prevDist = currDist;

            // pointA.x -= gradient.x * step;
            // pointA.y -= gradient.y * step;
            // pointB.x -= gradient.z * step;
            // pointB.y -= gradient.w * step;
            // // std::cout<<"G:[" << gradient.x << " " << gradient.y << " " << gradient.z << " " << gradient.w << "]" << std::endl;
            // // std::cout<<"P:[" << pointA.x << " " << pointA.y << " " << pointB.x << " " << pointB.y << "]" << std::endl;
            // cap(pointA.x, 0, surfaceA.rangeU(), surfaceA.wrapU());
            // cap(pointA.y, 0, surfaceA.rangeV(), surfaceA.wrapV());
            // cap(pointB.x, 0, surfaceB.rangeU(), surfaceB.wrapU());
            // cap(pointB.y, 0, surfaceB.rangeV(), surfaceB.wrapV());
            // step = gradientStartStep;
            // intersectionPoints.push_back(surfaceA.evaluate(pointA.x, pointA.y)); // First point on the intersection
            // intersectionPoints.push_back(surfaceB.evaluate(pointB.x, pointB.y)); // First point on the intersection
        }
        // // TODO DEBUG
        // intersectionPoints.push_back(surfaceA.evaluate(pointA.x, pointA.y));
        // intersectionUV1.emplace_back(pointA.x, pointA.y);
        // intersectionPoints.push_back(surfaceA.evaluate(pointB.x, pointB.y));
        // intersectionUV2.emplace_back(pointB.x, pointB.y);
        // std::vector<std::pair<IntersectableSurface, std::vector<glm::vec2>>> vec1;
        // vec1.push_back(std::make_pair(std::ref(surfaceA), intersectionUV1));
        // vec1.push_back(std::make_pair(std::ref(surfaceB), intersectionUV2));
        // return IntersectionResult{
        //     intersectionPoints,
        //     vec1,
        //     false
        // };
        // // TODO DEBUG

        if(prevDist > gradientPrecision)
            return std::unexpected<std::string>("Couldn't find an intersection close to the cursor with desired precision");

        intersectionPoints.push_back(surfaceA.evaluate(pointA.x, pointA.y)); // First point on the intersection
        // intersectionPoints.push_back(surfaceB.evaluate(pointB.x, pointB.y)); // First point on the intersection
        intersectionUV1.emplace_back(pointA.x, pointA.y);
        intersectionUV2.emplace_back(pointB.x, pointB.y);

         float u = pointA.x, v = pointA.y, s = pointB.x, t = pointB.y;
         float startU = u, startV = v, startS = s, startT = t;

         glm::vec3 T = {};
         bool positiveDirection = true;
        bool wasCapped = false;
         for(int k = 0; k < maxIntersectionPoints; k++) {
             if(wasCapped) {
                 if(!positiveDirection) {
                     std::vector<std::pair<IntersectableSurface, std::vector<glm::vec2>>> vec;
                     vec.push_back(std::make_pair(std::ref(surfaceA), intersectionUV1));
                     vec.push_back(std::make_pair(std::ref(surfaceB), intersectionUV2));
                     return IntersectionResult{
                         intersectionPoints,
                         vec,
                         false
                     };
                 }
                 pointA.x = startU;
                 pointA.y = startV;
                 pointB.x = startS;
                 pointB.y = startT;
                 u = startU;
                 v = startV;
                 s = startS;
                 t = startT;
                 std::cout<<"Try from the other side" << std::endl;
                 positiveDirection = false;
                 T = {};
             }
             wasCapped = false;
             int stepTries = 0;
             float prevDist = std::numeric_limits<float>::max();
             float step = intersectionPointsDistance;
             glm::vec3 p0 = surfaceA.evaluate(u, v);

             glm::vec3 PdU = surfaceA.evaluateDU(u, v);
             glm::vec3 PdV = surfaceA.evaluateDV(u, v);
             glm::vec3 QdU = surfaceB.evaluateDU(s, t);
             glm::vec3 QdV = surfaceB.evaluateDV(s, t);

             glm::vec3 pN = glm::normalize(glm::cross(PdU, PdV));
             glm::vec3 qN = glm::normalize(glm::cross(QdU, QdV));

             auto newT = glm::normalize(glm::cross(pN, qN));
             newT = positiveDirection ? newT : -newT;
             // std::cout<< pN << ", " << qN << ", " << newT << " vs. " << T << std::endl;
             // if(glm::length(newT) < 0.001) {
             //     std::vector<std::pair<IntersectableSurface, std::vector<glm::vec2>>> vec;
             //     vec.push_back(std::make_pair(std::ref(surfaceA), intersectionUV1));
             //     vec.push_back(std::make_pair(std::ref(surfaceB), intersectionUV2));
             //     return IntersectionResult{
             //         intersectionPoints,
             //         vec
             //     };
             // }
             bool signChanged = glm::dot(T, newT) < -0.0 ;
             // std::cout << glm::dot(T, newT) << std::endl;
             T = newT;
             T *= signChanged? -1 : 1;
             for(int it = 0; it < intersectionIterations; it++) {
                 glm::vec3 p = surfaceA.evaluate(u, v);
                 glm::vec3 q = surfaceB.evaluate(s, t);

                 glm::vec3 PdU = surfaceA.evaluateDU(u, v);
                 glm::vec3 PdV = surfaceA.evaluateDV(u, v);
                 glm::vec3 QdU = surfaceB.evaluateDU(s, t);
                 glm::vec3 QdV = surfaceB.evaluateDV(s, t);

                 glm::vec4 f = glm::vec4(p - q, glm::dot(p - p0, T) - step);
                 float currDist = glm::length(f);
                 if(currDist < intersectionPrecision || it == intersectionIterations-1) {
                     if(it == intersectionIterations - 1)
                         return std::unexpected<std::string>("Reached intersection iteration limit!");
                     pointA.x = u;
                     pointA.y = v;
                     pointB.x = s;
                     pointB.y = t;
                     break;
                 }

                 if(
                     // TODO u < 0 || v < 0 || s < 0 || t < 0 || u > surfaceA.rangeU() || v > surfaceA.rangeV() || s > surfaceB.rangeU() || t > surfaceB.rangeV()
                     currDist > prevDist) {
                     if(stepTries == smallerStepTriesLimit) /*return std::unexpected<std::string>("Next intersection point Newton diverged");*/ {
                         // Try from the other side
                        // // if(positiveDirection) {
                        //     std::cout<<"Try from the other side" << std::endl;
                        //     positiveDirection = !positiveDirection;
                        //     stepTries = 0;
                            // pointA.x = startU;
                            // pointA.y = startV;
                            // pointB.x = startS;
                            // pointB.y = startT;
                            // u = startU;
                            // v = startV;
                            // s = startS;
                            // t = startT;
                            // continue;
                        // }

                         std::vector<std::pair<IntersectableSurface, std::vector<glm::vec2>>> vec;
                         vec.push_back(std::make_pair(std::ref(surfaceA), intersectionUV1));
                         vec.push_back(std::make_pair(std::ref(surfaceB), intersectionUV2));
                         return IntersectionResult{
                             intersectionPoints,
                             vec,
                             false
                         };
                     }
                     stepTries++;
                     std::cout<< "Smaller step" << std::endl;
                     step /= 2;
                     prevDist = std::numeric_limits<float>::max();
                     u = pointA.x;
                     v = pointA.y;
                     s = pointB.x;
                     t = pointB.y;

                     wasCapped |= cap(u, 0, surfaceA.rangeU(), surfaceA.wrapU());
                     wasCapped |= cap(v, 0, surfaceA.rangeV(), surfaceA.wrapV());
                     wasCapped |= cap(s, 0, surfaceB.rangeU(), surfaceB.wrapU());
                     wasCapped |= cap(t, 0, surfaceB.rangeV(), surfaceB.wrapV());
                     it = 0;
                     continue;
                 }
                 prevDist = currDist;

                 glm::vec4 FdPu = glm::vec4(PdU, glm::dot(PdU, T));
                 glm::vec4 FdPv = glm::vec4(PdV, glm::dot(PdV, T));
                 glm::vec4 FdQu = glm::vec4(-QdU, 0);
                 glm::vec4 FdQv = glm::vec4(-QdV, 0);

                 glm::mat4 m = glm::mat4(FdPu, FdPv, FdQu, FdQv);
                 m = glm::inverse(m);

                 glm::vec4 x = {u, v, s, t};
                 glm::vec4 dx = m * f;
                 dx.x /= surfaceA.rangeU();
                 dx.y /= surfaceA.rangeV();
                 dx.z /= surfaceB.rangeU();
                 dx.w /= surfaceB.rangeV();
                 x = x - dx;

                 u = x.x;
                 v = x.y;
                 s = x.z;
                 t = x.w;

                 wasCapped |= cap(u, 0, surfaceA.rangeU(), surfaceA.wrapU());
                 wasCapped |= cap(v, 0, surfaceA.rangeV(), surfaceA.wrapV());
                 wasCapped |= cap(s, 0, surfaceB.rangeU(), surfaceB.wrapU());
                 wasCapped |= cap(t, 0, surfaceB.rangeV(), surfaceB.wrapV());
                 if(wasCapped || glm::length(dx) < intersectionPrecision)
                     break;
             }
             // if(positiveDirection) {
                 intersectionPoints.push_back(surfaceA.evaluate(u, v));
                 intersectionUV1.emplace_back(u, v);
                 intersectionUV2.emplace_back(s, t);
             // } else {
                 // intersectionPoints.insert(intersectionPoints.begin(), surfaceA.evaluate(u, v));
                 // intersectionUV1.insert(intersectionUV1.begin(), {u, v});
                 // intersectionUV2.insert(intersectionUV1.begin(), {s, t});
             // }

             // intersectionPoints.push_back(surfaceB.evaluate(s, t));

             if(intersectionPoints.size() > 2 && glm::length(intersectionPoints.front() - intersectionPoints.back()) < step) {
                 if(!positiveDirection) continue;
                 break;
             }
             // if(intersectionPoints.size() > 2) {
             //     bool breakFlag = false;
             //     for(int i = 0; !breakFlag && i < intersectionPoints.size()-2; i++)
             //         if(glm::length(intersectionPoints.back() - intersectionPoints[i]) < step)
             //             breakFlag = true;
             //     if(breakFlag) break;
             // }
         }

        // TODO DEBUG
         // if(intersectionPoints.size() == maxIntersectionPoints) return std::unexpected<std::string>("Too many intersection points");
        std::vector<std::pair<IntersectableSurface, std::vector<glm::vec2>>> vec;
        vec.push_back(std::make_pair(std::ref(surfaceA), intersectionUV1));
        vec.push_back(std::make_pair(std::ref(surfaceB), intersectionUV2));
        return IntersectionResult{
            intersectionPoints,
            vec,
            true
        };
    }

    void addToMask(IntersectableSurface &surface, Intersection &intersection, int i) {
        auto m = std::visit([&](auto &e) { return e.get().getMask(); }, surface);
        auto &mask = m.second.get();
        auto &uvs = intersection.surfaces[i].second;
        auto lambda = [&](int x, int y, float) {
            if(x >= 0 && y >= 0 && x < mask.size() && y < mask[0].size())
                mask[x][y] = intersection.color;
        };
        for(int i = 0; i < uvs.size(); i++) {
            auto p1 = uvs[i];
            auto p2 = uvs[(i + 1)%uvs.size()];
            std::visit([&](auto &s) {
                if( std::fabs(p2.x - p1.x) < s.get().rangeU()/2 && std::fabs(p2.y - p1.y) < s.get().rangeV()/2) {
                    p1.x *= mask.size() / s.get().rangeU();
                   p2.x *= mask.size() / s.get().rangeU();
                   p1.y *= mask[0].size() / s.get().rangeV();
                   p2.y *= mask[0].size() / s.get().rangeV();
                   bresenhamAlgorithm(p1.x, p1.y, p2.x, p2.y, lambda);
                } else if(intersection.wrapped) {
                    glm::vec2 p1Prim = p1;
                    glm::vec2 p2Prim = p2;
                    glm::vec2 p1Bis = p1;
                    glm::vec2 p2Bis = p2;
                    if(std::fabs(p2.x - p1.x) > s.get().rangeU()/2) {
                        if(p1.x < p2.x) {
                            p1Prim.x += s.get().rangeU();
                            p2Bis.x -= s.get().rangeU();
                        } else {
                            p2Prim.x += s.get().rangeU();
                            p1Bis.x -= s.get().rangeU();
                        }
                    }
                    if(std::fabs(p2.y - p1.y) > s.get().rangeV()/2) {
                        if(p1.y < p2.y) {
                            p1Prim.y += s.get().rangeV();
                            p2Bis.y -= s.get().rangeV();
                        } else {
                            p2Prim.y += s.get().rangeV();
                            p1Bis.y -= s.get().rangeV();
                        }
                    }

                    p1Prim.x *= mask.size() / s.get().rangeU();
                    p1Prim.y *= mask[0].size() / s.get().rangeV();
                    p2Prim.x *= mask.size() / s.get().rangeU();
                    p2Prim.y *= mask[0].size() / s.get().rangeV();
                    p1Bis.x *= mask.size() / s.get().rangeU();
                    p1Bis.y *= mask[0].size() / s.get().rangeV();
                    p2Bis.x *= mask.size() / s.get().rangeU();
                    p2Bis.y *= mask[0].size() / s.get().rangeV();
                    bresenhamAlgorithm(p1Prim.x, p1Prim.y, p2Prim.x, p2Prim.y, lambda);
                    bresenhamAlgorithm(p1Bis.x, p1Bis.y, p2Bis.x, p2Bis.y, lambda);
               }
            }, surface);

        }
        std::visit([&](auto &s) {
            s.get().setMask(mask);
        }, surface);
    }
};


#endif //OPENGL_SANDBOX_SURFACEINTERSECTION_H
