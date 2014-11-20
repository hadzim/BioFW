/*
 * Results.h
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#ifndef RESULTS_H_
#define RESULTS_H_
#include "BioFW/Evaluation/Result.h"
#include "BioFW/Evaluation/MethodResults.h"

namespace BioFW {

	class Results {
		public:
			typedef Result::MethodName MethodName;
			typedef Result::Methods Methods;

			Results();

			const Result::List & getResults() const;
			const Methods & getMethods() const;
			MethodResults getMethodResult(MethodName name) const;

			void addResult(Result r);


	        void merge(Results other);
		private:
	        Result & getExistingResult(const Result & r);
		private:
			Result::List results;

			typedef std::map<MethodName, int> IDIndexer;
			IDIndexer idIndexer;

			typedef std::map<MethodName, MethodResults> MethodResultsContainer;
			MethodResultsContainer methodsResults;
			Methods methods;
	};

} /* namespace BioFW */
#endif /* RESULTS_H_ */
