#ifndef SCENE_H
#define SCENE_H

#include <QString>

#include "BVH/BVH.h"

#include "basiccamera.h"

class Scene
{
public:
    Scene(const BVH& m_bvh, BasicCamera camera);

    static bool load(QString filename, Scene **scenePointer);

    const BVH& getBVH() const;

private:

    BVH m_bvh;

    BasicCamera m_camera;

};

#endif // SCENE_H
