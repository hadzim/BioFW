#ifndef GABOR_H
#define GABOR_H
#include "BioFW/FeatureVector/Filter/helpers/common.h"


namespace Face {
namespace LinAlg {

class Gabor
{
public:

    static void createWavelet(Matrix &real, Matrix &imag, int frequency, int orientation);
private:
    static void gaborFunc(int x, int y, double omega, double theta, double sigma, double &real, double &imag);
};

}
}

#endif // GABOR_H
