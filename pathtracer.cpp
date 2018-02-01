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
    Vector3f intensityValues[m_width * m_height];
    Matrix4f invViewMat = (scene.getCamera().getScaleMatrix() * scene.getCamera().getViewMatrix()).inverse();
    for(int y = 0; y < m_height; ++y) {
        for(int x = 0; x < m_width; ++x) {
            int offset = x + (y * m_width);
            intensityValues[offset] = tracePixel(x, y, scene, invViewMat);
        }
    }

    toneMap(imageData, intensityValues);
}

Vector3f PathTracer::tracePixel(int x, int y, const Scene& scene, const Matrix4f &invViewMatrix)
{
    Vector3f p(0, 0, 0);
    Vector3f d((2.f * x / m_width) - 1, 1 - (2.f * y / m_height), -1);
    d.normalize();

    Ray r(p, d);
    return traceRay(r, scene, invViewMatrix);
}

Vector3f PathTracer::traceRay(const Ray& r, const Scene& scene, const Matrix4f& invViewMatrix)
{
    IntersectionInfo i;
    Ray ray(r.transform(invViewMatrix));
    if(scene.getBVH().getIntersection(ray, &i, false)) {
        //const Mesh * m = static_cast<const Mesh *>(i.object);//Get the mesh that was intersected
        //const Triangle *t = static_cast<const Triangle *>(i.data);//Get the triangle in the mesh that was intersected
        //const tinyobj::material_t& mat = m->getMaterial(t->getIndex());//Get the material of the triangle from the mesh
        //const tinyobj::real_t *d = mat.diffuse;//Diffuse color
        //const std::string diffuseTex = mat.diffuse_texname;//Diffuse texture name
        return Vector3f(1, 1, 1);
    } else {
        return Vector3f(0, 0, 0);
    }
}

void PathTracer::toneMap(QRgb *imageData, Vector3f *intensityValues) {
    for(int y = 0; y < m_height; ++y) {
        for(int x = 0; x < m_width; ++x) {
            int offset = x + (y * m_width);
            imageData[offset] = intensityValues[offset].norm() > 0 ? qRgb(255, 255, 255) : qRgb(40, 40, 40);
        }
    }

}
