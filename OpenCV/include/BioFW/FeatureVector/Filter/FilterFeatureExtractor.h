/*
 * FilterFeatureExtractor.h
 *
 *  Created on: 12.3.2014
 *      Author: JV
 */

#ifndef FILTERFEATUREEXTRACTOR_H_
#define FILTERFEATUREEXTRACTOR_H_
#include "BioFW/FeatureVector/IFeatureVectorExtractor.h"
#include "BioFW/FeatureVector/MatFV.h"
#include <memory>

namespace BioFW {

	class FilterFeatureExtractor {
		public:

			typedef std::shared_ptr <FilterFeatureExtractor> Ptr;

			FilterFeatureExtractor(const cv::Mat &realKernel, const cv::Mat &imagKernel, double imageScale);
			virtual ~FilterFeatureExtractor();


			cv::Mat extractPhaseBinaryCode(const cv::Mat &inputImage) const;

			cv::Mat extractAbsoluteResponse(const cv::Mat &inputImage) const;

		private:
			cv::Mat realKernel;
			cv::Mat imagKernel;
			double imageScale;
	};

} /* namespace BioFW */
#endif /* FILTERFEATUREEXTRACTOR_H_ */
