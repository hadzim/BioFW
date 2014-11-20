/*
 * ThreshedStatisticsSettings.h
 *
 *  Created on: 29.1.2014
 *      Author: JV
 */

#ifndef THRESHEDSTATISTICSSETTINGS_H_
#define THRESHEDSTATISTICSSETTINGS_H_

namespace BioFW {
	namespace Statistics {

		class ThreshedStatisticsSettings {
			public:
	        enum Direction {
	            AcceptBelowThresh,
	            AcceptAboveThresh
	        };

	        explicit ThreshedStatisticsSettings(double thresh, Direction direction = AcceptAboveThresh);

	        double getThresh() const;
	        Direction getDirection() const;
		private:
			double thresh;
			Direction direction;
		};

	} /* namespace Statistics */
} /* namespace BioFW */
#endif /* THRESHEDSTATISTICSSETTINGS_H_ */
