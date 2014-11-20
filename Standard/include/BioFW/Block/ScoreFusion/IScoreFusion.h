/*
 * IScoreFusion.h
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#ifndef ISCOREFUSION_H_
#define ISCOREFUSION_H_

#include <memory>
#include "BioFW/Comparison/ComparisonScore.h"
#include "BioFW/Evaluation/Results.h"
#include "BioFW/Block/ScoreNormalization/IScoreNormalizer.h"

namespace BioFW {

	class IScoreFusion {
		public:
			typedef std::shared_ptr <IScoreFusion> Ptr;
			typedef Results TrainingData;
			virtual ~IScoreFusion();

			virtual ComparisonScore resolve(const ComparisonScore::Subscores & matchingSubscores)= 0;

			virtual std::string getName() const = 0;

			virtual void train(const Results & results) = 0;

			virtual IScoreNormalizer::Ptr scoreNormalizer() = 0;
	};

} /* namespace BioFW */
#endif /* ISCOREFUSION_H_ */
