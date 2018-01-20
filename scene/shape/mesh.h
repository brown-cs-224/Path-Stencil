#ifndef MESH_H
#define MESH_H

#include <BVH/Object.h>
#include <BVH/BVH.h>

#include <util/tiny_obj_loader.h>

#include <vector>

#include <Eigen/StdVector>

EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Matrix2f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Matrix3f)
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Eigen::Matrix3i)

class Mesh : public Object
{
public:
    Mesh(const std::vector<Eigen::Vector3f> &vertices,
         const std::vector<Eigen::Vector3f> &normals,
         const std::vector<Eigen::Vector2f> &uvs,
         const std::vector<Eigen::Vector3f> &colors,
         const std::vector<Eigen::Vector3i> &faces,
         const std::vector<int> materialIds,
         const std::vector<tinyobj::material_t> materials);
    virtual ~Mesh();

    virtual bool getIntersection(const Ray &ray, IntersectionInfo *intersection) const;

    virtual Eigen::Vector3f getNormal(const IntersectionInfo &I) const;

    virtual BBox getBBox() const;

    virtual Eigen::Vector3f getCentroid() const;

private:
    std::vector<Eigen::Vector3f> _vertices;
    std::vector<Eigen::Vector3f> _normals;
    std::vector<Eigen::Vector3f> _colors;
    std::vector<Eigen::Vector2f> _uvs;
    std::vector<Eigen::Vector3i> _faces;
    std::vector<int> _materialIds;
    std::vector<tinyobj::material_t> _materials;

    BVH *_meshBvh;

    Eigen::Vector3f _centroid;

    BBox _bbox;

    void calculateMeshStats();
    void createMeshBVH();
};

#endif // MESH_H
