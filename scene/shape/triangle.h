#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <BVH/Object.h>

class Triangle : public Object
{
public:
    Triangle();
    Triangle(Eigen::Vector3f v1, Eigen::Vector3f v2, Eigen::Vector3f v3,
             Eigen::Vector3f n1, Eigen::Vector3f n2, Eigen::Vector3f n3);

    virtual bool getIntersection(const Ray &ray, IntersectionInfo *intersection) const;

    virtual Eigen::Vector3f getNormal(const IntersectionInfo &I) const;

    virtual BBox getBBox() const;

    virtual Eigen::Vector3f getCentroid() const;

private:
    Eigen::Vector3f _v1, _v2, _v3;
    Eigen::Vector3f _n1, _n2, _n3;

    BBox _bbox;

    Eigen::Vector3f _centroid;
};

#endif // TRIANGLE_H
