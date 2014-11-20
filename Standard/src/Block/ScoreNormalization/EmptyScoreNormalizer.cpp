/*
 * TrainableScoreNormalizer.cpp
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#include "BioFW/Block/ScoreNormalization/EmptyScoreNormalizer.h"

namespace BioFW {

	EmptyScoreNormalizer::EmptyScoreNormalizer(std::string name){
	}

	void EmptyScoreNormalizer::train(const TrainingData & trainingData) {
		return;
	}

	ComparisonScore EmptyScoreNormalizer::normalize(ComparisonScore comparisonScore) {
		return comparisonScore;
	}

	std::string EmptyScoreNormalizer::getName() {
		return "empty";
	}

} /* namespace BioFW */
