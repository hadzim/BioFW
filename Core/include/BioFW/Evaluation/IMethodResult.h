/*
 * IMethodResult.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef IMETHODRESULT_H_
#define IMETHODRESULT_H_
#include <string>
#include "BioFW/Comparison/ComparisonScore.h"

namespace BioFW {

	class IMethodResults {
		public:

			typedef std::string Name;

			virtual ~IMethodResults() {

			}

			virtual const ComparisonScore::List & getGenuineScores() const = 0;
			virtual const ComparisonScore::List & getImpostorScores() const = 0;

			virtual const ComparisonScore::List & getAllScores() const = 0;

			virtual Name getMethodName() const = 0;

			virtual void addComparisonScore(ComparisonScore score, bool isGenuine) = 0;
	};

}  // namespace BioFW

#endif /* IMETHODRESULT_H_ */
