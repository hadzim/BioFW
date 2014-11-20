/*
 * ZScoreNormalizer.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef ZSCORENORMALIZER_H_
#define ZSCORENORMALIZER_H_
#include "TrainableScoreNormalizer.h"

namespace BioFW {

	class ZScoreNormalizer : public TrainableScoreNormalizer {
		public:
			ZScoreNormalizer();
			virtual ~ZScoreNormalizer();

		protected:

			virtual bool internalTrain(const TrainingData & trainingData);
			virtual ComparisonScore internalNormalize(ComparisonScore score);

		private:

			double mean;
			double stdDev;

	};

} /* namespace BioFW */
#endif /* ZSCORENORMALIZER_H_ */
