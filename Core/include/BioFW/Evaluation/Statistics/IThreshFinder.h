/*
 * IThreshFinder.h
 *
 *  Created on: 30.1.2014
 *      Author: JV
 */

#ifndef ITHRESHFINDER_H_
#define ITHRESHFINDER_H_

#include <memory>
#include "BioFW/Evaluation/Statistics/ThreshedStatisticsSettings.h"
#include "BioFW/Evaluation/Statistics/BasicStatistics.h"

namespace BioFW {
	namespace Statistics {

		class IThreshFinder {
			public:
				typedef std::shared_ptr <IThreshFinder> Ptr;
				virtual ~IThreshFinder();


				virtual ThreshedStatisticsSettings findThresh(const BasicStatistics & basicStatistics) = 0;

		};

	} /* namespace Statistics */
} /* namespace BioFW */
#endif /* ITHRESHFINDER_H_ */
