#ifndef Ray_h
#define Ray_h

#include "Eigen/Dense"

struct Ray {
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  void setO(const Eigen::Vector3f o) {
      this->o = o;
  }
  void setD(const Eigen::Vector3f d) {
      this->d = d;
      this->inv_d = Eigen::Vector3f(1, 1, 1).cwiseQuotient(d);
  }

  const Eigen::Vector3f& getO() const {return o;} // Ray Origin
  const Eigen::Vector3f& getD() const {return d;} // Ray Direction
  const Eigen::Vector3f& getInvD() const {return inv_d;} // Inverse of each Ray Direction component

private:
  Eigen::Vector3f o; // Ray Origin
  Eigen::Vector3f d; // Ray Direction
  Eigen::Vector3f inv_d; // Inverse of each Ray Direction component
};

#endif
