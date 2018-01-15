#include "basiccamera.h"

using namespace Eigen;

BasicCamera::BasicCamera(Vector3f position, Vector3f direction, Vector3f up)
    : m_position(position), m_direction(direction), m_up(up)
{
}

Matrix4f BasicCamera::getViewMatrix()
{
    Vector3f f = m_direction.normalized();
    Vector3f u = m_up.normalized();
    Vector3f s = f.cross(u);
    u = s.cross(f);

    Matrix4f Result;
    Result(0, 0) = s(0);
    Result(1, 0) = s(1);
    Result(2, 0) = s(2);
    Result(0, 1) = u(0);
    Result(1, 1) = u(1);
    Result(2, 1) = u(2);
    Result(0, 2) =-f(0);
    Result(1, 2) =-f(1);
    Result(2, 2) =-f(2);
    Result(3, 0) =-s.dot(m_position);
    Result(3, 1) =-u.dot(m_position);
    Result(3, 2) =-f.dot(m_position);
    Result(3, 3) =1;
    return Result;
}
