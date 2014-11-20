/*
 * IScoreNormalizer.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef ISCORENORMALIZER_H_
#define ISCORENORMALIZER_H_

#include <memory>
#include "BioFW/Comparison/ComparisonScore.h"
#include "BioFW/Evaluation/MethodResults.h"

namespace BioFW {

	class IScoreNormalizer {
		public:
			typedef std::shared_ptr <IScoreNormalizer> Ptr;
			typedef MethodResults TrainingData;

			virtual ~IScoreNormalizer();

			virtual std::string getName() = 0;

			//normalize score
			virtual ComparisonScore normalize(ComparisonScore score) = 0;

			//by default empty implementation
			virtual void train(const TrainingData & results);
	};

} /* namespace BioFW */
#endif /* ISCORENORMALIZER_H_ */
