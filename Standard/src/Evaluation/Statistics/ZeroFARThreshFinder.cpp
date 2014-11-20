/*
 * ZeroFARThreshFinder.cpp
 *
 *  Created on: 30.1.2014
 *      Author: JV
 */

#include "BioFW/Evaluation/Statistics/ZeroFARThreshFinder.h"
#include "ThreshHelpers.h"
#include "BioFW/Exception.h"

namespace BioFW {
	namespace Statistics {

		ThreshedStatisticsSettings ZeroFARThreshFinder::findThresh(const BasicStatistics & basicStatistics) {
			if (basicStatistics.getImpostorCount() > 0) {

				double thresh = 0.0;
				ThreshedStatisticsSettings::Direction direction  = ThreshHelpers::shouldAcceptBellow(basicStatistics);

				if (direction == ThreshedStatisticsSettings::AcceptBelowThresh) {
					thresh = basicStatistics.getImpostorExtremes().getMin();
				} else {
					thresh = basicStatistics.getImpostorExtremes().getMax();
				}
				return ThreshedStatisticsSettings(thresh, direction);
			} else {
				throw BioFWException("Statistics does not contain any impostor data");
			}
		}

	} /* namespace Statistics */
} /* namespace BioFW */
