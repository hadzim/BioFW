/*
 * MinScoreSelector.cpp
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#include "BioFW/Comparsion/ScoreSelector/ScoreSelector.h"
#include "BioFW/Exception.h"
#include <algorithm>

namespace BioFW {

		bool isSmaller(const ComparisonScore & i, const ComparisonScore & j) {
			return i.getScore() < j.getScore();
		}

		ComparisonScore MinScoreSelector::selectScore(const ComparisonScore::List & scores) {
			if (scores.size() == 0) {
				throw BioFWException("ScoreSelector: No score within matching scores list");
			}
			return *std::min_element(scores.begin(), scores.end(), isSmaller);
		}

		ComparisonScore MaxScoreSelector::selectScore(const ComparisonScore::List & scores) {
			if (scores.size() == 0) {
				throw BioFWException("ScoreSelector: No score within matching scores list");
			}
			return *std::max_element(scores.begin(), scores.end(), isSmaller);
		}


} /* namespace BioFW */
