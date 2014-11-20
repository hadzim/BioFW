/*
 * ThreshHelpers.h
 *
 *  Created on: 30.1.2014
 *      Author: JV
 */

#ifndef THRESHHELPERS_H_
#define THRESHHELPERS_H_
#include "BioFW/Evaluation/Statistics/ThreshedStatisticsSettings.h"
#include "BioFW/Evaluation/Statistics/BasicStatistics.h"

namespace BioFW {
	namespace Statistics {

		class ThreshHelpers {
			public:
				static ThreshedStatisticsSettings::Direction shouldAcceptBellow(const BasicStatistics & basicStatistics);
		};

	} /* namespace Statistics */
} /* namespace BioFW */
#endif /* THRESHHELPERS_H_ */
