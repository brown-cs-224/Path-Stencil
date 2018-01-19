#include "scene.h"

#include "shape/Sphere.h"

#include <util/CS123XmlSceneParser.h>

using namespace Eigen;

Scene::Scene(const BVH &bvh)
    : m_bvh(bvh)
{
}

bool Scene::load(QString filename, Scene **scenePointer)
{
    CS123XmlSceneParser parser(filename.toStdString());
    if(!parser.parse()) {
        return false;
    }
    //TODO this leaks memory right now
    std::vector<Object *> *objects = new std::vector<Object *>;
    Sphere *s = new Sphere(Vector3f(0.5, 0, -1), 1);
    objects->push_back(s);
    BVH bvh(objects);
    CS123SceneCameraData cameraData;
    parser.getCameraData(cameraData);
    BasicCamera camera(cameraData.pos.head<3>(),
                       cameraData.look.head<3>(),
                       cameraData.up.head<3>());
    Scene *scene = new Scene(bvh);
    scene->setCamera(camera);

    CS123SceneGlobalData globalData;
    parser.getGlobalData(globalData);
    scene->setGlobalData(globalData);

    CS123SceneLightData lightData;
    for(int i = 0, size = parser.getNumLights(); i < size; ++i) {
        parser.getLightData(i, lightData);
        scene->addLight(lightData);
    }

    CS123SceneNode *root = parser.getRootNode();
    parseTree(root, scene);

    *scenePointer = scene;
    return true;
}

void Scene::parseTree(CS123SceneNode *root, Scene *scene)
{

}

const BVH &Scene::getBVH() const
{
    return m_bvh;
}

void Scene::setCamera(const BasicCamera &camera)
{
    m_camera = camera;
}

void Scene::setGlobalData(const CS123SceneGlobalData& data)
{
    m_globalData = data;
}

void Scene::addLight(const CS123SceneLightData &data)
{
    m_lights.push_back(data);
}

