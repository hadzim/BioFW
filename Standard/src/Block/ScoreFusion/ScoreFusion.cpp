/*
 * TrainableScoreFusion.cpp
 *
 *  Created on: 11.3.2014
 *      Author: JV
 */

#include "BioFW/Block/ScoreFusion/ScoreFusion.h"

namespace BioFW {

	ScoreFusion::ScoreFusion(std::string name, IScoreNormalizer::Ptr scoreNormalizer_) :
			name(name), scoreNormalizer_(scoreNormalizer_) {

	}

	ScoreFusion::~ScoreFusion() {

	}

	void ScoreFusion::train(const TrainingData & results) {

	}
	std::string ScoreFusion::getName() const {
		return this->name;
	}

	IScoreNormalizer::Ptr ScoreFusion::scoreNormalizer(){
		return this->scoreNormalizer_;
	}

} /* namespace BioFW */
