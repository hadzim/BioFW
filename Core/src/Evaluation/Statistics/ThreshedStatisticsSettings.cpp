/*
 * ThreshedStatisticsSettings.cpp
 *
 *  Created on: 29.1.2014
 *      Author: JV
 */

#include "BioFW/Evaluation/Statistics/ThreshedStatisticsSettings.h"

namespace BioFW {
	namespace Statistics {


		ThreshedStatisticsSettings::ThreshedStatisticsSettings(double thresh, Direction direction)
			: thresh(thresh), direction(direction)  {

		}

		double ThreshedStatisticsSettings::getThresh() const {
			return this->thresh;
		}

		ThreshedStatisticsSettings::Direction ThreshedStatisticsSettings::getDirection() const {
			return this->direction;
		}

	} /* namespace Statistics */
} /* namespace BioFW */
