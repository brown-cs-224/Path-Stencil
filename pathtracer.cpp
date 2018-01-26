#include "pathtracer.h"

#include <iostream>

#include <Eigen/Dense>

#include <util/CS123Common.h>

using namespace Eigen;

PathTracer::PathTracer(int width, int height)
    : m_width(width), m_height(height)
{
}

void PathTracer::traceScene(QRgb *imageData, const Scene& scene)
{
    Matrix4f invViewMat = scene.getCamera().getViewMatrix().inverse();
    for(int y = 0; y < m_height; ++y) {
        for(int x = 0; x < m_width; ++x) {
            int offset = x + (y * m_width);
            imageData[offset] = tracePixel(x, y, scene, invViewMat);
        }
    }
}

QRgb PathTracer::tracePixel(int x, int y, const Scene& scene, const Matrix4f &invViewMatrix)
{
    Vector3f p(0, 0, 0);
    Vector3f d((2.f * x / m_width) - 1, 1 - (2.f * y / m_height), -1);
    d.normalize();

    Ray r;
    r.setO(p);
    r.setD(d);
    return traceRay(r, scene, invViewMatrix);
}

QRgb PathTracer::traceRay(const Ray& r, const Scene& scene, const Matrix4f& invViewMatrix)
{
    IntersectionInfo i;
    Ray ray;
    ray.setO((invViewMatrix * vec3Tovec4(r.getO(), 1.f)).head<3>());
    ray.setD((invViewMatrix * vec3Tovec4(r.getD(), 0.f)).head<3>());
    if(scene.getBVH().getIntersection(ray, &i, true)) {
        return qRgb(255, 255, 255);
    } else {
        return qRgb(40, 40, 40);
    }
}
