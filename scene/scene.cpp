#include "scene.h"

#include "shape/Sphere.h"

#include <util/CS123XmlSceneParser.h>

#include <Eigen/Geometry>

#include <iostream>

#include <Eigen/StdVector>

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
    if(!parseTree(root, scene)) {
        return false;
    }

    *scenePointer = scene;
    return true;
}

void Scene::setBVH(const BVH &bvh)
{
    m_bvh = new BVH(bvh);
}

bool Scene::parseTree(CS123SceneNode *root, Scene *scene)
{
    //TODO this leaks memory right now
    std::vector<Object *> *objects = new std::vector<Object *>;
    parseNode(root, Affine3f(), objects);
    if(objects->size() == 0) {
        return false;
    }
    //TODO this leaks memory right now
    BVH *bvh = new BVH(objects);

    scene->setBVH(*bvh);
    return true;
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
    std::vector<Vector3f> colors;
    std::vector<Vector2f> uvs;
    std::vector<int> materialIds;
    std::vector<Vector3i> faces;

    //TODO populate vectors and use tranform
    for(size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];

            Vector3i face;
            for(size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
                tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
                tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
                tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
                tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
                tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
                tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
                tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];
                tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
                tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
                tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];

                face[v] = vertices.size();
                vertices.push_back(Vector3f(vx, vy, vz));
                normals.push_back(Vector3f(nx, ny, nz));
                uvs.push_back(Vector2f(tx, ty));
                colors.push_back(Vector3f(red, green, blue));
            }
            faces.push_back(face);
            materialIds.push_back(shapes[s].mesh.material_ids[f]);

            index_offset += fv;
        }
    }

    Mesh *m = new Mesh(vertices, normals, uvs, colors, faces, materialIds, materials);
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

