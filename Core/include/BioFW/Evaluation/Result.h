/*
 * Result.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef RESULT_H_
#define RESULT_H_
#include <string>
#include <map>
#include <set>
#include "BioFW/Database/Record.h"
#include "BioFW/Comparison/ComparisonScore.h"

namespace BioFW {

	class Result {
		public:
			typedef std::vector<Result> List;

			typedef std::string MethodName;
			typedef std::map<MethodName, ComparisonScore> Scores;
			typedef std::set<MethodName> Methods;

			Result(Record templated, Record tested);

			const Record getTemplateRecord() const;
			const Record getTestedRecord() const;

			const Scores & getComparisonScores() const;
			//Scores & getComparisonScores();

			const Methods & getMethods() const;

			ComparisonScore getComparisonScore(MethodName method) const;
			void setComparisonScore(MethodName method, ComparisonScore score);

			bool isGenuine() const;
			void merge(const Result & anotherResult);

			std::string getUniqueId() const;
		private:
			Record templateRecord;
			Record testedRecord;

			Scores comparisonScores;
			Methods methods;
	};

} /* namespace BioFW */
#endif /* RESULT_H_ */
