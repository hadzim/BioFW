/*
 * CosineComparison.h
 *
 *  Created on: 13.3.2014
 *      Author: JV
 */

#ifndef HistogramComparatorCOSINECOMPARISON_H_
#define HistogramComparatorCOSINECOMPARISON_H_
#include "BioFW/Comparison/IFeatureVectorComparator.h"

namespace BioFW {



	class HistogramComparator : public IFeatureVectorComparator <MatFV, MatFV> {
		public:
			typedef std::shared_ptr <HistogramComparator> Ptr;

			virtual ~HistogramComparator(){

			}
			virtual ComparisonScore computeComparisonScore(const MatFV & probe, const MatFV & ref){
				double dist = compareHist(ref.mat(), probe.mat(), CV_COMP_CHISQR);
				return ComparisonScore(dist);
			}
	};

}  // namespace BioFW


#endif /* COSINECOMPARISON_H_ */
