/*
 * MinMaxScoreNormalizer.cpp
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#include "BioFW/Block/ScoreNormalization/MinMaxScoreNormalizer.h"
#include "BioFW/Evaluation/Statistics/BasicStatistics.h"
#include <iostream>

namespace BioFW {

	MinMaxScoreNormalizer::MinMaxScoreNormalizer() :
			TrainableScoreNormalizer("MinMax"), min(0), max(1) {

	}

	bool MinMaxScoreNormalizer::internalTrain(const TrainingData & trainingData) {
		Statistics::BasicStatistics bs(trainingData);

		this->min = bs.getGlobalExtremes().getMin();
		this->max = bs.getGlobalExtremes().getMax();

		if (min == max) {
			throw std::runtime_error("MinMaxScoreNormalizationModule min and max are the same");
		}
		return true;
	}

	ComparisonScore MinMaxScoreNormalizer::internalNormalize(ComparisonScore score) {
		double val = (score.getScore() - this->min) / (this->max - this->min);
		return ComparisonScore(val);
	}

} /* namespace BioFW */
