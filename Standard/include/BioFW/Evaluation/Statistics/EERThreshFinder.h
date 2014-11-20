/*
 * EERThreshFinder.h
 *
 *  Created on: 30.1.2014
 *      Author: JV
 */

#ifndef EERTHRESHFINDER_H_
#define EERTHRESHFINDER_H_
#include "BioFW/Evaluation/Statistics/IThreshFinder.h"

namespace BioFW {
	namespace Statistics {

		class EERThreshFinder: public IThreshFinder {
			public:

				virtual ThreshedStatisticsSettings findThresh(const BasicStatistics & basicStatistics);



		};

	} /* namespace Statistics */
} /* namespace BioFW */
#endif /* EERTHRESHFINDER_H_ */
