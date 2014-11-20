/*
 * IFeatureExtractor.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef IFEATUREEXTRACTOR_H_
#define IFEATUREEXTRACTOR_H_
#include <memory>
#include "BioFW/Training.h"

namespace BioFW {
	template <class TBiometricSample, class TFeatureVector>
	class IFeatureVectorExtractor {
		public:
			typedef std::shared_ptr <IFeatureVectorExtractor> Ptr;
			typedef TBiometricSample Sample;
			typedef TFeatureVector FeatureVector;

			typedef std::vector <Sample> Samples;
			typedef std::vector <FeatureVector> FeatureVectors;

			virtual ~IFeatureVectorExtractor(){
			}

			virtual FeatureVector extractFeatureVector(const Sample & input) = 0;


			virtual void train(const Samples & inputs, const Labels & labels){
			}
	};

} /* namespace BioFW */
#endif /* IFEATUREEXTRACTOR_H_ */
