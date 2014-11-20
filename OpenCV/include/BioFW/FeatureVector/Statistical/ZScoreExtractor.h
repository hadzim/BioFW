/*
 * ZScoreExtractor.h
 *
 *  Created on: 12.3.2014
 *      Author: JV
 */

#ifndef ZSCOREEXTRACTOR_H_
#define ZSCOREEXTRACTOR_H_
#include "BioFW/FeatureVector/IFeatureVectorExtractor.h"
#include "BioFW/FeatureVector/MatFV.h"

namespace BioFW {


	class ZScoreExtractor : public IFeatureVectorExtractor<MatFV, MatFV> {
		public:
			ZScoreExtractor();
			virtual ~ZScoreExtractor();

			virtual FeatureVector extractFeatureVector(const Sample & input);
			virtual void train(const Samples & inputs, const Labels & labels);
		private:
			int cols;
			cv::Mat mean;
			cv::Mat stddev;
	};

} /* namespace BioFW */
#endif /* ZSCOREEXTRACTOR_H_ */
