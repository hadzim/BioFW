/*
 * EERThreshFinder.cpp
 *
 *  Created on: 30.1.2014
 *      Author: JV
 */

#include "BioFW/Evaluation/Statistics/EERThreshFinder.h"
#include <cmath>
#include "ThreshHelpers.h"
#include "BioFW/Evaluation/Statistics/ThreshedStatistics.h"
namespace BioFW {
	namespace Statistics {





        static double findEERThresh(const BasicStatistics & basicStatistics, ThreshedStatisticsSettings::Direction d, double min, double max, int iter) {

            if (iter > 50) return min;

            ThreshedStatistics minStat(basicStatistics, ThreshedStatisticsSettings(min, d));
            ThreshedStatistics maxStat(basicStatistics, ThreshedStatisticsSettings(max, d));

            if (minStat.getFAR() == minStat.getFRR()){
                return min;
            }

            if (std::abs(minStat.getFAR() - minStat.getFRR()) < 0.001) {
                return min;
            }

            if (minStat.getFalseAcceptance() == maxStat.getFalseAcceptance()){
                return min;
            } else {
                double middle = (min + max) / 2.0;
                ThreshedStatistics middleStat(basicStatistics, ThreshedStatisticsSettings(middle, d));

                if (middleStat.getFAR() < middleStat.getFRR()){
                    if (d == ThreshedStatisticsSettings::AcceptBelowThresh){
                        return findEERThresh(basicStatistics, d, middle, max, ++iter);
                    } else {
                        return findEERThresh(basicStatistics, d, min, middle, ++iter);
                    }
                } else {
                    if (d == ThreshedStatisticsSettings::AcceptBelowThresh) {
                        return findEERThresh(basicStatistics, d, min, middle, ++iter);
                    } else {
                        return findEERThresh(basicStatistics, d, middle, max, ++iter);
                    }
                }

            }

        }

        ThreshedStatisticsSettings EERThreshFinder::findThresh(const BasicStatistics & basicStatistics){

        	ThreshedStatisticsSettings::Direction finalDirection = ThreshHelpers::shouldAcceptBellow(basicStatistics);
        	double finalThresh = findEERThresh(basicStatistics, finalDirection, basicStatistics.getGlobalExtremes().getMin(), basicStatistics.getGlobalExtremes().getMax(), 0);

            //compute
            ThreshedStatistics ts(basicStatistics, ThreshedStatisticsSettings(finalThresh, finalDirection));
            if (ts.getFAR() > 0.5 && ts.getFRR() > 0.5) {
                if (finalDirection == ThreshedStatisticsSettings::AcceptAboveThresh) {
                	finalDirection = ThreshedStatisticsSettings::AcceptBelowThresh;
                } else {
                	finalDirection = ThreshedStatisticsSettings::AcceptAboveThresh;
                }
            }

            return ThreshedStatisticsSettings(finalThresh, finalDirection);
        }


	} /* namespace Statistics */
} /* namespace BioFW */
