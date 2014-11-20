/*
 * MethodResults.cpp
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#include "BioFW/Evaluation/IMethodResult.h"
#include "BioFW/Evaluation/MethodResults.h"
#include <iostream>

namespace BioFW {

	MethodResults::MethodResults(Name name) :
			name(name) {

	}

	const ComparisonScore::List & MethodResults::getGenuineScores() const {
		return this->genuine;
	}
	const ComparisonScore::List & MethodResults::getImpostorScores() const {
		return this->impostor;
	}

	const ComparisonScore::List & MethodResults::getAllScores() const {
		return this->all;
	}

	MethodResults::Name MethodResults::getMethodName() const {
		return this->name;
	}

	void MethodResults::addComparisonScore(ComparisonScore score, bool isGenuine) {

		this->all.push_back(score);
		if (isGenuine){
			this->genuine.push_back(score);
		} else {
			this->impostor.push_back(score);
		}
	}

} /* namespace BioFW */
