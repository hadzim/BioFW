/*
 * GaborFeatureExtractor.h
 *
 *  Created on: 12.3.2014
 *      Author: JV
 */

#ifndef GABORFEATUREEXTRACTOR_H_
#define GABORFEATUREEXTRACTOR_H_
#include "BioFW/FeatureVector/IFeatureVectorExtractor.h"
#include "BioFW/FeatureVector/MatFV.h"
#include "FilterFeatureExtractor.h"


namespace BioFW {

	class GaborFeatureExtractor  : public IFeatureVectorExtractor<MatFV, MatFV> {
		public:
			GaborFeatureExtractor(int frequency, int orientation, double scale);
			virtual ~GaborFeatureExtractor();

			virtual FeatureVector extractFeatureVector(const Sample & input);
		private:
			FilterFeatureExtractor::Ptr filter;
	};

} /* namespace BioFW */
#endif /* GABORFEATUREEXTRACTOR_H_ */
