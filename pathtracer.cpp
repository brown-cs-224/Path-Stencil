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
            imageData[offset] = tracePixel(x, y);
        }
    }
}

QRgb PathTracer::tracePixel(int x, int y)
{
    Vector3f p(0, 0, 0);
    Vector3f d((2.f * x / m_width) - 1, 1 - (2.f * y / m_height), -1);
    d.normalize();

    return traceRay(Ray(p, d));
}

QRgb PathTracer::traceRay(Ray r)
{
    int red = 255 * (r.d.dot(Vector3f(0, 0, -1)));
    return qRgb(red, 0, 0);
}
