/*
 * CosineComparison.h
 *
 *  Created on: 13.3.2014
 *      Author: JV
 */

#ifndef COSINECOMPARISON_H_
#define COSINECOMPARISON_H_
#include "BioFW/Comparison/IFeatureVectorComparator.h"
#include "BioFW/FeatureVector/Filter/helpers/common.h"

namespace BioFW {

	Matrix mat2doubleRow(const cv::Mat & m){
		Matrix tmpMat =  Matrix::zeros(1, m.total());
		m.reshape(1,1).convertTo(tmpMat, CV_64FC1);
		return tmpMat;
	}

	double sqrMagnitude(Matrix & m)
	{
	    int n = m.rows;
	    double sum = 0;
	    for (int i = 0; i < n; i++)
	    {
	        double v = m(i);
	        sum +=  (v * v);
	    }
	    return sum;
	}

	double magnitude(Matrix & m){
	    return sqrt(sqrMagnitude(m));
	}

	class CosineComparator : public IFeatureVectorComparator <MatFV, MatFV> {
		public:
			typedef std::shared_ptr <CosineComparator> Ptr;

			virtual ~CosineComparator(){

			}
			virtual ComparisonScore computeComparisonScore(const MatFV & probe, const MatFV & ref){

				Matrix m1 = mat2doubleRow(probe.mat());
				Matrix m2 = mat2doubleRow(ref.mat());

				//std::cout << "m1: " << m1.cols << std::endl;
				//std::cout << "m2: " << m2.cols << std::endl;

				 Matrix mul = (m1*m2.t());

				 //std::cout << "mul: " << mul.total() << std::endl;

				 //std::cout << "mag1: " << magnitude(m1) << std::endl;
				 //std::cout << "mag2: " << magnitude(m2) << std::endl;


				 double dist = 1.0 - (mul(0))/(magnitude(m1) * magnitude(m2));

				 //std::cout << "dist: " << dist << std::endl;

				 return ComparisonScore(dist);
			}
	};

}  // namespace BioFW


#endif /* COSINECOMPARISON_H_ */
