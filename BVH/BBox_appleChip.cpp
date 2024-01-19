#include "BBox.h"
#include <algorithm>
// #include "vector3.h"

#include <iostream>

using Eigen::Vector3f;

void BBox::setMinMax(const Vector3f& min, const Vector3f& max)
{
    this->min = min;
    this->max = max;
    extent = max - min;
}

void BBox::setP(const Vector3f& p) {
    min = p;
    max = p;
    extent = max - min;
}

void BBox::expandToInclude(const Vector3f& p) {
    min = min.cwiseMin(p);
    max = max.cwiseMax(p);
    extent = max - min;
}

void BBox::expandToInclude(const BBox& b) {
    min = min.cwiseMin(b.min);
    max = max.cwiseMax(b.max);
    extent = max - min;
}

uint32_t BBox::maxDimension() const {
    uint32_t result = 0;
    if(extent(1) > extent(0)) result = 1;
    if(extent(2) > extent(1)) result = 2;
    return result;
}

float BBox::surfaceArea() const {
    return 2.f*( extent(0)*extent(2) + extent(0)*extent(1) + extent(1)*extent(2) );
}

// http://www.flipcode.com/archives/SSE_RayBox_Intersection_Test.shtml
// turn those verbose intrinsics into something readable.
// #define loadps(mem)		_mm_load_ps((const float * const)(mem))
// #define storess(ss,mem)		_mm_store_ss((float * const)(mem),(ss))
// #define minss			_mm_min_ss
// #define maxss			_mm_max_ss
// #define minps			_mm_min_ps
// #define maxps			_mm_max_ps
// #define mulps			_mm_mul_ps
// #define subps			_mm_sub_ps
// #define rotatelps(ps)		_mm_shuffle_ps((ps),(ps), 0x39)	// a,b,c,d -> b,c,d,a
// #define muxhps(low,high)	_mm_movehl_ps((low),(high))	// low{a,b,c,d}|high{e,f,g,h} = {c,d,g,h}

static const float flt_plus_inf = -logf(0);	// let's keep C and C++ compilers happy.
static const float __attribute__((aligned(16)))
ps_cst_plus_inf[4] = {  flt_plus_inf,  flt_plus_inf,  flt_plus_inf,  flt_plus_inf },
    ps_cst_minus_inf[4] = { -flt_plus_inf, -flt_plus_inf, -flt_plus_inf, -flt_plus_inf };
bool BBox::intersect(const Ray& ray, float *tnear, float *tfar) const {
    float box_min[3] = {min(0), min(1), min(2)};
    float box_max[3] = {max(0), max(1), max(2)};

    // Unpack the origin and inverse direction of the ray
    float ray_origin[3] = {ray.o(0), ray.o(1), ray.o(2)};

    float l1[3], l2[3], lmin[3], lmax[3];

    // Calculate intersection intervals for each dimension
    for (int i = 0; i < 3; ++i) {
        l1[i] = (box_min[i] - ray_origin[i]) * ray.inv_d(i);
        l2[i] = (box_max[i] - ray_origin[i]) * ray.inv_d(i);

        lmin[i] = l1[i] < l2[i] ? l1[i] : l2[i];
        lmax[i] = l1[i] > l2[i] ? l1[i] : l2[i];
    }

    // Find the maximum of the mins and the minimum of the maxs
    float max_lmin = lmin[0] > lmin[1] ? (lmin[0] > lmin[2] ? lmin[0] : lmin[2]) : (lmin[1] > lmin[2] ? lmin[1] : lmin[2]);
    float min_lmax = lmax[0] < lmax[1] ? (lmax[0] < lmax[2] ? lmax[0] : lmax[2]) : (lmax[1] < lmax[2] ? lmax[1] : lmax[2]);

    *tnear = max_lmin;
    *tfar = min_lmax;

    // Check if the ray intersects the bounding box
    return min_lmax >= max_lmin && min_lmax >= 0;
}

