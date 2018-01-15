#ifndef BASICCAMERA_H
#define BASICCAMERA_H

#include "camera.h"

class BasicCamera : public Camera
{
public:
    BasicCamera(Eigen::Vector3f position, Eigen::Vector3f direction, Eigen::Vector3f up);

    virtual Eigen::Matrix4f getViewMatrix();
private:
    Eigen::Vector3f m_position;
    Eigen::Vector3f m_direction;
    Eigen::Vector3f m_up;
};

#endif // BASICCAMERA_H
