#include "scene.h"

#include "shape/Sphere.h"

#include <util/CS123XmlSceneParser.h>

#include <Eigen/Geometry>

#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "util/tiny_obj_loader.h"

using namespace Eigen;

Scene::Scene()
{
}

bool Scene::load(QString filename, Scene **scenePointer)
{
    CS123XmlSceneParser parser(filename.toStdString());
    if(!parser.parse()) {
        return false;
    }
    CS123SceneCameraData cameraData;
    parser.getCameraData(cameraData);
    BasicCamera camera(cameraData.pos.head<3>(),
                       cameraData.look.head<3>(),
                       cameraData.up.head<3>());
    Scene *scene = new Scene;
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

void Scene::setBVH(const BVH &bvh)
{
    m_bvh = new BVH(bvh);
}

void Scene::parseTree(CS123SceneNode *root, Scene *scene)
{
    //TODO this leaks memory right now
    std::vector<Object *> *objects = new std::vector<Object *>;
    parseNode(root, Affine3f(), objects);
    Sphere *s = new Sphere(Vector3f(0.5, 0, -1), 1);
    objects->push_back(s);
    BVH bvh(objects);

    scene->setBVH(bvh);
}

void Scene::parseNode(CS123SceneNode *node, const Affine3f &parentTransform, std::vector<Object *> *objects)
{
    Affine3f transform = parentTransform;
    for(CS123SceneTransformation *trans : node->transformations) {
            switch(trans->type) {
            case TRANSFORMATION_TRANSLATE:
                transform = transform * Translation<float, 3>(trans->translate);
                break;
            case TRANSFORMATION_SCALE:
                transform = transform * Scaling(trans->scale);
                break;
            case TRANSFORMATION_ROTATE:
                transform = transform * AngleAxis<float>(trans->angle, trans->rotate);
                break;
            case TRANSFORMATION_MATRIX:
                transform = transform * trans->matrix;
                break;
            }
        }
        for(CS123ScenePrimitive *prim : node->primitives) {
            addPrimitive(prim, transform, objects);
        }
        for(CS123SceneNode *child : node->children) {
            parseNode(child, transform, objects);
        }
}

void Scene::addPrimitive(CS123ScenePrimitive *prim, const Affine3f &transform, std::vector<Object *> *objects)
{
    switch(prim->type) {
    case PrimitiveType::PRIMITIVE_MESH:
        objects->push_back(loadMesh(prim->meshfile, transform));
        break;
    default:
        std::cerr << "We don't handle any other formats yet" << std::endl;
        break;
    }
}

Mesh *Scene::loadMesh(std::string filePath, const Affine3f &transform)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err,
                                filePath.data(), nullptr, true);
    if(!err.empty()) {
        std::cerr << err << std::endl;
    }

    if(!ret) {
        std::cerr << "Failed to load/parse .obj file" << std::endl;
        return nullptr;
    }

    std::vector<Vector3f> vertices;
    std::vector<Vector3f> normals;
    std::vector<Vector2f> uvs;
    std::vector<Vector3i> faces;

    //TODO populate vectors and use tranform

    Mesh *m = new Mesh(vertices, normals, uvs, faces);
    return m;

}

const BVH &Scene::getBVH() const
{
    return *m_bvh;
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

