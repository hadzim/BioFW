/*
 * ThreshedStatistics.h
 *
 *  Created on: 29.1.2014
 *      Author: JV
 */

#ifndef THRESHEDSTATISTICS_H_
#define THRESHEDSTATISTICS_H_
#include <Poco/BasicEvent.h>
#include "ThreshedStatisticsSettings.h"
#include "BioFW/Evaluation/Statistics/BasicStatistics.h"
#include "Extremes.h"
#include "IThreshFinder.h"

namespace BioFW {
	namespace Statistics {

		class ThreshedStatistics {
			public:
				Poco::BasicEvent<ThreshedStatisticsSettings> ThreshChanged;

				ThreshedStatistics(BasicStatistics statistics, ThreshedStatisticsSettings threshSettings);
				ThreshedStatistics(BasicStatistics statistics, IThreshFinder::Ptr threshFinder);

				double getThresh() const;
				ThreshedStatisticsSettings::Direction getDirection() const;

				const Extremes & getOverlapArea() const;
				double getTrueAcceptance() const;
				double getFalseAcceptance() const;
				double getTrueRejection() const;
				double getFalseRejection() const;

				double getFAR() const;
				double getFRR() const;

				ThreshedStatisticsSettings getThreshSettings() const;
				void setThreshSettings(const ThreshedStatisticsSettings & tsettings);
			private:
				void addScoreToStatistics(const ComparisonScore & ms, bool isGenuine);
				void refresh();
				void clear();
			private:
				BasicStatistics statistics;
				ThreshedStatisticsSettings threshSettings;

				Extremes overlapArea;

				double trueAcceptance;
				double falseAcceptance;
				double trueRejection;
				double falseRejection;

		};

	} /* namespace Statistics */
} /* namespace BioFW */
#endif /* THRESHEDSTATISTICS_H_ */
