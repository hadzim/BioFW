/*
 * TrainableScoreNormalizer.cpp
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#include "BioFW/Block/ScoreNormalization/TrainableScoreNormalizer.h"

namespace BioFW {

	TrainableScoreNormalizer::TrainableScoreNormalizer(std::string name) :
			name(name), isTrained_(false) {

	}

	bool TrainableScoreNormalizer::isTrained() {
		return isTrained_;
	}

	void TrainableScoreNormalizer::train(const TrainingData & trainingData) {
		this->isTrained_ = this->internalTrain(trainingData);
	}

	ComparisonScore TrainableScoreNormalizer::normalize(ComparisonScore comparisonScore) {
		if (this->isTrained()) {
			return this->internalNormalize(comparisonScore);
		} else {
			throw std::runtime_error("Score normalizer not trained");
			//return ComparisonScore::invalid();
		}
	}

	std::string TrainableScoreNormalizer::getName() {
		return this->name;
	}

} /* namespace BioFW */
