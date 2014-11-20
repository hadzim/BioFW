/*
 * SVMScoreFusion.cpp
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#include "BioFW/Block/ScoreFusion/SumScoreFusion.h"
#include "BioFW/Block/ScoreNormalization/ZScoreNormalizer.h"

namespace BioFW {

	SumScoreFusion::SumScoreFusion() : ScoreFusion("Sum", std::make_shared<ZScoreNormalizer>()) {

	}

	SumScoreFusion::~SumScoreFusion() {

	}

	ComparisonScore SumScoreFusion::resolve(const ComparisonScore::Subscores & matchingSubscores){

		double sum(0);

		for (auto sscore : matchingSubscores) {
			sum += sscore.second;
		}

		return ComparisonScore(sum / matchingSubscores.size(), matchingSubscores);

	}

} /* namespace BioFW */
