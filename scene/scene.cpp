#include "scene.h"

#include "shape/Sphere.h"

#include <util/CS123XmlSceneParser.h>

using namespace Eigen;

Scene::Scene(const BVH &bvh, BasicCamera camera)
    : m_bvh(bvh), m_camera(camera)
{
}

bool Scene::load(QString filename, Scene **scenePointer)
{
    CS123XmlSceneParser parser(filename.toStdString());
    if(!parser.parse()) {
        return false;
    }
    std::vector<Object *> objects;
    Sphere s(Vector3f(0, 0, 0), 1);
    objects.push_back(&s);
    BVH bvh(&objects);
    CS123SceneCameraData cameraData;
    parser.getCameraData(cameraData);
    BasicCamera camera(cameraData.pos.head<3>(),
                       cameraData.look.head<3>(),
                       cameraData.up.head<3>());
    *scenePointer = new Scene(bvh, camera);
    return true;
}

const BVH &Scene::getBVH() const
{
    return m_bvh;
}
