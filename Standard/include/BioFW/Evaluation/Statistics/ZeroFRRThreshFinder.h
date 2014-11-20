/*
 * ZeroFRRThreshFinder.h
 *
 *  Created on: 30.1.2014
 *      Author: JV
 */

#ifndef ZEROFRRTHRESHFINDER_H_
#define ZEROFRRTHRESHFINDER_H_
#include "BioFW/Evaluation/Statistics/IThreshFinder.h"

namespace BioFW {
	namespace Statistics {

		class ZeroFRRThreshFinder : public IThreshFinder {
			public:
				virtual ThreshedStatisticsSettings findThresh(const BasicStatistics & basicStatistics);
		};

	} /* namespace Statistics */
} /* namespace BioFW */
#endif /* ZEROFRRTHRESHFINDER_H_ */
