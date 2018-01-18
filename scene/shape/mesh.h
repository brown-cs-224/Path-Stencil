#ifndef MESH_H
#define MESH_H

#include <BVH/Object.h>
#include <BVH/BVH.h>

#include <vector>

class Mesh : public Object
{
public:
    Mesh(const std::vector<Eigen::Vector3f> &vertices, const std::vector<Eigen::Vector3i> &faces);
    virtual ~Mesh();

    virtual bool getIntersection(const Ray &ray, IntersectionInfo *intersection) const;

    virtual Eigen::Vector3f getNormal(const IntersectionInfo &I) const;

    virtual BBox getBBox() const;

    virtual Eigen::Vector3f getCentroid() const;

private:
    std::vector<Eigen::Vector3f> _vertices;
    std::vector<Eigen::Vector3i> _faces;

    BVH *_meshBvh;

    Eigen::Vector3f _centroid;

    BBox _bbox;

    void calculateMeshStats();
    void createMeshBVH();
};

#endif // MESH_H
