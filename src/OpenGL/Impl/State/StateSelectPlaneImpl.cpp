// Shipeng Xu
// billhsu.x@gmail.com

#include "StateSelectPlaneImpl.h"
#include "OpenGL/Impl/Scene/Scene.h"
#include <OpenGL/gl3.h>
#include "OpenGL/Impl/Basic/PlaneRenderer.h"
#include "Core/Controller/Controller.h"
#include "OpenGL/DepthPeeling/DepthPeeling.h"
#include "OpenGL/Impl/Basic/PointRenderer.h"
#include "OpenGL/TextureManager/TextureManager.h"
#include "OpenGL/Impl/Scene/DrawLineSegment.h"

Vector3 StateSelectPlaneImpl::renderCurrentPlaneCenter;
Vector4 StateSelectPlaneImpl::renderCurrentPlaneColor;
Plane StateSelectPlaneImpl::currentPlane;
TextureManager* StateSelectPlaneImpl::textureManager = NULL;
StateSelectPlaneImpl::StateSelectPlaneImpl() {
    depthPeeling = &DepthPeeling::getInstance();
    textureManager = &TextureManager::getInstance();
}
void StateSelectPlaneImpl::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Vector3 center(0, 0, 0);
    for (int i = 0; i < selectedPoints.size(); ++i) {
        center += selectedPoints[i];
    }
    center /= selectedPoints.size();
    Vector4 color = Vector4(0.3, 0.3, 0.3, 0.1);
    currentPlane = Controller::currPlane;
    renderCurrentPlaneColor = color;
    renderCurrentPlaneCenter = center;
    Scene::renderSketchLines(NULL);
    depthPeeling->addToRenderCallbackList(renderCurrentPlane);
    depthPeeling->addToRenderCallbackList(Scene::drawSceneWrapper);
    depthPeeling->addToRenderCallbackList(Scene::renderCurrentPoint);
    depthPeeling->addToRenderCallbackList(renderCurrentPoints);
}

void StateSelectPlaneImpl::renderCurrentPlane(void* data) {
    PlaneRenderer::getPlaneShader()->bind();
    GLuint local_modelView = glGetUniformLocation(
        PlaneRenderer::getPlaneShader()->getProgram(), "modelView");
    glUniformMatrix4fv(local_modelView, 1, GL_FALSE,
                       Controller::modelView.get());
    GLuint local_projection = glGetUniformLocation(
        PlaneRenderer::getPlaneShader()->getProgram(), "projection");
    glUniformMatrix4fv(local_projection, 1, GL_FALSE,
                       Controller::projection.get());

    PlaneRenderer::render(currentPlane, renderCurrentPlaneCenter, 20,
                          renderCurrentPlaneColor);
}

void StateSelectPlaneImpl::renderCurrentPoints(void* data) {
    PointRenderer::getPointShader()->bind();
    GLuint local_modelView = glGetUniformLocation(
        PointRenderer::getPointShader()->getProgram(), "modelView");
    glUniformMatrix4fv(local_modelView, 1, GL_FALSE,
                       Controller::modelView.get());
    GLuint local_projection = glGetUniformLocation(
        PointRenderer::getPointShader()->getProgram(), "projection");
    glUniformMatrix4fv(local_projection, 1, GL_FALSE,
                       Controller::projection.get());
    GLuint local_pointSize = glGetUniformLocation(
        PointRenderer::getPointShader()->getProgram(), "pointSize");
    glUniform1f(local_pointSize, 0.3f);
    glUniform1i(
        glGetUniformLocation(PointRenderer::getPointShader()->getProgram(),
                             "pointTexture"),
        1);
    PointRenderer::getPointList().clear();
    for_each(selectedPoints.begin(), selectedPoints.end(),
             [](Vector3 v) { PointRenderer::getPointList().push_back(v); });
    PointRenderer::render(
        textureManager->getTexture("media/textures/point_3.png").glTextureID);
}
