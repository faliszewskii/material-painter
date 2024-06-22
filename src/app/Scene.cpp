//
// Created by faliszewskii on 16.06.24.
//

#include <algorithm>
#include "Scene.h"
#include "../interface/camera/CameraAnchor.h"
#include "../model/renderer/ModelRenderer.h"

Scene::Scene(AppContext &appContext) : appContext(appContext) {
    appContext.camera = std::make_unique<CameraAnchor>(1920, 1080, glm::vec3(0.0f, 3.0f, 3.0f), glm::vec3(0.f), glm::vec3(-M_PI / 4, 0, 0));
    appContext.frameBufferManager = std::make_unique<FrameBufferManager>();
    appContext.frameBufferManager->create_buffers(appContext.camera->screenWidth, appContext.camera->screenHeight);
    appContext.modelImporter = std::make_unique<ModelImporter>();
    appContext.model = std::make_unique<Model>();
    appContext.logger = std::make_unique<Logger>();
    appContext.lscm = std::make_unique<TAGen::LSCM>();

    // TODO --- Initialization of the app state goes here.

    appContext.phongShader = std::make_unique<Shader>(Shader::createTraditionalShader(
            "../res/shaders/phong/phong.vert", "../res/shaders/phong/phong.frag"));
    appContext.pointShader = std::make_unique<Shader>(Shader::createTraditionalShader(
            "../res/shaders/point/point.vert", "../res/shaders/point/point.frag"));

    appContext.cylinder = std::make_unique<Cylinder>();
    appContext.light = std::make_unique<PointLight>();
    appContext.light->position = {1.0f , 0.0f, 0.25f};
    appContext.lightBulb = std::make_unique<Point>();

    std::vector<PositionVertex> uvMapVertices;
    std::transform(appContext.cylinder->vertices.begin(), appContext.cylinder->vertices.end(), std::back_inserter(uvMapVertices),
                   [&](PosNorTexVertex &el) {
        return PositionVertex{{el.texCoords.x, el.texCoords.y, 1}};
    });
    appContext.uvMapMesh = std::make_unique<Mesh<PositionVertex>>(Mesh<PositionVertex>(uvMapVertices, appContext.cylinder->indices, GL_TRIANGLES));

    appContext.testTexture = std::make_unique<Texture>("../res/textures/debugTexture2.jpg", true);
}

void Scene::update() {
    // TODO --- Here goes scene data update.
    appContext.lightBulb->position = appContext.light->position;
    appContext.lightBulb->color = glm::vec4(appContext.light->color, 1);
}

void Scene::render() {
    appContext.frameBufferManager->bind();

    // TODO --- Here goes scene render.
    appContext.phongShader->use();
    appContext.phongShader->setUniform("viewPos", appContext.camera->getViewPosition());
    appContext.phongShader->setUniform("view", appContext.camera->getViewMatrix());
    appContext.phongShader->setUniform("projection", appContext.camera->getProjectionMatrix());
    appContext.light->setupPointLight(*appContext.phongShader);
    appContext.phongShader->setUniform("model", glm::identity<glm::mat4>());
    appContext.phongShader->setUniform("material.hasTexture", true);
    appContext.testTexture->bind(0);
    appContext.phongShader->setUniform("material.albedo", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    appContext.phongShader->setUniform("material.shininess", 256.f);
//    ModelRenderer::render(*appContext.model, *appContext.phongShader);
    appContext.cylinder->render();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    appContext.uvMapMesh->render();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    appContext.pointShader->use();
    appContext.pointShader->setUniform("view", appContext.camera->getViewMatrix());
    appContext.pointShader->setUniform("projection", appContext.camera->getProjectionMatrix());
    appContext.lightBulb->render(*appContext.pointShader);

    appContext.frameBufferManager->unbind();
}
