#ifndef GAUSSLAGUERRE_H
#define GAUSSLAGUERRE_H
#include <opencv2/opencv.hpp>
#include "BioFW/FeatureVector/Filter/helpers/common.h"

namespace Face {
namespace LinAlg {

class GaussLaguerre
{
public:

    static void createWavelet(Matrix &real, Matrix &imag, int kernelSize, int n, int k);

private:
    static double L(double r, int n, int k);
    static double h(double r, double theta, int n, int k, int j);
};

}
}

#endif // GAUSSLAGUERRE_H
