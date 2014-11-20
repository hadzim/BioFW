/*
 * IComparator.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef ICOMPARATOR_H_
#define ICOMPARATOR_H_
#include "BioFW/Comparison/ComparisonScore.h"
#include <memory>

namespace BioFW {
	template <class TProbeFeatureVector, class TReferenceTemplate>
	class IComparator {
		public:
			typedef std::shared_ptr <IComparator> Ptr;

			virtual ~IComparator(){}

        	virtual ComparisonScore computeComparisonScore(const TProbeFeatureVector & probe, const TReferenceTemplate & referenceTemplate) = 0;

	};

} /* namespace BioFW */
#endif /* ICOMPARATOR_H_ */
