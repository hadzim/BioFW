/*
 * ZeroFRRThreshFinder.cpp
 *
 *  Created on: 30.1.2014
 *      Author: JV
 */

#include "BioFW/Evaluation/Statistics/ZeroFRRThreshFinder.h"
#include "ThreshHelpers.h"
#include "BioFW/Exception.h"

namespace BioFW {
	namespace Statistics {

		ThreshedStatisticsSettings ZeroFRRThreshFinder::findThresh(const BasicStatistics & basicStatistics) {
			if (basicStatistics.getGenuineCount() > 0) {

				double thresh = 0.0;
				ThreshedStatisticsSettings::Direction direction = ThreshHelpers::shouldAcceptBellow(basicStatistics);

				if (direction == ThreshedStatisticsSettings::AcceptBelowThresh) {
					thresh = basicStatistics.getGenuineExtremes().getMax();
				} else {
					thresh = basicStatistics.getGenuineExtremes().getMin();
				}
				return ThreshedStatisticsSettings(thresh, direction);
			} else {
				throw BioFWException("Statistics does not contain any genuine data");
			}
		}

	} /* namespace Statistics */
} /* namespace BioFW */
