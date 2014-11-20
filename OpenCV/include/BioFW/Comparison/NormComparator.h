/*
 * IFeatureVectorComparator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef MyNORMEFVECTORCOMPARATOR_H_
#define MyNORMFEVECTORCOMPARATOR_H_

#include "BioFW/Comparison/ComparisonScore.h"
#include <memory>
#include "BioFW/FeatureVector/MatFV.h"
#include "BioFW/Comparison/IFeatureVectorComparator.h"
#include <math.h>


namespace BioFW {
	template <int Norm, class TProbeFeatureVector, class TReferenceFeatureVector>
	class NormComparator : public IFeatureVectorComparator <TProbeFeatureVector, TReferenceFeatureVector> {
		public:
			typedef std::shared_ptr <NormComparator> Ptr;

			virtual ~NormComparator(){

			}
			virtual ComparisonScore computeComparisonScore(const TProbeFeatureVector & probe, const TReferenceFeatureVector & ref){
				return cv::norm(probe.mat(),ref.mat(), Norm);
			}
	};

	//typedef NormComparator<cv::NORM_L1, MatFV, MatFV> NormL1Comparator;


} /* namespace BioFW */
#endif /* MyNORMEFVECTORCOMPARATOR_H_ */
