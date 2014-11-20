/*
 * DatabaseOverview.h
 *
 *  Created on: 28.1.2014
 *      Author: JV
 */

#ifndef RESULTSDATABASEOVERVIEW_H_
#define RESULTSDATABASEOVERVIEW_H_
#include <iosfwd>
#include "BioFW/Evaluation/Results.h"
#include "BioFW/Evaluation/Statistics/EERThreshFinder.h"
#include <memory>
#include "BioFW/Evaluation/MethodResults.h"
#include "BioFW/Evaluation/Statistics/ThreshedStatistics.h"
namespace BioFW {
	namespace Utils {


		void dumpEEROverview(std::ostream & o, const MethodResults & r){
				Statistics::BasicStatistics bs(r);
				Statistics::ThreshedStatistics s(bs, std::make_shared<Statistics::EERThreshFinder>());

				//const ComparisonScore::List & r.getAllScores()

				o << "Statistics:" << std::endl;
				o << "\tTotal: " << bs.getTotalCount() << std::endl;
				o << "\tTrue accepts: " << s.getTrueAcceptance() << std::endl;
				o << "\tTrue rejects: " << s.getTrueRejection() << std::endl;

				o << "\tFalse accepts: " << s.getFalseAcceptance() << std::endl;
				o << "\tFalse rejects: " << s.getFalseRejection() << std::endl;

				o << "\tFAR: " << s.getFAR() << " and FRR: " << s.getFRR() << std::endl;
		}

	} /* namespace Utils */
} /* namespace BioFW */
#endif /* DATABASEOVERVIEW_H_ */
