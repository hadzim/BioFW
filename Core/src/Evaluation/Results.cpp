/*
 * Results.cpp
 *
 *  Created on: 25.1.2014
 *      Author: JV
 */

#include "BioFW/Evaluation/Results.h"
#include "BioFW/Exception.h"

namespace BioFW {

	Results::Results() {
	}

	const Result::List & Results::getResults() const {
		return this->results;
	}

	const Results::Methods & Results::getMethods() const {
		return this->methods;
	}

	MethodResults Results::getMethodResult(Results::MethodName name) const {
		return this->methodsResults.at(name);
	}

	void Results::addResult(Result r) {
		try {
			Result & existing = this->getExistingResult(r);
			existing.merge(r);
		} catch (...){
			this->results.push_back(r);
			this->idIndexer[r.getUniqueId()] = this->results.size() - 1;
		}

		//add to methods results
		for (Methods::const_iterator i = r.getMethods().begin(); i != r.getMethods().end(); i++){
            if (methodsResults.find(*i) == methodsResults.end()){
            	methodsResults.insert(std::make_pair(*i, MethodResults(*i)));
            }
            methodsResults.at(*i).addComparisonScore(r.getComparisonScore(*i), r.isGenuine());
            methods.insert(*i);
		}
	}
	Result & Results::getExistingResult(const Result & r) {
		if (idIndexer.find(r.getUniqueId()) != idIndexer.end()) {
			return results.at(idIndexer.at(r.getUniqueId()));
		}
		throw BioFWException("Non existing result");
	}

	void Results::merge(Results other) {
	}

} /* namespace BioFW */
