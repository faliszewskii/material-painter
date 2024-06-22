//
// Created by faliszewskii on 16.06.24.
//

#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <ranges>
#include <nfd.h>
#include "Gui.h"
#include "imgui.h"
#include "../import/ModelImporter.h"

Gui::Gui(AppContext &appContext) : appContext(appContext) {}

void Gui::render() {
    renderLogUI();
    ImGui::Begin("Sceme");
    // TODO --- ImGUI User Interface goes here.
//    renderLightUI(*appContext.light);
    if(ImGui::Button("LSCM")) {
        TAGen::Mesh mesh;
        std::ranges::transform(appContext.cylinder->vertices, std::back_inserter(mesh.vertices), [&](auto &v) {
            return Eigen::Vector3f{v.position.x, v.position.y, v.position.z};
        });
        mesh.indices = appContext.cylinder->indices;
        auto t = appContext.lscm->getFullTriangulation(mesh, *appContext.logger);
        auto uv = appContext.lscm->parametrizeChart(t, *appContext.logger);
        if(uv) {
            std::vector<Eigen::Vector2f> normalisedUv;
            float maxU = std::max_element(uv->uv.begin(), uv->uv.end(), [&](auto &el1, auto &el2){return el1.x() < el2.x();})->x();
            float maxV = std::max_element(uv->uv.begin(), uv->uv.end(), [&](auto &el1, auto &el2){return el1.y() < el2.y();})->y();
            float maxUV = std::max(maxU, maxV);
            float minU = std::min_element(uv->uv.begin(), uv->uv.end(), [&](auto &el1, auto &el2){return el1.x() < el2.x();})->x();
            float minV = std::min_element(uv->uv.begin(), uv->uv.end(), [&](auto &el1, auto &el2){return el1.y() < el2.y();})->y();
            float minUV = std::min(minU, minV);

            std::ranges::transform(uv->uv, std::back_inserter(normalisedUv), [&](Eigen::Vector2f &el) {
                float diff = maxUV - minUV;
                return Eigen::Vector2f(((el - Eigen::Vector2f(minUV, minUV)) / diff));
            });

            std::vector<PositionVertex> newVertices;
            std::ranges::transform(normalisedUv, std::back_inserter(newVertices), [&](auto &v) {
                return PositionVertex{{v.x(), v.y(), 2}};
            });

            std::vector<PosNorTexVertex> newCylinder;
            for(int i = 0; i < appContext.cylinder->vertices.size(); i++) {
                PosNorTexVertex curr = appContext.cylinder->vertices[i];
                newCylinder.push_back(PosNorTexVertex{curr.position, curr.normal, {normalisedUv[i].x(), normalisedUv[i].y()}});
            }
//            appContext.model->materials.push_back(std::make_unique<Material>(glm::vec4(1.0f), *appContext.testTexture, 256.f));
//            appContext.cylinder->material = *appContext.model->materials[1];

            appContext.cylinder->mesh->update(std::move(newCylinder), appContext.cylinder->indices);
            appContext.uvMapMesh->update(std::move(newVertices), appContext.cylinder->indices);
        } else {
            appContext.logger->logError(uv.error());
        }

    }
    ImGui::End();

    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Import")) {
                renderMenuItemLoadModel();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Gui::renderMenuItemLoadModel() {
            NFD_Init();

            nfdchar_t *outPath;
            nfdfilteritem_t filterItem[1]{{"3D models", "gltf,fbx,FBX,obj"}};
            nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
            if (result == NFD_OKAY) {
                try {
                    appContext.model = std::make_unique<Model>(appContext.modelImporter->importModel(outPath));
                } catch (FailedToLoadModelException &ex) {
                    // TODO Log error to log window.
                    std::cerr << ex.what() << std::endl;
                }
                NFD_FreePath(outPath);
            } else if (result == NFD_CANCEL) {
            } else {
                printf("Error: %s\n", NFD_GetError());
            }

            NFD_Quit();
//        }
    }

void Gui::renderLightUI(PointLight &light) {
    ImGui::ColorPicker3("Light Color", glm::value_ptr(light.color));
    ImGui::DragFloat3("Light Position", glm::value_ptr(light.position), 0.001f);
}

void Gui::renderLogUI() {
    ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

    static bool setup = true;
    const float PAD = 10.0f;
    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;
    window_pos.x = work_pos.x + work_size.x - PAD;
    window_pos.y = work_pos.y + work_size.y - PAD;
    window_pos_pivot.x = 1.0f;
    window_pos_pivot.y = 1.0f;
    if(setup) ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
//        window_flags |= ImGuiWindowFlags_NoMove;
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if(setup) ImGui::SetNextWindowSize(ImVec2(600, 150));
    if (ImGui::Begin("Log Overlay", nullptr, window_flags)) {
        auto &logs = appContext.logger->getLogs();
        for(auto &log : std::ranges::views::reverse(logs)) {
            auto color = Logger::getColor(log.logType);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(color.r*255, color.g*255, color.b*255, 255));
            ImGui::Text("[%s](%.2g) %s", Logger::getName(log.logType).c_str(), log.time, log.message.c_str());
            ImGui::PopStyleColor();
        }
    }
    setup = false;
    ImGui::End();
}
