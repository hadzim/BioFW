/*
 * MethodResults.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef METHODRESULTS_H_
#define METHODRESULTS_H_
#include "BioFW/Evaluation/IMethodResult.h"

namespace BioFW {

	class MethodResults: public IMethodResults {

		public:
			MethodResults(Name name);

			virtual const ComparisonScore::List & getGenuineScores() const;
			virtual const ComparisonScore::List & getImpostorScores() const;

			virtual const ComparisonScore::List & getAllScores() const;

			virtual Name getMethodName() const;

			virtual void addComparisonScore(ComparisonScore score, bool isGenuine);

		private:
			Name name;
			ComparisonScore::List genuine;
			ComparisonScore::List impostor;
			ComparisonScore::List all;
	};

} /* namespace BioFW */
#endif /* METHODRESULTS_H_ */
