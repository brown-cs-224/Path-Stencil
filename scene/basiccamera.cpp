#include "basiccamera.h"

using namespace Eigen;

BasicCamera::BasicCamera(Vector3f position, Vector3f direction, Vector3f up)
    : m_position(position), m_direction(direction), m_up(up)
{
}

Matrix4f BasicCamera::getViewMatrix() const
{
    Vector3f f = m_direction.normalized();
    Vector3f u = m_up.normalized();
    Vector3f s = f.cross(u);
    u = s.cross(f);

    Matrix4f Result;
    Result << s.x(),s.y(),s.z(), -s.dot(m_position),
            u.x(),u.y(),u.z(), -u.dot(m_position),
            -f.x(),-f.y(),-f.z(), f.dot(m_position),
            0, 0, 0, 1;
    return Result;
}
