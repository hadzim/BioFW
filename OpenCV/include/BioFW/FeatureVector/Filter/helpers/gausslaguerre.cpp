#include "gausslaguerre.h"
#include <math.h>

using namespace Face::LinAlg;

unsigned int factorial(unsigned int n) {
	unsigned int ret = 1;
	for (unsigned int i = 1; i <= n; ++i)
		ret *= i;
	return ret;
}

double over(double n, double k) {
	return ((double) factorial(n)) / (factorial(k) * factorial(n - k));
}

void GaussLaguerre::createWavelet(Matrix &real, Matrix &imag, int kernelSize, int n, int k) {
	int j = 0;

	//log2(x) == log(x) / log(2.0)
	int size = pow(2, ceil(log(kernelSize * 0.75) / log(2.0)));

	real = Matrix(size, size);
	imag = Matrix(size, size);

	for (int y = 0; y < size; y++) {
		double realY = size / 2 - y;
		for (int x = 0; x < size; x++) {
			double realX = size / 2 - x;
			double r = sqrt(realX * realX + realY * realY) / (kernelSize / 4);
			double theta = atan2(realY, realX);

			real(y, x) = h(r, theta, n, k, j) * cos(n * theta);
			imag(y, x) = h(r, theta, n, k, j) * sin(n * theta);
		}
	}

	//powerOfTwoEnvelope(real);
	//powerOfTwoEnvelope(imag);
}

double GaussLaguerre::L(double r, int n, int k) {
	double sum = 0;
	for (int h = 0; h <= k; h++) {
		sum += pow((double) -1, k) * over(n + k, k - h) * pow((double) r, h) / factorial(h);
	}
	return sum;
}

double GaussLaguerre::h(double r, double theta, int n, int k, int j) {
	return pow((double) -1, k) * pow(2, (fabs((double) n) + 1.0) / 2.0) * pow(M_PI, fabs((double) n) / 2.0) * pow((((double) (factorial(k))) / factorial(fabs((double) n) + k)), 0.5) * pow((double) r, fabs((double) n))
			* L(2 * M_PI * r * r, n, k) * exp(-M_PI * r * r) * exp(j * n * theta);
}
