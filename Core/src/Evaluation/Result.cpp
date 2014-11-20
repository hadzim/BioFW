/*
 * Result.cpp
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#include "BioFW/Evaluation/Result.h"
#include <iostream>

namespace BioFW {

	Result::Result(Record templated, Record tested) : templateRecord(templated), testedRecord(tested){}

	const Record Result::getTemplateRecord() const {
		return this->templateRecord;
	}
	const Record Result::getTestedRecord() const {
		return this->testedRecord;
	}

	const Result::Scores & Result::getComparisonScores() const {
		return this->comparisonScores;
	}
	//Scores & getComparisonScores();

	const Result::Methods & Result::getMethods() const {
		return this->methods;
	}

	ComparisonScore Result::getComparisonScore(MethodName method) const {
		if (comparisonScores.find(method) == comparisonScores.end()) return ComparisonScore::invalid();
		return comparisonScores.at(method);
	}
	void Result::setComparisonScore(MethodName method, ComparisonScore score) {
		if (comparisonScores.find(method) == comparisonScores.end()){
			comparisonScores.insert(std::make_pair(method, score));
			methods.insert(method);
		} else {
			comparisonScores.at(method) = score;
		}


	}

	bool Result::isGenuine() const {
		return this->templateRecord.getBiometricID() == this->testedRecord.getBiometricID();
	}
	void Result::merge(const Result & anotherResult) {
		for (Result::Scores::const_iterator i = anotherResult.comparisonScores.begin(); i != anotherResult.comparisonScores.end(); i++){
			this->setComparisonScore(i->first, i->second);
		}
	}

	std::string Result::getUniqueId() const {
		return this->templateRecord.getSampleID() + "_" + this->testedRecord.getSampleID();
	}
} /* namespace BioFW */
