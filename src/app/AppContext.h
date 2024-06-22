//
// Created by faliszewskii on 16.06.24.
//

#ifndef OPENGL_TEMPLATE_APPCONTEXT_H
#define OPENGL_TEMPLATE_APPCONTEXT_H

#include <memory>
#include "../opengl/framebuffer/FrameBufferManager.h"
#include "../interface/camera/BaseCamera.h"
#include "entity/quad/Quad.h"
#include "../opengl/shader/Shader.h"
#include "entity/light/PointLight.h"
#include "entity/point/Point.h"
#include "entity/cylinder/Cylinder.h"
#include "../opengl/mesh/PositionVertex.h"
#include "../../dep/texture-atlas-generator/src/alg/LSCM.h"
#include "../interface/logger/Logger.h"
#include "../opengl/texture/Texture.h"
#include "../interface/import/ModelImporter.h"

struct AppContext {
    AppContext() = default;

    std::unique_ptr<BaseCamera> camera;
    std::unique_ptr<FrameBufferManager> frameBufferManager;
    std::unique_ptr<Logger> logger;

    // Shaders
    std::unique_ptr<Shader> phongShader;
    std::unique_ptr<Shader> pointShader;

    // TODO --- App data goes here
    std::unique_ptr<ModelImporter> modelImporter;

    std::unique_ptr<Model> model;

    std::unique_ptr<PointLight> light;
    std::unique_ptr<Point> lightBulb;
    std::unique_ptr<Cylinder> cylinder;
    std::unique_ptr<Mesh<PositionVertex>> uvMapMesh;

    std::unique_ptr<TAGen::LSCM> lscm;

    std::unique_ptr<Texture> testTexture;
};

#endif //OPENGL_TEMPLATE_APPCONTEXT_H
