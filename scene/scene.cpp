#include "scene.h"

#include <BVH/Sphere.h>

using namespace Eigen;

Scene::Scene(const BVH &bvh, BasicCamera camera)
    : m_bvh(bvh), m_camera(camera)
{
}

Scene Scene::load(QString filename)
{
    std::vector<Object *> objects;
    Sphere s(Vector3f(0, 0, 0), 1);
    objects.push_back(&s);
    BVH bvh(&objects);
    BasicCamera camera(Vector3f(0, 0, 0),
                       Vector3f(0, 0, -1),
                       Vector3f(0, 1, 0));
    return Scene(bvh, camera);
}

const BVH &Scene::getBVH()
{
    return m_bvh;
}
