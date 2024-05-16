//
// Created by faliszewskii on 12.05.24.
//

#include "Serializer.h"
#include "Scene/Scene.h"
#include "Scene/SceneSerializer.h"

void Serializer::exportScene(AppState &appState, const std::string &path) {
    unsigned int id = 1;
    auto &scene = MG1::Scene::Get();
    scene.Clear();

    std::map<int, unsigned int> idMap; // maps my ids to json ids.

    for(auto &point : appState.pointSet) {
        MG1::Point pointData;
        idMap.emplace(point.first, id);
        pointData.SetId(id++);
        pointData.name = point.second->name;
        pointData.position.x = point.second->position.x;
        pointData.position.y = point.second->position.y;
        pointData.position.z = point.second->position.z;
        scene.points.push_back(pointData);
    }

    for(auto &torus : appState.torusSet) {
        MG1::Torus torusData;

        torusData.SetId(id++);
        torusData.name = torus.second->name;
        torusData.position.x = torus.second->transform.translation.x;
        torusData.position.y = torus.second->transform.translation.y;
        torusData.position.z = torus.second->transform.translation.z;
        auto rot = torus.second->transform.getRotationAngles();
        torusData.rotation.x = rot.x;
        torusData.rotation.y = rot.y;
        torusData.rotation.z = rot.z;
        torusData.scale.x = torus.second->transform.scale.x;
        torusData.scale.y = torus.second->transform.scale.y;
        torusData.scale.z = torus.second->transform.scale.z;
        torusData.samples.x = torus.second->radialResolution;
        torusData.samples.y = torus.second->tubularResolution;
        torusData.smallRadius = torus.second->thickness;
        torusData.largeRadius = torus.second->radius;

        scene.tori.push_back(torusData);
    }

    for(auto &bezier : appState.bezierC0Set) {
        MG1::BezierC0 bezierData;

        bezierData.SetId(id++);
        bezierData.name = bezier.second->name;
        for(auto &point : bezier.second->controlPoints) {
            bezierData.controlPoints.emplace_back(idMap[point.first]);
        }
        scene.bezierC0.push_back(bezierData);
    }

    for(auto &bezier : appState.bezierC2Set) {
        MG1::BezierC2 bezierData;

        bezierData.SetId(id++);
        bezierData.name = bezier.second->name;
        for(auto &point : bezier.second->controlPoints) {
            bezierData.controlPoints.emplace_back(idMap[point.first]);
        }
        scene.bezierC2.push_back(bezierData);
    }

    for(auto &interpolated : appState.interpolatedC2Set) {
        MG1::InterpolatedC2 interpolatedData;

        interpolatedData.SetId(id++);
        interpolatedData.name = interpolated.second->name;
        for(auto &point : interpolated.second->controlPoints) {
            interpolatedData.controlPoints.emplace_back(idMap[point.first]);
        }
        scene.interpolatedC2.push_back(interpolatedData);
    }

    for(auto &patch : appState.patchC0Set) {
        MG1::BezierSurfaceC0 surface;
        surface.name = patch.second->name;
        auto indices = patch.second->mesh.getIndices().value();
        int n = indices.size() / 16;
        for(int i=0; i< n; i++) {
            MG1::BezierPatchC0 patchData;
            patchData.SetId(id++);
            for(auto &point : patch.second->controlPoints) {
                patchData.controlPoints.emplace_back(idMap[point.first]);
            }
            surface.patches.push_back(patchData);
        }
        scene.surfacesC0.push_back(surface);
    }

    for(auto &patch : appState.patchC2Set) {
        MG1::BezierSurfaceC2 surface;
        surface.name = patch.second->name;
        auto indices = patch.second->mesh.getIndices().value();
        int n = indices.size() / 16;
        for(int i=0; i< n; i++) {
            MG1::BezierPatchC2 patchData;
            patchData.SetId(id++);
            for(auto &point : patch.second->controlPoints) {
                patchData.controlPoints.emplace_back(idMap[point.first]);
            }
            surface.patches.push_back(patchData);
        }
        scene.surfacesC2.push_back(surface);
    }

    MG1::SceneSerializer serializer;
    serializer.SaveScene(path);
}

void Serializer::importScene(AppState &appState, const std::string &path) {
    MG1::SceneSerializer serializer;
    serializer.LoadScene(path);

    auto& scene = MG1::Scene::Get();

    std::map<int, int> idMap; // maps json ids to my ids.
    for(auto &point : scene.points) {
        appState.eventPublisher.publish(CreatePointEvent{{point.position.x, point.position.y, point.position.z}});
        appState.pointSet[appState.lastIdCreated]->name = point.name;
        idMap.emplace(point.GetId(), appState.lastIdCreated);
    }

    for(auto &torus : scene.tori) {
        appState.eventPublisher.publish(CreateTorusEvent{{torus.position.x, torus.position.y, torus.position.z}});
        auto &t = *appState.torusSet[appState.lastIdCreated];
        t.name = torus.name;
        t.transform.setRotation({torus.rotation.x, torus.rotation.y, torus.rotation.z});
        t.transform.setScale({torus.scale.x, torus.scale.y, torus.scale.z});
        t.radius = torus.largeRadius;
        t.thickness = torus.smallRadius;
        t.radialResolution = torus.samples.x;
        t.tubularResolution = torus.samples.y;
        t.generate();
    }

    for(auto &bezierData : scene.bezierC0) {
        appState.eventPublisher.publish(CreateBezierC0Event{});
        auto &bezier = *appState.bezierC0Set[appState.lastIdCreated];
        bezier.name = bezierData.name;
        for(auto &point : bezierData.controlPoints) {
            int id = idMap[point.GetId()];
            bezier.addPoint(*appState.pointSet[id]);
        }
    }

    for(auto &bezierData : scene.bezierC2) {
        appState.eventPublisher.publish(CreateBezierC2Event{});
        auto &bezier = *appState.bezierC2Set[appState.lastIdCreated];
        bezier.name = bezierData.name;
        for(auto &point : bezierData.controlPoints) {
            int id = idMap[point.GetId()];
            bezier.addPoint(*appState.pointSet[id]);
        }
    }

    for(auto &bezierData : scene.interpolatedC2) {
        appState.eventPublisher.publish(CreateInterpolatedC2Event{});
        auto &bezier = *appState.interpolatedC2Set[appState.lastIdCreated];
        bezier.name = bezierData.name;
        for(auto &point : bezierData.controlPoints) {
            int id = idMap[point.GetId()];
            bezier.addPoint(*appState.pointSet[id]);
        }
    }

    for(auto &surfaceData : scene.surfacesC0) {
        for(auto &patchData : surfaceData.patches) {
            appState.bezierPatchCreator.reset();
            appState.bezierPatchCreator.update(BezierPatchCreatorParams{
                1,
                1,
                0,
                0,
                0,
                0,
                false,
                false,
                false
            });
            std::vector<int> points;
            for(auto &point : patchData.controlPoints) {
                int id = idMap[point.GetId()];
                points.push_back(id);
            }
            appState.eventPublisher.publish(CreateBezierPatch{
                    appState.bezierPatchCreator.getPatchIndices(),
                    appState.bezierPatchCreator.getGridIndices(),
                    points,
                    appState.bezierPatchCreator.getParams().C2});
        }
    }

    for(auto &surfaceData : scene.surfacesC2) {
        for(auto &patchData : surfaceData.patches) {
            appState.bezierPatchCreator.reset();
            appState.bezierPatchCreator.update(BezierPatchCreatorParams{
                    1,
                    1,
                    0,
                    0,
                    0,
                    0,
                    false,
                    true,
                    false
            });
            std::vector<int> points;
            for(auto &point : patchData.controlPoints) {
                int id = idMap[point.GetId()];
                points.push_back(id);
            }
            appState.eventPublisher.publish(CreateBezierPatch{
                    appState.bezierPatchCreator.getPatchIndices(),
                    appState.bezierPatchCreator.getGridIndices(),
                    points,
                    appState.bezierPatchCreator.getParams().C2});
        }
    }

}
