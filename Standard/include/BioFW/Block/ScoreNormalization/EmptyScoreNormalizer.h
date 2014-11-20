/*
 * TrainableScoreNormalizer.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef EMPTYTRAINABLESCORENORMALIZER_H_
#define EMPTYTRAINABLESCORENORMALIZER_H_
#include "IScoreNormalizer.h"

namespace BioFW {

	class EmptyScoreNormalizer: public IScoreNormalizer {
		public:
			typedef std::shared_ptr <EmptyScoreNormalizer> Ptr;
			EmptyScoreNormalizer(std::string name);
		public:

			void train(const TrainingData & trainingData);
			ComparisonScore normalize(ComparisonScore comparisonScore);
			std::string getName();


	};

} /* namespace BioFW */
#endif /* TRAINABLESCORENORMALIZER_H_ */
