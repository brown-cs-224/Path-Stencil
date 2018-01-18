#include "mesh.h"

#include "triangle.h"

using namespace Eigen;
using namespace std;

Mesh::Mesh(const vector<Vector3f> &vertices, const vector<Vector3i> &faces)
    : _vertices(vertices), _faces(faces)
{
    calculateMeshStats();
    createMeshBVH();
}

Mesh::~Mesh()
{
   delete _meshBvh;
}

bool Mesh::getIntersection(const Ray &ray, IntersectionInfo *intersection) const
{
    return _meshBvh->getIntersection(ray, intersection, false);
}

Vector3f Mesh::getNormal(const IntersectionInfo &I) const
{
    return I.object->getNormal(I);
}

BBox Mesh::getBBox() const
{
    return _bbox;
}

Vector3f Mesh::getCentroid() const
{
    return _centroid;
}

void Mesh::calculateMeshStats()
{
    _bbox = BBox(_vertices[0]);
    for(auto v : _vertices) {
        _centroid += v;
        _bbox.expandToInclude(v);
    }
    _centroid /= _vertices.size();
}

void Mesh::createMeshBVH()
{
    Triangle *triArray = new Triangle[_faces.size()];
    std::vector<Object *> objects;
    objects.resize(_faces.size());
    for(unsigned int i = 0; i < _faces.size(); ++i) {
        Vector3i face = _faces[i];
        Vector3f v1 = _vertices[face(0)];
        Vector3f v2 = _vertices[face(1)];
        Vector3f v3 = _vertices[face(2)];
        triArray[i] = Triangle(v1, v2, v3);
        objects[i] = &triArray[i];
    }

    _meshBvh = new BVH(&objects);

    delete[] triArray;
}
