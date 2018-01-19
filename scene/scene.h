#ifndef SCENE_H
#define SCENE_H

#include <QString>

#include "BVH/BVH.h"

#include "basiccamera.h"

#include "util/CS123SceneData.h"

class Scene
{
public:
    Scene(const BVH& m_bvh);

    static bool load(QString filename, Scene **scenePointer);

    const BVH& getBVH() const;

    void setCamera(const BasicCamera& camera);
    void setGlobalData(const CS123SceneGlobalData& data);
    void addLight(const CS123SceneLightData& data);

private:

    BVH m_bvh;

    BasicCamera m_camera;

    CS123SceneGlobalData m_globalData;

    std::vector<CS123SceneLightData> m_lights;

};

#endif // SCENE_H
