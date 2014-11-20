/*
 * MinMaxScoreNormalizer.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef MINMAXSCORENORMALIZER_H_
#define MINMAXSCORENORMALIZER_H_
#include "TrainableScoreNormalizer.h"

namespace BioFW {


	class MinMaxScoreNormalizer : public TrainableScoreNormalizer {

		public:
			typedef std::shared_ptr <MinMaxScoreNormalizer> Ptr;
	        MinMaxScoreNormalizer();
		protected:

	        virtual bool internalTrain(const TrainingData & trainingData);
	        virtual ComparisonScore internalNormalize(ComparisonScore score);

		protected:
	        double min;
	        double max;

	    };

} /* namespace BioFW */
#endif /* MINMAXSCORENORMALIZER_H_ */
