#ifndef BBox_h
#define BBox_h

#include "Ray.h"
#include "Eigen/Dense"
#include <stdint.h>

struct BBox {
  Eigen::Vector3f min, max, extent;
  BBox() { }
  BBox(const Eigen::Vector3f& min, const Eigen::Vector3f& max);
  BBox(const Eigen::Vector3f& p);

  bool intersect(const Ray& ray, float *tnear, float *tfar) const;
  void expandToInclude(const Eigen::Vector3f& p);
  void expandToInclude(const BBox& b);
  uint32_t maxDimension() const;
  float surfaceArea() const;
};

#endif
