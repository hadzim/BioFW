/*
 * ZeroFARThreshFinder.h
 *
 *  Created on: 30.1.2014
 *      Author: JV
 */

#ifndef ZEROFARTHRESHFINDER_H_
#define ZEROFARTHRESHFINDER_H_
#include "BioFW/Evaluation/Statistics/IThreshFinder.h"

namespace BioFW {
	namespace Statistics {

		class ZeroFARThreshFinder : public IThreshFinder {
			public:

				virtual ThreshedStatisticsSettings findThresh(const BasicStatistics & basicStatistics);
		};

	} /* namespace Statistics */
} /* namespace BioFW */
#endif /* ZEROFARTHRESHFINDER_H_ */
