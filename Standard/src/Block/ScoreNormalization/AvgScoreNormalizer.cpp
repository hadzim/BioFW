/*
 * AvgScoreNormalizer.cpp
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#include "BioFW/Block/ScoreNormalization/AvgScoreNormalizer.h"
#include "BioFW/Evaluation/Statistics/BasicStatistics.h"
#include <iostream>

namespace BioFW {

	AvgScoreNormalizer::AvgScoreNormalizer() :
			TrainableScoreNormalizer("Avg"), genavg(0), impavg(1), totalavg(0) {

	}

	bool AvgScoreNormalizer::internalTrain(const TrainingData & trainingData) {
		Statistics::BasicStatistics bs(trainingData);

		this->genavg = bs.getGenuineMean();
		this->impavg = bs.getImpostorMean();
		this->totalavg = bs.getGlobalMean();

		return true;
	}

	ComparisonScore AvgScoreNormalizer::internalNormalize(ComparisonScore score) {
		double val = (score.getScore() - this->genavg) / (this->impavg - this->genavg);
		return ComparisonScore(val);
	}

} /* namespace BioFW */
