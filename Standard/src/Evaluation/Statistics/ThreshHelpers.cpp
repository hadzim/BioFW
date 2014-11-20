/*
 * ThreshHelpers.cpp
 *
 *  Created on: 30.1.2014
 *      Author: JV
 */

#include "ThreshHelpers.h"

namespace BioFW {
	namespace Statistics {

		ThreshedStatisticsSettings::Direction ThreshHelpers::shouldAcceptBellow(const BasicStatistics & basicStatistics) {
			if (basicStatistics.getImpostorExtremes().getMin() > basicStatistics.getGlobalExtremes().getMin()) {
				return ThreshedStatisticsSettings::AcceptBelowThresh;
			} else {
				return ThreshedStatisticsSettings::AcceptAboveThresh;
			}
		}

	} /* namespace Statistics */
} /* namespace BioFW */
