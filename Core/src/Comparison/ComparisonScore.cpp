/*
 * ComparisonScore.cpp
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#include <math.h>
#include "BioFW/Comparison/ComparisonScore.h"

namespace BioFW {

	ComparisonScore ComparisonScore::invalid(){
		return ComparisonScore(NAN);
	}


	ComparisonScore::ComparisonScore(Score score) : score(score){

	}
	ComparisonScore::ComparisonScore(Score score, Subscores subscores) : score(score), subscores(subscores){

	}

	ComparisonScore::Score ComparisonScore::getScore() const {
		return score;
	}

	bool ComparisonScore::isValid() const {
		return !isnan(score);
	}

	const ComparisonScore::Subscores & ComparisonScore::getSubscores() const {
		return subscores;
	}

} /* namespace BioFW */
