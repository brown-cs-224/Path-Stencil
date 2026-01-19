#ifndef SCENE_H
#define SCENE_H

#include <QString>

#include "BVH/BVH.h"

#include "basiccamera.h"

#include "util/SceneData.h"

#include "shape/mesh.h"

#include <memory>

class Scene
{
public:
    Scene();
    virtual ~Scene();

    static bool load(QString filename, Scene **scenePointer, float imageWidth, float imageHeight);

    void setBVH(const BVH &bvh);
    const BVH& getBVH() const;

    const BasicCamera& getCamera() const;

    void setCamera(const BasicCamera& camera);
    void setGlobalData(const SceneGlobalData& data);
    void addLight(const SceneLightData& data);

    const std::vector<SceneLightData>& getLights();

    bool getIntersection(const Ray& ray, IntersectionInfo* I) const;

    // returns all triangles in the scene whose material has non-zero emission
    const std::vector<Triangle*>& getEmissives() const { return m_emissives; };

private:

    BVH *m_bvh;
    std::vector<Object *> *_objects;

    BasicCamera m_camera;

    SceneGlobalData m_globalData;
    std::vector<Triangle*> m_emissives;

    std::vector<SceneLightData> m_lights;

    static bool parseTree(SceneNode *root, Scene *scene, const std::string& baseDir);
    static void parseNode(SceneNode *node, const Eigen::Affine3f &parentTransform, std::vector<Object *> *objects, const std::string& baseDir);
    static void addPrimitive(ScenePrimitive *prim, const Eigen::Affine3f &transform, std::vector<Object *> *objects, const std::string& baseDir);
    static Mesh *loadMesh(std::string filePath, const Eigen::Affine3f &transform, const std::string& baseDir);
};

#endif // SCENE_H
