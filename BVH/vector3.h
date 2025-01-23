#ifndef Vector3_h
#define Vector3_h

#include <string>
#include <cmath>
#include "Log.h"

#if defined(__ARM_NEON)
#include <arm_neon.h>
// Terrible hack
typedef float32x4_t __m128;
#else // defined(__AVX__) || defined(__AVX2__) || defined(__AVX512F__) || defined(__SSSE3__) || defined(__SSE3__)
#define __INTEL_SSE 1
#include <emmintrin.h>
#include <pmmintrin.h>
//#else
//#error "Your CPU architecture is not supported"
#endif

// SSE Vector object
struct Vector3 {
  // This is silly, but it still helps.
  union __attribute__((aligned(16))) {
    struct { float x,y,z,w; };
#if defined(__ARM_NEON)
    float32x4_t m128;
#elif defined(__INTEL_SSE)
    __m128 m128;
#endif
  };

  Vector3() { }
  Vector3(float x, float y, float z, float w=0.f) :
#if defined(__ARM_NEON)
      m128{x,y,z,w}
#elif defined(__INTEL_SSE)
      m128(_mm_set_ps(w,z,y,x))
#endif
  { }

#if defined(__ARM_NEON)
  Vector3(const float32x4_t& m128) : m128(m128) { }

  Vector3 operator+(const Vector3& b) const { return vaddq_f32(m128, b.m128); }
  Vector3 operator-(const Vector3& b) const { return vsubq_f32(m128, b.m128); }
  Vector3 operator*(float b) const { return vmulq_n_f32(m128, b); }
  Vector3 operator/(float b) const { return vmulq_n_f32(m128, 1.f/b); }
  Vector3 operator/(const Vector3& b) const { return vdivq_f32(m128, b.m128); }

  // Component-wise multiply and divide
  Vector3 cmul(const Vector3& b) const { return vmulq_f32(m128, b.m128); }
  Vector3 cdiv(const Vector3& b) const { return vdivq_f32(m128, b.m128); }

  // Cross Product
  Vector3 operator^(const Vector3& b) const {
      // TODO: no shuffle instructions in ARM_NEON!
      float32x4_t v0 {vgetq_lane_f32(m128, 3), vgetq_lane_f32(m128, 0), vgetq_lane_f32(m128, 2), vgetq_lane_f32(m128, 1)};
      float32x4_t v1 {vgetq_lane_f32(m128, 3), vgetq_lane_f32(m128, 1), vgetq_lane_f32(m128, 0), vgetq_lane_f32(m128, 2)};
      float32x4_t v2 {vgetq_lane_f32(m128, 3), vgetq_lane_f32(m128, 0), vgetq_lane_f32(m128, 2), vgetq_lane_f32(m128, 1)};

      v0 = vmulq_f32(v0, v1);
      v1 = vmulq_f32(v1, v2);

      return vsubq_f32(v0, v1);
  }

#elif defined(__INTEL_SSE)
  Vector3(const __m128& m128) : m128(m128) { }

  Vector3 operator+(const Vector3& b) const { return _mm_add_ps(m128, b.m128); }
  Vector3 operator-(const Vector3& b) const { return _mm_sub_ps(m128, b.m128); }
  Vector3 operator*(float b) const { return _mm_mul_ps(m128, _mm_set_ps(b,b,b,b)); }
  Vector3 operator/(float b) const { return _mm_div_ps(m128, _mm_set_ps(b,b,b,b)); }
  Vector3 operator/(const Vector3& b) const { return _mm_div_ps(m128, b.m128); }


  // Component-wise multiply and divide
  Vector3 cmul(const Vector3& b) const { return _mm_mul_ps(m128, b.m128); }
  Vector3 cdiv(const Vector3& b) const { return _mm_div_ps(m128, b.m128); }

  // Cross Product
  Vector3 operator^(const Vector3& b) const {
      return _mm_sub_ps(
          _mm_mul_ps(
              _mm_shuffle_ps(m128, m128, _MM_SHUFFLE(3, 0, 2, 1)),
              _mm_shuffle_ps(b.m128, b.m128, _MM_SHUFFLE(3, 1, 0, 2))),
          _mm_mul_ps(
              _mm_shuffle_ps(m128, m128, _MM_SHUFFLE(3, 1, 0, 2)),
              _mm_shuffle_ps(b.m128, b.m128, _MM_SHUFFLE(3, 0, 2, 1)))
          );
  }
#endif


  // dot (inner) product
  float operator*(const Vector3& b) const {
      return x*b.x + y*b.y + z*b.z;
  }

  // Handy component indexing
  float& operator[](const unsigned int i) { return (&x)[i]; }
  const float& operator[](const unsigned int i) const { return (&x)[i]; }
};

#if defined(__ARM_NEON)
inline Vector3 operator*(float a, const Vector3&b)  { return vmulq_n_f32(b.m128, a); }

// Component-wise min
inline Vector3 min(const Vector3& a, const Vector3& b) {
    return vminq_f32(a.m128, b.m128);
}

// Component-wise max
inline Vector3 max(const Vector3& a, const Vector3& b) {
    return vmaxq_f32(a.m128, b.m128);
}

// Make a vector unit length
inline Vector3 normalize(const Vector3& in) {
    Vector3 a = in;
    a.w = 0.f;

    __m128 D = a.m128;
    D = vmulq_f32(D, D);
    float sum = vaddvq_f32(D);
    float rsqrt = 1/sqrt(sum);

    // 1 iteration of Newton-raphson -- Idea from Intel's Embree.
    __m128 r = vdupq_n_f32(rsqrt);
    r = vaddq_f32(
        vmulq_f32(float32x4_t{1.5f, 1.5f, 1.5f, 1.5f}, r),
        vmulq_f32(vmulq_f32(vmulq_f32(D, float32x4_t{-0.5f, -0.5f, -0.5f, -0.5f}), r), vmulq_f32(r, r)));

    return vmulq_f32( a.m128, r );
}

#elif defined(__INTEL_SSE)

inline Vector3 operator*(float a, const Vector3&b)  { return _mm_mul_ps(_mm_set1_ps(a), b.m128); }

// Component-wise min
inline Vector3 min(const Vector3& a, const Vector3& b) {
  return _mm_min_ps(a.m128, b.m128);
}

// Component-wise max
inline Vector3 max(const Vector3& a, const Vector3& b) {
  return _mm_max_ps(a.m128, b.m128);
}

// Make a vector unit length
inline Vector3 normalize(const Vector3& in) {
  Vector3 a = in;
  a.w = 0.f;

  __m128 D = a.m128;
  D = _mm_mul_ps(D, D);
  D = _mm_hadd_ps(D, D);
  D = _mm_hadd_ps(D, D);

  // 1 iteration of Newton-raphson -- Idea from Intel's Embree.
  __m128 r = _mm_rsqrt_ps(D);
  r = _mm_add_ps(
      _mm_mul_ps(_mm_set_ps(1.5f, 1.5f, 1.5f, 1.5f), r),
      _mm_mul_ps(_mm_mul_ps(_mm_mul_ps(D, _mm_set_ps(-0.5f, -0.5f, -0.5f, -0.5f)), r), _mm_mul_ps(r, r)));

  return _mm_mul_ps( a.m128, r );
}
#endif

// Length of a vector
inline float length(const Vector3& a) {
    return sqrtf(a*a);
}


#endif