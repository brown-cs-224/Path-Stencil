/**
 * @file CS123Common.h
 *
 * Contains data structures and macros commonly used in CS123.
 */
#pragma once
#ifndef __CS123COMMON_H__
#define __CS123COMMON_H__

#include <math.h>
#include <fstream>
#include <vector>


//// glu.h in different location on macs
//#ifdef __APPLE__
//#include <glu.h>
//#else
//#include <GL/glu.h>
//#endif

// from http://en.wikipedia.org/wiki/Assertion_(computing)
#define COMPILE_TIME_ASSERT(pred) switch(0){case 0:case pred:;}

typedef float REAL;

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

// ---------------------
// Common math utilities
// ---------------------

const float FLOAT_EPSILON = 1e-4f;
const double DOUBLE_EPSILON = 1e-8;

inline bool floatEpsEqual(float a, float b) {
    // If the difference between a and b is less than epsilon, they are equal
    return fabs(a - b) < FLOAT_EPSILON;
}

inline bool doubleEpsEqual(double a, double b)
{
    // If the difference between a and b is less than epsilon, they are equal
    return fabs(a - b) < DOUBLE_EPSILON;
}

inline Eigen::Vector4f vec3Tovec4(const Eigen::Vector3f &v, float w) {
    return Eigen::Vector4f(v.x(), v.y(), v.z(), w);
}

//following two functions are adapted from Microsoft's AirSim from https://github.com/microsoft/AirSim/blob/main/AirLib/include/common/common_utils/Utils.hpp
//   as of Jan 21, 2024
// STUDENTS: IGNORE!!
// check whether machine is little endian
inline static bool isLittleEndian()
{
    int intval = 1;
    unsigned char* uval = reinterpret_cast<unsigned char*>(&intval);
    return uval[0] == 1;
}

//STUDENTS: use this function anywhere this file is defined
//path: the filepath of the .pfm image to be output (you must specific ".pfm")
inline static void outputPFM(const std::string& path, int width, int height, std::vector<Eigen::Vector3f> &intensityValues, float scalef = 1) {
    std::ofstream file(path.c_str(), std::ios::binary);

            std::string bands;
            float fvalue; // scale factor and temp value to hold pixel value
            bands = "PF"; // rgb

            // sign of scalefact indicates endianness, see pfm specs
            if (isLittleEndian())
                scalef = -scalef;

            // insert header information
            file << bands << "\n";
            file << width << " ";
            file << height << "\n";
            file << scalef << "\n";

            if (bands == "PF") { // handles 3-band image
                for (int i = 0; i < height; ++i) {
                    for (int j = 0; j < width; ++j) {
                        for (int p = 0; p < 3; ++p) {
                            fvalue = intensityValues[(height-1-i) * width + j](p); //PFMs are defined from bottom to top
                            file.write(reinterpret_cast<char*>(&fvalue), sizeof(fvalue));
                        }
                    }
                }
            }

            file.close();
}

#endif
