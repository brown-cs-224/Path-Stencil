#include "triangle.h"

#include "util/CS123Common.h"

using namespace Eigen;

Triangle::Triangle()
{
}

Triangle::Triangle(Vector3f v1, Vector3f v2, Vector3f v3, Vector3f n1, Vector3f n2, Vector3f n3)
    : _v1(v1), _v2(v2), _v3(v3), _n1(n1), _n2(n2), _n3(n3)
{
    _centroid = (_v1 + _v2 + _v3) / 3.f;
    _bbox.setP(_v1);
    _bbox.expandToInclude(_v2);
    _bbox.expandToInclude(_v3);
}

bool Triangle::getIntersection(const Ray &ray, IntersectionInfo *intersection) const
{
    //https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    Vector3f edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = _v2 - _v1;
    edge2 = _v3 - _v1;

    h = ray.d.cross(edge2);
    a = edge1.dot(h);

    if(floatEpsEqual(a, 0)) {
        return false;
    }
    f = 1/a;
    s = ray.o - _v1;
    u = f * s.dot(h);
    if(u < 0.f || u > 1.f) {
        return false;
    }
    q = s.cross(edge1);
    v = f * ray.o.dot(q);
    if(v < 0.f || u + v > 1.f) {
        return false;
    }
    float t = f * edge2.dot(q);
    if(t > FLOAT_EPSILON) {
        intersection->t = t;
        intersection->object = this;
        return true;
    } else {
        return false;
    }
}

Vector3f Triangle::getNormal(const IntersectionInfo &I) const
{
    return (_n1 + _n2 + _n3) / 3.f;
}

BBox Triangle::getBBox() const
{
    return _bbox;
}

Vector3f Triangle::getCentroid() const
{
    return (_v1 + _v2 + _v3) / 3.f;
}
