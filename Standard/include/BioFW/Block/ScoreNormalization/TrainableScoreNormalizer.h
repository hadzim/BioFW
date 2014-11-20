/*
 * TrainableScoreNormalizer.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef TRAINABLESCORENORMALIZER_H_
#define TRAINABLESCORENORMALIZER_H_
#include "IScoreNormalizer.h"

namespace BioFW {

	class TrainableScoreNormalizer: public IScoreNormalizer {
		public:
			typedef std::shared_ptr <TrainableScoreNormalizer> Ptr;
			TrainableScoreNormalizer(std::string name);

		protected:

			virtual bool internalTrain(const TrainingData & trainingData) = 0;

			virtual ComparisonScore internalNormalize(ComparisonScore score) = 0;

			bool isTrained();

		public:

			void train(const TrainingData & trainingData);
			ComparisonScore normalize(ComparisonScore comparisonScore);
			std::string getName();

		private:
			std::string name;
			bool isTrained_;

	};

} /* namespace BioFW */
#endif /* TRAINABLESCORENORMALIZER_H_ */
