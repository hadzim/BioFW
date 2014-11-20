/*
 * AvgScoreNormalizer.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef AVGSCORENORMALIZER_H_
#define AVGSCORENORMALIZER_H_
#include "TrainableScoreNormalizer.h"

namespace BioFW {


	class AvgScoreNormalizer : public TrainableScoreNormalizer {

		public:
			typedef std::shared_ptr <AvgScoreNormalizer> Ptr;
	        AvgScoreNormalizer();
		protected:

	        virtual bool internalTrain(const TrainingData & trainingData);
	        virtual ComparisonScore internalNormalize(ComparisonScore score);

		protected:
	        double genavg;
	        double impavg;
	        double totalavg;

	    };

} /* namespace BioFW */
#endif /* MINMAXSCORENORMALIZER_H_ */
