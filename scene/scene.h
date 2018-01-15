#ifndef SCENE_H
#define SCENE_H

#include <QString>

#include "BVH/BVH.h"

#include "basiccamera.h"

class Scene
{
public:
    Scene(const BVH& m_bvh, BasicCamera camera);

    static Scene load(QString filename);

    const BVH& getBVH();

private:

    BVH m_bvh;

    BasicCamera m_camera;

};

#endif // SCENE_H
