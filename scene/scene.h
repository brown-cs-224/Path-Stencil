#ifndef SCENE_H
#define SCENE_H

#include <QString>

#include "BVH/BVH.h"

#include "basiccamera.h"

#include "util/CS123SceneData.h"

#include "shape/mesh.h"

class Scene
{
public:
    Scene();

    static bool load(QString filename, Scene **scenePointer);

    void setBVH(const BVH &bvh);
    const BVH& getBVH() const;

    void setCamera(const BasicCamera& camera);
    void setGlobalData(const CS123SceneGlobalData& data);
    void addLight(const CS123SceneLightData& data);

private:

    BVH *m_bvh;

    BasicCamera m_camera;

    CS123SceneGlobalData m_globalData;

    std::vector<CS123SceneLightData> m_lights;

    static void parseTree(CS123SceneNode *root, Scene *scene);
    static void parseNode(CS123SceneNode *node, const Eigen::Affine3f &parentTransform, std::vector<Object *> *objects);
    static void addPrimitive(CS123ScenePrimitive *prim, const Eigen::Affine3f &transform, std::vector<Object *> *objects);
    static Mesh *loadMesh(std::string filePath, const Eigen::Affine3f &transform);
};

#endif // SCENE_H
