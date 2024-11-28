//
// Created by USER on 03/09/2024.
//

#include "IntersectionGui.h"



bool IntersectionGui::isIntersectionEnabled(const std::vector<std::pair<int, EntityType>>& selected) {
    if( selected.empty() || selected.size() > 2)
        return false;
    if(!std::visit([](auto& a) {
        return ParametricSurface<decltype(a.get())> && Intersectable<decltype(a.get())>;
    }, selected[0].second))
        return false;
    if(selected.size() == 2 && !std::visit([](auto& b) {
        return ParametricSurface<decltype(b.get())> && Intersectable<decltype(b.get())>;
    }, selected[1].second))
        return false;
    return true;
}

std::optional<IntersectableSurface> IntersectionGui::getIfIntersectableSurface(EntityType entity) {
    return std::visit([](auto& a) -> std::optional<IntersectableSurface> {
        using T = std::decay_t<decltype(a.get())>;
        if constexpr (ParametricSurface<T> && Intersectable<T>)
            return a;
        return {};
    }, entity);
}

void IntersectionGui::render() {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize;

    bool modified = false;

    if (ImGui::Begin("Intersections GUI", &appState.intersectionGuiOpen, window_flags)) {

        ImGui::SeparatorText("Closest point to cursor search");
        ImGui::SliderInt("Subdivision count", &appState.surfaceIntersection.subdivisionCount, 2, 128);
        ImGui::SliderInt("Subdivision iterations", &appState.surfaceIntersection.subdivisionIterations, 10, 1000);
        ImGui::DragFloat("Same surface repulsion exponent", &appState.surfaceIntersection.repulserExponent, 0.001, 0.01, 2);


        ImGui::SeparatorText("First intersection point search");
        ImGui::SliderInt("Gradient iteration limit", &appState.surfaceIntersection.gradientIterationLimit, 10, 4000);
        ImGui::DragFloat("Gradient start step", &appState.surfaceIntersection.gradientStartStep, 0.001, 0.005, 0.1);
        ImGui::SliderInt("Gradient precision epsilon", &appState.surfaceIntersection.gradientPrecisionEpsilon, 10, 10000);

        ImGui::SeparatorText("Intersection points search");
        ImGui::DragFloat("Intersection points distance", &appState.surfaceIntersection.intersectionPointsDistance, 0.001, 0.01, 1);
        ImGui::SliderInt("Max intersection points", &appState.surfaceIntersection.maxIntersectionPoints, 100, 10000);
        ImGui::SliderInt("Newton iterations limit", &appState.surfaceIntersection.intersectionIterations, 100, 10000);
        ImGui::SliderInt("Newton precision epsilon", &appState.surfaceIntersection.intersectionPrecisionEpsilon, 10, 10000);
        ImGui::SliderInt("Newton smaller step tries", &appState.surfaceIntersection.smallerStepTriesLimit, 1, 100);
        ImGui::DragFloat("Newton minimum tangent dot product", &appState.surfaceIntersection.intersectionTangentDotProduct, 0.01, -1, 1);



        if(ImGui::Button("Set Defaults")) {
            appState.surfaceIntersection.subdivisionCount = appState.surfaceIntersection.subdivisionCountDefault;
            appState.surfaceIntersection.subdivisionIterations = appState.surfaceIntersection.subdivisionIterationsDefault;
            appState.surfaceIntersection.gradientIterationLimit = appState.surfaceIntersection.gradientIterationLimitDefault;
            appState.surfaceIntersection.gradientStartStep = appState.surfaceIntersection.gradientStartStepDefault;
            appState.surfaceIntersection.gradientPrecisionEpsilon = appState.surfaceIntersection.gradientPrecisionEpsilonDefault;
            appState.surfaceIntersection.intersectionPointsDistance = appState.surfaceIntersection.intersectionPointsDistanceDefault;
            appState.surfaceIntersection.intersectionIterations = appState.surfaceIntersection.intersectionIterationsDefault;
            appState.surfaceIntersection.intersectionPrecisionEpsilon = appState.surfaceIntersection.intersectionPrecisionEpsilonDefault;
            appState.surfaceIntersection.maxIntersectionPoints = appState.surfaceIntersection.maxIntersectionPointsDefault;
            appState.surfaceIntersection.smallerStepTriesLimit = appState.surfaceIntersection.smallerStepTriesLimitDefault;
            appState.surfaceIntersection.intersectionTangentDotProduct = appState.surfaceIntersection.intersectionTangentDotProductDefault;
            appState.surfaceIntersection.repulserExponent = appState.surfaceIntersection.repulserExponentDefault;
        }
        ImGui::SameLine();
        ImGui::BeginDisabled(!isIntersectionEnabled(appState.selectedEntities));
        if(ImGui::Button("Find Intersection")) {
            auto& [idA, entityA] = appState.selectedEntities[0];
            auto& [idB, entityB] = appState.selectedEntities[1 % appState.selectedEntities.size()];
            auto sA = getIfIntersectableSurface(entityA).value();
            auto sB = getIfIntersectableSurface(entityB).value();
            std::visit([&](auto& a, auto& b) {
                auto result = appState.surfaceIntersection.findIntersection(a.get(), b.get(), idA == idB, appState.cursorPosition);
                if(!result.has_value()) {
                    appState.logger.logError(result.error());
                    return;
                }
                auto points = result->intersectionPoints;
//                for(int i = 0; i < points.size(); i++) {
//                    glm::vec3 &p = points[i];
//                    glm::vec2 uv = result->surfaces[1].second[i];
//                    glm::vec3 t1 = b.get().evaluateDU(uv.x, uv.y);
//                    glm::vec3 t2 = b.get().evaluateDV(uv.x, uv.y);
//                    glm::vec3 n = glm::cross(t1, t2);
////                    n = glm::vec3(n.x, 0, n.z);
//                    n = glm::normalize(n);
//                    float radius = 4;
//                    n *= glm::vec3(radius/5.7243f,radius/6.57439232f,radius/5.235f);
//
//                    p += n;
//                }
                appState.eventPublisher.publish(CreateIntersectionEvent(
                    points,
                    result->surfaces,
                    result->wrapped
                    ));
                // TODO DEBUG
                appState.surfaceIntersection.addToMask(sA, *appState.intersectionSet[appState.lastIdCreated], 0);
                appState.surfaceIntersection.addToMask(sB, *appState.intersectionSet[appState.lastIdCreated], 1);
            }, sA, sB);
        }
        ImGui::EndDisabled();
        ImGui::End();
    }
}