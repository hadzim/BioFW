#ifndef COMMON_H
#define COMMON_H


#include <limits>
#include <opencv2/opencv.hpp>

#ifndef NAN
    #define NAN (std::numeric_limits<double>::quiet_NaN());
#endif

#ifndef M_PI
    #define M_PI       3.14159265358979323846
    #define M_PI_2     1.57079632679489661923
    #define M_1_PI     0.318309886183790671538
    #define M_SQRT2    1.41421356237309504880
#endif

typedef cv::Mat_<double> Matrix;

namespace Face {
namespace LinAlg {


}
}

#endif // COMMON_H
