#ifndef Ray_h
#define Ray_h

#include "Eigen/Dense"

struct Ray {
  Eigen::Vector3f o; // Ray Origin
  Eigen::Vector3f d; // Ray Direction
  Eigen::Vector3f inv_d; // Inverse of each Ray Direction component

  Ray(const Eigen::Vector3f& o, const Eigen::Vector3f& d)
    : o(o), d(d), inv_d(Eigen::Vector3f(1,1,1).cwiseQuotient(d)) { }
};

#endif
