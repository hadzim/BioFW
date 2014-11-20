/*
 * SpatialHistogramExtractor.h
 *
 *  Created on: 10.3.2014
 *      Author: JV
 */

#ifndef SPATIALHISTOGRAMEXTRACTOR_H_
#define SPATIALHISTOGRAMEXTRACTOR_H_
#include "BioFW/FeatureVector/MatFV.h"
#include "BioFW/BiometricSample/MatSample.h"
#include "BioFW/FeatureVector/IFeatureVectorExtractor.h"
#include <opencv2/opencv.hpp>
#include "BioFW/Comparison/NormComparator.h"

namespace BioFW {
	template <class TBiometricSample = MatSample, class TFeatureVector = MatFV>
	class SpatialHistogramExtractor : public IFeatureVectorExtractor <TBiometricSample, TFeatureVector> {
		public:

			typedef TBiometricSample Sample;
			typedef TFeatureVector FeatureVector;


			SpatialHistogramExtractor(int gridx=8, int gridy=8, double threshold = DBL_MAX, int h_len=255,int h_type= CV_8U, int step_size=0) :
				gridX(gridx), gridY(gridy), threshold(threshold), histLen(h_len), histType(h_type), stepSize(step_size) {

			}

			virtual ~SpatialHistogramExtractor(){}

			virtual FeatureVector extractFeatureVector(const Sample & input){
				SpatialHistogramExtractor::FeatureVector fv;

				cv::Mat src = input.mat();
				if (src.empty()) {
					return fv;
				}

				// calculate patch size
				int width = src.cols / gridX;
				int height = src.rows / gridY;

				cv::Mat result = cv::Mat::zeros(0, 0, histType);
				// iterate through grid
				for (int i = 0; i < gridY; i++) {
					for (int j = 0; j < gridX; j++) {
						cv::Mat src_cell(src, cv::Range(i * height, (i + 1) * height), cv::Range(j * width, (j + 1) * width));
						cv::Mat hist = cv::Mat::zeros(1, histLen, histType);

						oper(src_cell, hist);

						result.push_back(hist);
					}
				}
				fv.mat() = result;
				return fv;
			}
		protected:
			virtual void oper(const cv::Mat & src, cv::Mat & hist) = 0;
		private:
			int gridX;
			int gridY;
			double threshold;

			int histLen;
			int histType;

			int stepSize;

	};

} /* namespace BioFW */
#endif /* SPATIALHISTOGRAMEXTRACTOR_H_ */
