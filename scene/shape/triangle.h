#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <BVH/Object.h>
#include <util/tiny_obj_loader.h>

class Triangle : public Object
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Triangle();
    Triangle(Eigen::Vector3f v1, Eigen::Vector3f v2, Eigen::Vector3f v3,
             Eigen::Vector3f n1, Eigen::Vector3f n2, Eigen::Vector3f n3,
             int index);

    bool getIntersection(const Ray &ray, IntersectionInfo *intersection) const override;

    Eigen::Vector3f getNormal(const IntersectionInfo &I) const override;
    virtual Eigen::Vector3f getNormal(const Eigen::Vector3f &p) const;

    BBox getBBox() const override;

    Eigen::Vector3f getCentroid() const override;

    int getIndex() const;

    tinyobj::material_t getMaterial() const;
    void setMaterial(const tinyobj::material_t &material);

    Eigen::Vector3<Eigen::Vector3f> getVertices() { return Eigen::Vector3<Eigen::Vector3f>(_v1, _v2, _v3); }
    Eigen::Vector3<Eigen::Vector3f> getNormals()  { return Eigen::Vector3<Eigen::Vector3f>(_n1, _n2, _n3); }

private:
    Eigen::Vector3f _v1, _v2, _v3;
    Eigen::Vector3f _n1, _n2, _n3;

    tinyobj::material_t m_material;

    int m_index;

    BBox _bbox;

    Eigen::Vector3f _centroid;

};

#endif // TRIANGLE_H
