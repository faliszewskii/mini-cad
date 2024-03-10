//
// Created by faliszewskii on 03.03.24.
//

#ifndef OPENGL_SANDBOX_ENTITYLISTWORKSPACE_H
#define OPENGL_SANDBOX_ENTITYLISTWORKSPACE_H

#include "../../../logic/state/AppState.h"
#include "../../../logic/concepts/has_name.h"
#include "../../../logic/events/SelectEntityEvent.h"
#include "../../../logic/events/CreateBezierC0Event.h"
#include "../../../logic/events/PointMovedEvent.h"

namespace EntityListWorkspace {

    void renderWorkspaceTorus(Torus &torus);
    void renderWorkspaceTransform(Transformation &transform);
    void renderWorkspacePoint(Point &point, AppState &appState);
    void renderWorkspaceBezierC0(BezierC0 &bezier, AppState &appState);
    void renderWorkspaceMultiple(std::map<int, EntityType> &selected, AppState &appState);

    template<typename T> requires has_name<T>
    void renderNameInput(T &el);
    template<typename T> requires has_name<T> && has_id<T>
    void renderListing(std::map<int, std::unique_ptr<T>> &list, AppState &appState);
    void renderWorkspaceSelected(AppState &appState);

    void renderDeleteButton(AppState &appState);

    inline void render(AppState &appState) {
        if (ImGui::Button("Add Torus"))
            appState.eventPublisher.publish(CreateTorusEvent{appState.cursorPosition});
        ImGui::SameLine();
        if (ImGui::Button("Add Point"))
            appState.eventPublisher.publish(CreatePointEvent{appState.cursorPosition});
        ImGui::SameLine();
        if (ImGui::Button("Add Bezier C0"))
            appState.eventPublisher.publish(CreateBezierC0Event{});

        renderDeleteButton(appState);

        if(ImGui::CollapsingHeader("Scene objects", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::BeginListBox("Objects#Workspace", ImVec2(-FLT_MIN, 0))) {
                renderListing(appState.torusSet, appState);
                renderListing(appState.pointSet, appState);
                renderListing(appState.bezierC0Set, appState);
                ImGui::EndListBox();
            }
        }
        renderWorkspaceSelected(appState);
    }

    inline void renderDeleteButton(AppState &appState) {
        ImGui::BeginDisabled(appState.selectedEntities.empty());
        if (ImGui::Button("Delete")) {
            for(auto &el : appState.selectedEntities) {
                std::visit(overloaded{
                       [&](Torus &torus) { appState.torusSet.erase(appState.torusSet.find(torus.id)); },
                       [&](Point &point) {
                           int id = point.id;
                           appState.pointSet.erase(appState.pointSet.find(id));
                           appState.eventPublisher.publish(PointDeletedEvent{id});
                       },
                       [&](BezierC0 &bezier) { appState.bezierC0Set.erase(appState.bezierC0Set.find(bezier.id)); }
                    }, el.second);
            }
            appState.selectedEntities.clear(); // TODO Move to events.
        }
        ImGui::EndDisabled();
    }

    inline void renderWorkspaceSelected(AppState &appState) {
        if(ImGui::BeginChild("##SelectedEntities")) {
            auto &selected = appState.selectedEntities;
            if (selected.empty())
                ImGui::SeparatorText("Select entities");
            else if (selected.size() == 1) {
                std::visit(overloaded{
                        [](Torus &torus) { renderWorkspaceTorus(torus); },
                        [&](Point &point) { renderWorkspacePoint(point, appState); },
                        [&](BezierC0 &bezier) { renderWorkspaceBezierC0(bezier, appState); }
                }, selected.begin()->second);
            } else {
                renderWorkspaceMultiple(selected, appState);
            }
            ImGui::EndChild();
        }
    }

    inline void renderWorkspaceBezierC0(BezierC0 &bezier, AppState &appState) {
        ImGui::SeparatorText("Bezier C0");
        ImGui::Text("Segment count: %d", bezier.segmentCount);
        ImGui::DragInt("segments", &bezier.segmentCount);
        ImGui::Checkbox("Draw Polyline", &bezier.drawPolyline);
        ImGui::SeparatorText("Control Points");
        if (ImGui::BeginListBox("Control points#Workspace", ImVec2(-FLT_MIN, 0))) {
            for(auto &pPoint : bezier.controlPoints) {
                Point &point = pPoint.second;
                if (ImGui::Selectable((point.name + "##" + std::to_string(point.id)).c_str(), appState.selectedEntities.contains(point.id))) {
                    appState.eventPublisher.publish(SelectEntityEvent{point});
                }
            }
            ImGui::EndListBox();
        }
    }

    inline void renderWorkspaceMultiple(std::map<int, EntityType> &selected, AppState &appState) {
        ImGui::SeparatorText(("Selected " + std::to_string(selected.size()) + " entities").c_str());
        bool modified = false;
        auto &centerTransform = appState.centerOfMassTransformation;

        // Because first element is Bézier curve we can add following points to it
        if(std::holds_alternative<std::reference_wrapper<BezierC0>>(appState.selectedEntities.begin()->second)) {
            if(ImGui::Button("Add points to curve")) {
                BezierC0 &bezier = std::get<std::reference_wrapper<BezierC0>>(appState.selectedEntities.begin()->second);
                for(auto &el : appState.selectedEntities)
                    if(std::holds_alternative<std::reference_wrapper<Point>>(el.second))
                        bezier.addPoint(std::get<std::reference_wrapper<Point>>(el.second));
            }
        }

        auto position = glm::vec<3,double>(centerTransform.getTranslationRef());
        ImGui::Text("Position:");
        modified |= ImGui::DragScalar("x##position", ImGuiDataType_Double, glm::value_ptr(position) + 0, 0.01f);
        modified |= ImGui::DragScalar("y##position", ImGuiDataType_Double, glm::value_ptr(position) + 1, 0.01f);
        modified |= ImGui::DragScalar("z##position", ImGuiDataType_Double, glm::value_ptr(position) + 2, 0.01f);

        auto angle = glm::vec<3, double>(centerTransform.getRotationAngles());
        ImGui::Text("Rotation:");
        modified |= ImGui::DragScalar("x##orientation", ImGuiDataType_Double, glm::value_ptr(angle) + 0, 0.01f);
        modified |= ImGui::DragScalar("y##orientation", ImGuiDataType_Double, glm::value_ptr(angle) + 1, 0.01f);
        modified |= ImGui::DragScalar("z##orientation", ImGuiDataType_Double, glm::value_ptr(angle) + 2, 0.01f);

        auto scale = glm::vec<3,double>(centerTransform.getScaleRef());
        ImGui::Text("Scale:");
        modified |= ImGui::DragScalar("x##scale", ImGuiDataType_Double, glm::value_ptr(scale) + 0, 0.001f);
        modified |= ImGui::DragScalar("y##scale", ImGuiDataType_Double, glm::value_ptr(scale) + 1, 0.001f);
        modified |= ImGui::DragScalar("z##scale", ImGuiDataType_Double, glm::value_ptr(scale) + 2, 0.001f);

        // TODO Zastanowić się nad reprezentacją kątów. Jako 3 macierze? Kwaternion ok? Co z kolejnością obrotów? Globalnie czy lokalnie? Zainspirować się ImGuizmo
        if(modified) { // TODO Maybe into an event ????
            auto translationDiff = position - centerTransform.getTranslationRef();
            auto angleDiff = angle - centerTransform.getRotationAngles();
            auto scaleRatio = scale / centerTransform.getScaleRef();
            auto T = glm::translate(glm::mat<4,4,double>{1.0f}, translationDiff) * glm::mat4_cast(glm::qua<double>(angleDiff)) * glm::scale(glm::mat<4,4,double>(1.0f), scaleRatio);
            centerTransform.setTransformation(T * centerTransform.getTransformation());

            centerTransform.setTranslation(position);
            centerTransform.setRotation(angle);
            centerTransform.setScale(scale);

            for(auto &el : appState.selectedEntities) {
                std::visit(overloaded{
                        [&](Torus &torus) {
                            torus.transform.setTranslation(torus.transform.getTranslationRef() - centerTransform.translation);
                            torus.transform.setTransformation(T * torus.transform.getTransformation());
                            torus.transform.setTranslation(torus.transform.getTranslationRef() + centerTransform.translation);
                        },
                        [&](Point &point) {
                            point.position -= centerTransform.translation;
                            point.position = T * glm::vec4(point.position, 1);
                            point.position += centerTransform.translation;
                            appState.eventPublisher.publish(PointMovedEvent{point});
                        },
                        [&](BezierC0 &bezier) {
                            /*TODO*/
                            // Czy zaznaczenie Beziera oznacza podczas transformacji, że każdy punkt obracamy?
                            // Może trzeba będzie rozdzielić selected entities na klasę gdzie każdy obiekt jest oddzielnie.
                        }
                }, el.second);
            }
        }
    }

    inline void renderWorkspaceTorus(Torus &torus) {
        ImGui::SeparatorText(torus.name.c_str());
        renderNameInput(torus);

        renderWorkspaceTransform(torus.transform);

        ImGui::SeparatorText("Parameters");
                for(auto &parameter : torus.getParameters()) {
            std::visit(overloaded{
                    [&parameter](int& value) { ImGui::DragInt(parameter.first.c_str(), &value); },
                    [&parameter](float& value) { ImGui::DragFloat(parameter.first.c_str(), &value, 0.01); }
            }, parameter.second);
            if(ImGui::IsItemActive()) torus.generate();
        }
    }

    template<typename T> requires has_name<T>
    void renderNameInput(T &el) {
        const static int bufSize = 1024;
        static char nameBuffer[bufSize] = "\0";
        el.name.copy(nameBuffer, el.name.size());
        nameBuffer[el.name.size()] = '\0';
        if (ImGui::InputText("Name", nameBuffer, bufSize))
            el.name = std::string(nameBuffer);
    }

    inline void renderWorkspacePoint(Point &point, AppState &appState) {
        ImGui::SeparatorText(point.name.c_str());
        renderNameInput(point);

        ImGui::Text("Position:");
        bool pointMoved = false;
        pointMoved |= ImGui::DragFloat("x##position", static_cast<float *>(glm::value_ptr(point.position)) + 0, 0.01f);
        pointMoved |= ImGui::DragFloat("y##position", static_cast<float *>(glm::value_ptr(point.position)) + 1, 0.01f);
        pointMoved |= ImGui::DragFloat("z##position", static_cast<float *>(glm::value_ptr(point.position)) + 2, 0.01f);

        // TODO Maybe changes in point position also should be done through events?
        if(pointMoved) appState.eventPublisher.publish(PointMovedEvent{point});
    }

    inline void renderWorkspaceTransform(Transformation &transform) {
        ImGui::SeparatorText("Transform");

        auto position = static_cast<double *>(glm::value_ptr(transform.getTranslationRef()));
        ImGui::Text("Position:");
        ImGui::DragScalar("x##position", ImGuiDataType_Double, position + 0, 0.01f);
        ImGui::DragScalar("y##position", ImGuiDataType_Double, position + 1, 0.01f);
        ImGui::DragScalar("z##position", ImGuiDataType_Double, position + 2, 0.01f);

        auto oldAngle = transform.getRotationAngles();
        auto newAngle = glm::vec<3,double>(oldAngle);
        auto angleRef = static_cast<double *>(glm::value_ptr(newAngle));
        ImGui::Text("Rotation:");
        if(ImGui::DragScalar("x##orientation", ImGuiDataType_Double, angleRef + 0, 0.01f))
            transform.addRotation(glm::vec3(newAngle.x - oldAngle.x, 0, 0));
        if(ImGui::DragScalar("y##orientation", ImGuiDataType_Double, angleRef + 1, 0.01f))
            transform.addRotation(glm::vec3(0, newAngle.y - oldAngle.y, 0));
        if(ImGui::DragScalar("z##orientation", ImGuiDataType_Double, angleRef + 2, 0.01f))
            transform.addRotation(glm::vec3(0, 0, newAngle.z - oldAngle.z));

        auto scale = static_cast<double *>(glm::value_ptr(transform.getScaleRef()));
        ImGui::Text("Scale:");
        ImGui::DragScalar("x##scale", ImGuiDataType_Double, scale + 0, 0.001f);
        ImGui::DragScalar("y##scale", ImGuiDataType_Double, scale + 1, 0.001f);
        ImGui::DragScalar("z##scale", ImGuiDataType_Double, scale + 2, 0.001f);
    }

    template<typename T> requires has_name<T> && has_id<T>
    inline void renderListing(std::map<int, std::unique_ptr<T>> &list, AppState &appState) {
        for (auto &el: std::views::values(list)) {
            if (ImGui::Selectable((el->name + "##" + std::to_string(el->id)).c_str(), appState.selectedEntities.contains(el->id))) {
                appState.eventPublisher.publish(SelectEntityEvent{*el});
            }
        }
    }
}
#endif //OPENGL_SANDBOX_ENTITYLISTWORKSPACE_H
