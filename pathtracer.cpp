#include "pathtracer.h"

#include <iostream>

#include <Eigen/Dense>

using namespace Eigen;

PathTracer::PathTracer(int width, int height)
    : m_width(width), m_height(height)
{
}

void PathTracer::traceScene(QRgb *imageData, const Scene& scene)
{
    for(int y = 0; y < m_height; ++y) {
        for(int x = 0; x < m_width; ++x) {
            int offset = x + (y * m_width);
            imageData[offset] = tracePixel(x, y, scene);
        }
    }
}

QRgb PathTracer::tracePixel(int x, int y, const Scene &scene)
{
    Vector3f p(0, 0, 0);
    Vector3f d((2.f * x / m_width) - 1, 1 - (2.f * y / m_height), -1);
    d.normalize();

    return traceRay(Ray(p, d), scene);
}

QRgb PathTracer::traceRay(const Ray& r, const Scene& scene)
{
    return qRgb(255, 255, 0);
    IntersectionInfo i;
    if(scene.getBVH().getIntersection(r, &i, true)) {
        return qRgb(255, 255, 255);
    } else {
        return qRgb(0, 0, 0);
    }
}
