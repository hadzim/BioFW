/*
 * IFeatureVectorComparator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef IFEATUREVECTORCOMPARATOR_H_
#define IFEATUREVECTORCOMPARATOR_H_
#include "BioFW/Comparison/ComparisonScore.h"
#include <memory>

namespace BioFW {
	template <class TProbeFeatureVector, class TReferenceFeatureVector = TProbeFeatureVector>
	class IFeatureVectorComparator {
		public:
			typedef std::shared_ptr <IFeatureVectorComparator> Ptr;
			typedef TProbeFeatureVector ProbeFeatureVector;
			typedef TReferenceFeatureVector ReferenceFeatureVector;

			virtual ~IFeatureVectorComparator(){
			}
			virtual ComparisonScore computeComparisonScore(const ProbeFeatureVector & probe, const ReferenceFeatureVector & ref) = 0;
	};

} /* namespace BioFW */
#endif /* IFEATUREVECTORCOMPARATOR_H_ */
