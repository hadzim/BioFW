/*
 * ZScoreNormalizer.cpp
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#include "BioFW/Block/ScoreNormalization/ZScoreNormalizer.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include "BioFW/Evaluation/Statistics/BasicStatistics.h"

namespace BioFW {

	template <class T>
	void average(const std::vector<T>& vec, double & mean, double & stddev){
			double sum(0);
			std::for_each(vec.begin(), vec.end(), [&sum](const T& value){ sum += value.getScore(); });
			mean = sum/vec.size();
			double newsum(0);
			std::for_each(vec.begin(), vec.end(), [&newsum, &mean](const T& value){
				int diff = value.getScore() - mean;
				newsum += (diff * diff);
			});

			stddev = std::sqrt(newsum/vec.size());
	}

	ZScoreNormalizer::ZScoreNormalizer() :
			TrainableScoreNormalizer("ZScore"), mean(0), stdDev(0) {

	}

	ZScoreNormalizer::~ZScoreNormalizer() {

	}

	bool ZScoreNormalizer::internalTrain(const TrainingData & trainingData) {

		Statistics::BasicStatistics bs(trainingData);

		average(trainingData.getAllScores(), this->mean, this->stdDev);

		return true;
	}


	ComparisonScore ZScoreNormalizer::internalNormalize(ComparisonScore score) {
		double val = (score.getScore() - this->mean) / (this->stdDev);
		return ComparisonScore(val);
	}

} /* namespace BioFW */
